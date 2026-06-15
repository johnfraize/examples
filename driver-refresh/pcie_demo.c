// pcie_demo.c — canonical PCIe driver skeleton for interview talk-track.
//
// This is the Day-8 "bullseye" shape for an FPGA-card shop:
//   probe -> enable -> map BAR (MMIO) -> set DMA mask -> alloc coherent DMA
//   ring -> allocate MSI/MSI-X vector -> request_threaded_irq -> hand the ring
//   address to the device. The hard-IRQ top half just acks + wakes the thread;
//   the threaded bottom half drains the DMA ring (the deferrable heavy work).
//
// There is no real card behind this, so the driver REGISTERS but never .probe()s
// on this box (no matching vendor/device). That is expected and the point: it
// compiles clean against the running kernel and gives you the full idiom to walk
// a reviewer through, line by line. To bind it to a real device you'd set the
// id_table to that card's vendor/device (Xilinx is 0x10ee, e.g.).
//
//   make                 # builds pcie_demo.ko alongside cdev_demo.ko
//   sudo insmod pcie_demo.ko
//   dmesg | tail         # "registered; waiting for a matching device"
//   sudo rmmod pcie_demo

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>
#include <linux/atomic.h>

#define DRV_NAME        "pcie_demo"

// Placeholder IDs — there is no card with these on the bus, so probe never runs
// here. Swap to the real card's vendor/device to bind. PCI_ANY_ID would match
// anything of a vendor; we keep it specific and fake on purpose.
#define DEMO_VENDOR_ID  0x1234
#define DEMO_DEVICE_ID  0x0001

// Fake-but-plausible BAR0 register map (offsets into the MMIO window). On a real
// FPGA these come from the card's register spec.
#define REG_STATUS      0x00   // RO: bit0 = DMA-done IRQ pending
#define REG_CONTROL     0x04   // WO: bit0 = enable, bit1 = start DMA
#define REG_IRQ_ACK     0x08   // W1C: write the status bits to clear the IRQ
#define REG_DMA_ADDR_LO 0x10   // device's view of the ring's bus address (low)
#define REG_DMA_ADDR_HI 0x14   // ... (high)
#define REG_DMA_LEN     0x18   // ring size in bytes

#define STATUS_DMA_DONE BIT(0)

#define DMA_RING_BYTES  (4 * PAGE_SIZE)   // coherent DMA ring

// Per-device state. Everything probe() acquires lives here and is stashed via
// pci_set_drvdata so remove()/the IRQ handler can find it.
struct pcie_demo {
    struct pci_dev *pdev;
    void __iomem   *bar0;        // MMIO window from pci_iomap(BAR0)
    void           *dma_ring;    // CPU-side coherent ring (kernel VA)
    dma_addr_t      dma_handle;  // device-side bus address of the same ring
    int             irq;         // IRQ line/vector after MSI(-X) allocation
    atomic_t        irqs;        // top-half fires (cheap stat, IRQ-safe)
    atomic_t        completions; // bottom-half ring drains
};

// --- Bottom half: threaded IRQ handler. Runs in process context, MAY sleep.
// This is where you'd walk the DMA ring, hand descriptors/packets up the stack,
// and recycle buffers — the heavy, deferrable work kept OFF the hard-IRQ path.
static irqreturn_t pcie_demo_thread(int irq, void *data)
{
    struct pcie_demo *d = data;

    // ... iterate the coherent ring, process completed descriptors ...
    // Coherent DMA memory needs no explicit cache sync; if this were STREAMING
    // DMA you'd dma_sync_single_for_cpu() before reading device-written data.
    atomic_inc(&d->completions);
    return IRQ_HANDLED;
}

// --- Top half: hard-IRQ handler. Tiny and bounded — no sleeping, no locks that
// a sleeping path holds. Confirm the IRQ is ours, ack it at the device, and
// defer everything else to the thread. That bounded-time handler is the
// latency-determinism story.
static irqreturn_t pcie_demo_isr(int irq, void *data)
{
    struct pcie_demo *d = data;
    u32 status = readl(d->bar0 + REG_STATUS);

    if (!(status & STATUS_DMA_DONE))
        return IRQ_NONE;                 // shared line: not our interrupt

    writel(status, d->bar0 + REG_IRQ_ACK); // W1C ack so the line de-asserts
    atomic_inc(&d->irqs);
    return IRQ_WAKE_THREAD;               // kick the bottom half
}

static int pcie_demo_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
    struct pcie_demo *d;
    int ret;

    d = devm_kzalloc(&pdev->dev, sizeof(*d), GFP_KERNEL);
    if (!d)
        return -ENOMEM;
    d->pdev = pdev;
    atomic_set(&d->irqs, 0);
    atomic_set(&d->completions, 0);

    ret = pci_enable_device(pdev);       // power up, assign resources
    if (ret)
        return ret;
    pci_set_master(pdev);                // enable bus mastering — required for DMA

    ret = pci_request_regions(pdev, DRV_NAME);  // claim the BARs
    if (ret)
        goto err_disable;

    d->bar0 = pci_iomap(pdev, 0, 0);     // map BAR0 MMIO into kernel VA
    if (!d->bar0) { ret = -ENOMEM; goto err_regions; }

    // 64-bit DMA if the device can address it; the API falls back to 32-bit.
    ret = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(64));
    if (ret) {
        ret = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
        if (ret)
            goto err_unmap;
    }

    // Coherent ring: CPU and device see the same memory without manual cache
    // flush/invalidate. dma_handle is what the DEVICE programs into its DMA
    // engine — never hand it a kernel virtual address.
    d->dma_ring = dma_alloc_coherent(&pdev->dev, DMA_RING_BYTES,
                                     &d->dma_handle, GFP_KERNEL);
    if (!d->dma_ring) { ret = -ENOMEM; goto err_unmap; }

    // MSI/MSI-X preferred over legacy INTx: edge-triggered, no shared line.
    ret = pci_alloc_irq_vectors(pdev, 1, 1,
                                PCI_IRQ_MSIX | PCI_IRQ_MSI | PCI_IRQ_INTX);
    if (ret < 0)
        goto err_dma;
    d->irq = pci_irq_vector(pdev, 0);

    // Threaded IRQ = the kernel's built-in top/bottom-half split: isr runs in
    // hard-IRQ context, thread runs in process context and may sleep.
    ret = request_threaded_irq(d->irq, pcie_demo_isr, pcie_demo_thread,
                               IRQF_SHARED, DRV_NAME, d);
    if (ret)
        goto err_vectors;

    // Tell the device where the ring lives (its bus-address view) and arm it.
    writel(lower_32_bits(d->dma_handle), d->bar0 + REG_DMA_ADDR_LO);
    writel(upper_32_bits(d->dma_handle), d->bar0 + REG_DMA_ADDR_HI);
    writel(DMA_RING_BYTES,               d->bar0 + REG_DMA_LEN);
    writel(0x1,                          d->bar0 + REG_CONTROL);  // enable

    pci_set_drvdata(pdev, d);
    dev_info(&pdev->dev, "%s: bound, BAR0=%p ring=%pad irq=%d\n",
             DRV_NAME, d->bar0, &d->dma_handle, d->irq);
    return 0;

    // Teardown ladder — exact reverse order of acquisition.
err_vectors: pci_free_irq_vectors(pdev);
err_dma:     dma_free_coherent(&pdev->dev, DMA_RING_BYTES, d->dma_ring, d->dma_handle);
err_unmap:   pci_iounmap(pdev, d->bar0);
err_regions: pci_release_regions(pdev);
err_disable: pci_disable_device(pdev);
    return ret;
}

static void pcie_demo_remove(struct pci_dev *pdev)
{
    struct pcie_demo *d = pci_get_drvdata(pdev);

    writel(0x0, d->bar0 + REG_CONTROL);   // quiesce the device first
    free_irq(d->irq, d);                  // sync: no handler runs after this
    pci_free_irq_vectors(pdev);
    dma_free_coherent(&pdev->dev, DMA_RING_BYTES, d->dma_ring, d->dma_handle);
    pci_iounmap(pdev, d->bar0);
    pci_release_regions(pdev);
    pci_disable_device(pdev);
    dev_info(&pdev->dev, "%s: removed\n", DRV_NAME);
}

static const struct pci_device_id pcie_demo_ids[] = {
    { PCI_DEVICE(DEMO_VENDOR_ID, DEMO_DEVICE_ID) },
    { 0, }
};
MODULE_DEVICE_TABLE(pci, pcie_demo_ids);

static struct pci_driver pcie_demo_driver = {
    .name     = DRV_NAME,
    .id_table = pcie_demo_ids,
    .probe    = pcie_demo_probe,
    .remove   = pcie_demo_remove,
};

module_pci_driver(pcie_demo_driver);   // expands to init/exit that (un)register

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John Fraize");
MODULE_DESCRIPTION("PCIe driver skeleton: probe, BAR map, coherent DMA ring, threaded MSI IRQ");
