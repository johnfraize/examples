// SPDX-License-Identifier: GPL-2.0
/*
 * rcu_example.c - RCU pointer-swap idiom for a read-mostly config pointer.
 *
 * Demonstrates the canonical kernel pattern for swapping in a new immutable
 * object on a hot read path: readers use rcu_read_lock()/rcu_dereference(),
 * the writer allocates-new -> rcu_assign_pointer() -> synchronize_rcu() ->
 * free-old. This gives readers correct publish/consume ordering without
 * taking any lock, and guarantees the old object is never freed while a
 * reader might still hold it.
 */

#include <linux/rcupdate.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

/* The object readers dereference. Treat as immutable once published. */
struct demo_config {
	u32 ring_size;
	u32 threshold;
};

/* The shared pointer. __rcu annotates it so sparse can check access rules. */
static struct demo_config __rcu *cfg;

/* Writers serialize against each other with this; readers never take it. */
static DEFINE_SPINLOCK(cfg_lock);

/* Reader (hot path - e.g. inside an ioctl or mmap fault handler). */
static u32 demo_get_threshold(void)
{
	struct demo_config *c;
	u32 val;

	rcu_read_lock();		/* marks start of RCU read-side section */
	c = rcu_dereference(cfg);	/* safe load + the right read barrier  */
	val = c->threshold;		/* deref guaranteed valid until...     */
	rcu_read_unlock();		/* ...here. After this, don't touch c. */

	return val;
}

/* Writer (slow path - swap in a new config). */
static int demo_set_threshold(u32 new_threshold)
{
	struct demo_config *new_cfg, *old_cfg;

	new_cfg = kmalloc(sizeof(*new_cfg), GFP_KERNEL);
	if (!new_cfg)
		return -ENOMEM;

	spin_lock(&cfg_lock);
	old_cfg = rcu_dereference_protected(cfg, lockdep_is_held(&cfg_lock));

	/* fully initialize the NEW object before anyone can see it */
	*new_cfg = *old_cfg;
	new_cfg->threshold = new_threshold;

	rcu_assign_pointer(cfg, new_cfg);	/* publish: release barrier + store */
	spin_unlock(&cfg_lock);

	synchronize_rcu();	/* wait for all pre-existing readers to finish */
	kfree(old_cfg);		/* now provably no reader holds old_cfg - safe free */
	return 0;
}
