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

.PHONY: all module clean
