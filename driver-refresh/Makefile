# Kbuild out-of-tree module + userspace test.
obj-m += cdev_demo.o
obj-m += pcie_demo.o

KDIR ?= /lib/modules/$(shell uname -r)/build

all: module test

module:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

test: test.c
	$(CC) -Wall -O2 -o test test.c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f test

# Install the udev rule so /dev/cdev_demo is 0666 and ./test runs without root.
# Re-triggers udev so an already-loaded module's node gets the new mode too.
install-udev: 99-cdev_demo.rules
	cp 99-cdev_demo.rules /etc/udev/rules.d/
	udevadm control --reload-rules
	udevadm trigger --subsystem-match=cdev_demo

.PHONY: all module clean install-udev
