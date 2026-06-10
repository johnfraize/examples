// cdev_demo.c — a refresher character driver.
// Exercises the muscles you'll want fresh for HPR: cdev/class registration,
// file_operations, copy_to/from_user, unlocked_ioctl, and zero-copy mmap of a
// kernel buffer into userspace (the FPGA-BAR / packet-ring idiom).
//
//   make            # build against running kernel
//   sudo insmod cdev_demo.ko
//   dmesg | tail    # note the major; udev creates /dev/cdev_demo
//   ./test          # userspace exerciser
//   sudo rmmod cdev_demo

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/version.h>

#define DEV_NAME   "cdev_demo"
#define BUF_SIZE   PAGE_SIZE

// --- ioctl contract (keep in sync with test.c) ---
#define DEMO_IOC_MAGIC      'D'
#define DEMO_IOC_RESET      _IO(DEMO_IOC_MAGIC, 0)
#define DEMO_IOC_GET_LEN    _IOR(DEMO_IOC_MAGIC, 1, int)
#define DEMO_IOC_SET_LEN    _IOW(DEMO_IOC_MAGIC, 2, int)

static dev_t        dev_num;
static struct cdev  demo_cdev;
static struct class *demo_class;

static char *kbuf;          // backs read()/write()
static void *mmap_buf;      // vmalloc_user buffer exposed via mmap()
static int   data_len;

static ssize_t demo_read(struct file *f, char __user *ubuf, size_t len, loff_t *off)
{
    if (*off >= data_len)
        return 0;                                  // EOF
    if (len > data_len - *off)
        len = data_len - *off;
    if (copy_to_user(ubuf, kbuf + *off, len))
        return -EFAULT;
    *off += len;
    return len;
}

static ssize_t demo_write(struct file *f, const char __user *ubuf, size_t len, loff_t *off)
{
    if (len > BUF_SIZE)
        len = BUF_SIZE;
    if (copy_from_user(kbuf, ubuf, len))
        return -EFAULT;
    data_len = len;
    *off = 0;
    pr_info("%s: stored %zu bytes\n", DEV_NAME, len);
    return len;
}

static long demo_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    int v;
    switch (cmd) {
    case DEMO_IOC_RESET:
        data_len = 0;
        memset(kbuf, 0, BUF_SIZE);
        return 0;
    case DEMO_IOC_GET_LEN:
        return put_user(data_len, (int __user *)arg);
    case DEMO_IOC_SET_LEN:
        if (get_user(v, (int __user *)arg))
            return -EFAULT;
        if (v < 0 || v > BUF_SIZE)
            return -EINVAL;
        data_len = v;
        return 0;
    default:
        return -ENOTTY;
    }
}

// Zero-copy: map the kernel's vmalloc_user buffer straight into the process.
// This is the heart of high-perf device drivers — userspace touches device/ring
// memory with no copy_to_user in the hot path.
static int demo_mmap(struct file *f, struct vm_area_struct *vma)
{
    unsigned long sz = vma->vm_end - vma->vm_start;
    if (sz > BUF_SIZE)
        return -EINVAL;
    return remap_vmalloc_range(vma, mmap_buf, 0);
}

static const struct file_operations demo_fops = {
    .owner          = THIS_MODULE,
    .read           = demo_read,
    .write          = demo_write,
    .unlocked_ioctl = demo_ioctl,
    .mmap           = demo_mmap,
};

static int __init demo_init(void)
{
    int ret;

    kbuf = kzalloc(BUF_SIZE, GFP_KERNEL);
    if (!kbuf)
        return -ENOMEM;
    mmap_buf = vmalloc_user(BUF_SIZE);   // zeroed, page-aligned, mmap-safe
    if (!mmap_buf) { ret = -ENOMEM; goto err_kbuf; }

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
    if (ret) goto err_vmalloc;

    cdev_init(&demo_cdev, &demo_fops);
    demo_cdev.owner = THIS_MODULE;
    ret = cdev_add(&demo_cdev, dev_num, 1);
    if (ret) goto err_region;

    // class_create lost its owner arg in 6.4 — guard so it builds on either.
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    demo_class = class_create(DEV_NAME);
#else
    demo_class = class_create(THIS_MODULE, DEV_NAME);
#endif
    if (IS_ERR(demo_class)) { ret = PTR_ERR(demo_class); goto err_cdev; }

    device_create(demo_class, NULL, dev_num, NULL, DEV_NAME);  // udev -> /dev/cdev_demo
    pr_info("%s: loaded, major=%d\n", DEV_NAME, MAJOR(dev_num));
    return 0;

err_cdev:   cdev_del(&demo_cdev);
err_region: unregister_chrdev_region(dev_num, 1);
err_vmalloc:vfree(mmap_buf);
err_kbuf:   kfree(kbuf);
    return ret;
}

static void __exit demo_exit(void)
{
    device_destroy(demo_class, dev_num);
    class_destroy(demo_class);
    cdev_del(&demo_cdev);
    unregister_chrdev_region(dev_num, 1);
    vfree(mmap_buf);
    kfree(kbuf);
    pr_info("%s: unloaded\n", DEV_NAME);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("John Fraize");
MODULE_DESCRIPTION("Refresher char driver: fops, ioctl, zero-copy mmap");
