# interesting driver quirks
Study bits from working on linux drivers

## atomics

- atomic_t is a signed int, so the usable range is INT_MIN to INT_MAX, there is no unsigned version of an atomic_t
- there is no longer a 24 bit limitation
- if I need a pointer sized atomic use atomic_long_t

## publishing a pointer to other CPUs
- xchg(&ptr,new)  and   cmpxchg(&ptr,old, new) these  are lock free


## current reading pointers with , occasional updates

The linux way of hot read path swapping ina new buffer

- rcu_dereference() , rcu_assign_pointer() 

## API churn — the stuff every stale tutorial gets wrong

The interfaces moved after 4.x. These are the ones that actually cost time:

- class_create() lost its owner (THIS_MODULE) first arg in 6.4. Takes ONE arg on a
  6.x kernel. Every tutorial older than 2023 shows two and the compile error is not
  obvious. Guarded by LINUX_VERSION_CODE in cdev_demo.c
- platform_driver .remove used to return int. There was a .remove_new transition and
  modern kernels take a void return. If the compiler disagrees about the return type,
  that's the fix
- ioctl is unlocked_ioctl now, not the old .ioctl with the BKL
- pr_info() / dev_info() over bare printk()

## MODULE_LICENSE("GPL") is not paperwork

Omit it and half the kernel API vanishes — GPL-only exported symbols won't resolve,
so you get "Unknown symbol" at insmod and the module taints the kernel.

## vermagic

insmod: invalid module format almost always means the headers you built against
don't match the running kernel, not that the module is broken.






