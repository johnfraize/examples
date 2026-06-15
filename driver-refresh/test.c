// test.c — userspace exerciser for cdev_demo.
// Drives write/read, both ioctls, and the zero-copy mmap path.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#define DEMO_IOC_MAGIC   'D'
#define DEMO_IOC_RESET   _IO(DEMO_IOC_MAGIC, 0)
#define DEMO_IOC_GET_LEN _IOR(DEMO_IOC_MAGIC, 1, int)
#define DEMO_IOC_SET_LEN _IOW(DEMO_IOC_MAGIC, 2, int)
#define DEMO_IOC_IRQ_START _IO(DEMO_IOC_MAGIC, 3)
#define DEMO_IOC_IRQ_STOP  _IO(DEMO_IOC_MAGIC, 4)
#define DEMO_IOC_GET_IRQS  _IOR(DEMO_IOC_MAGIC, 5, int)
#define DEMO_IOC_WAIT_IRQ  _IO(DEMO_IOC_MAGIC, 6)
#define DEMO_IOC_SET_THRESH _IOW(DEMO_IOC_MAGIC, 7, int)
#define DEMO_IOC_GET_THRESH _IOR(DEMO_IOC_MAGIC, 8, int)

int main(void)
{
    int fd = open("/dev/cdev_demo", O_RDWR);
    if (fd < 0) { perror("open"); return 1; }

    const char *msg = "line-rate or bust\n";
    write(fd, msg, strlen(msg));

    int len = -1;
    ioctl(fd, DEMO_IOC_GET_LEN, &len);
    printf("driver reports data_len=%d\n", len);

    lseek(fd, 0, SEEK_SET);
    char buf[256] = {0};
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    printf("read back %zd bytes: %s", n, buf);

    // zero-copy: write through the mmap, read it via read() — no copy in between.
    char *map = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) { perror("mmap"); close(fd); return 1; }
    strcpy(map, "written via mmap, zero copy\n");
    int newlen = strlen(map);
    ioctl(fd, DEMO_IOC_SET_LEN, &newlen);
    lseek(fd, 0, SEEK_SET);
    memset(buf, 0, sizeof(buf));
    read(fd, buf, sizeof(buf) - 1);
    printf("read after mmap write: %s", buf);

    munmap(map, 4096);

    // simulated interrupt path: arm the IRQ source, block until two fire, read
    // the count, then disarm. WAIT_IRQ exercises the IRQ -> waitqueue wakeup.
    ioctl(fd, DEMO_IOC_IRQ_START, 0);
    for (int i = 0; i < 2; i++)
        ioctl(fd, DEMO_IOC_WAIT_IRQ, 0);   // blocks in the kernel until the next IRQ
    int irqs = 0;
    ioctl(fd, DEMO_IOC_GET_IRQS, &irqs);
    printf("simulated IRQs handled: %d\n", irqs);
    ioctl(fd, DEMO_IOC_IRQ_STOP, 0);

    // RCU config swap: set a new threshold (kernel does the allocate-new ->
    // publish -> synchronize_rcu -> free-old dance), then read it back.
    int thresh = 42;
    ioctl(fd, DEMO_IOC_SET_THRESH, &thresh);
    int got = -1;
    ioctl(fd, DEMO_IOC_GET_THRESH, &got);
    printf("RCU config threshold set=%d read=%d\n", thresh, got);

    close(fd);
    return 0;
}
