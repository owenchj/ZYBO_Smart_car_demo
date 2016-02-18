#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/uaccess.h>
 /* Needed for copy_from_user */
#include <asm/io.h>
 /* Needed for IO Read/Write Functions */
#include <linux/proc_fs.h> /* Needed for Proc File System Functions */
#include <linux/seq_file.h> /* Needed for Sequence File Operations */
#include <linux/platform_device.h> /* Needed for Platform Driver Functions */
#include <linux/slab.h>
#include <linux/of_irq.h>
// Add
#include <linux/miscdevice.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

 /* Define Driver Name */
#define DRIVER_NAME "myps2pl"
struct resource *io;
unsigned long remap_size;
char RI[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
char WI[4] = {0x00, 0x00, 0x00, 0x00};

int flag = 0;

/* Vitual Base Address */
/* Device Resource Structure */
/* Device Memory Size */
/* Write operation for /proc/myps2pl
 * -----------------------------------
 * When user cat a string to /proc/myps2pl file, the string will be stored in
 * const char __user *buf. This function will copy the string from user
 * space into kernel space, and change it to an unsigned long value.
 * It will then write the value to the register of myps2pl controller,
 * and turn on the corresponding PS2PLs eventually.
 */


struct myps2pl_device {
  /* Données propres à un périphérique (exemple) */
  void __iomem *regs;
  struct device *dev;
  struct miscdevice miscdev;
  int irq;
  int parameter_x;
};



static ssize_t my_ps2pl_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos){
  struct miscdevice *mdev = file->private_data;
  struct myps2pl_device *mpspl;
  int err;
  int i = 0;
  int data = 0;
 
  mpspl = container_of(mdev, struct myps2pl_device, miscdev);

  err = copy_from_user(RI, buf, count);

  for( i = 0; i < 5 ; i++){
    data =  0 | RI[i];
    dev_info(mpspl->dev, "Data:%d \n", data);    

    iowrite32(data, mpspl->regs + 4 * i);
  }
  
  if (err < 0){
    dev_err(mpspl->dev,"failed to copy to user space, total = %d\n", count);
    return err;
  }
  return count;
}



static int my_ps2pl_open(struct inode *inode, struct file *file)
{
  struct miscdevice *mdev = file->private_data;
  struct myps2pl_device *mpspl;

  //  iowrite32(0, base_addr);
  mpspl = container_of(mdev, struct myps2pl_device, miscdev);

  return 0;
}


static ssize_t my_pl2ps_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos){
  struct miscdevice *mdev = file->private_data;
  struct myps2pl_device *mplps;
  int err;
  int i = 0;
  int data = 0;

  mplps = container_of(mdev, struct myps2pl_device, miscdev);
  
  for( i = 0; i < 4 ; i++){
    data = ioread32(mplps->regs + 4 * i);
    WI[i] = (data) & 0xFF;
  }
  
  err = copy_to_user(buf, WI, count);

  
  if (err < 0){
    dev_err(mplps->dev,"failed to copy to user space, total = %d\n", count);
    return err;
  }
  return count;
}


static const struct file_operations my_ps2pl_fops = {
  .owner = THIS_MODULE,
  //	.llseek = no_llseek,
  .write = my_ps2pl_write,
  .open  = my_ps2pl_open,
  //.unlocked_ioctl = xilinx_vdma_unlocked_ioctl
};


static const struct file_operations my_pl2ps_fops = {
  .owner = THIS_MODULE,
  //	.llseek = no_llseek,
  .read  = my_pl2ps_read,
  .open  = my_ps2pl_open,
  //  .unlocked_ioctl = xilinx_vdma_unlocked_ioctl_filter
};



static irqreturn_t myps2pl_irq_handler(int irq, void *data)
{
  struct myps2pl_device *mplps = data;
  int i = 0;
  int read_data = 0;

  dev_info(mplps->dev, "Interruption\n");
  
  for( i = 0; i < 4 ; i++){
    read_data = ioread32(mplps->regs + 4 * i);
    WI[i] = (read_data) & 0xFF;
  }
  
  return IRQ_HANDLED;
}






/* Remove function for myps2pl
 * ----------------------------------
 * When myps2pl module is removed, turn off all the ps2pls first,
 * release virtual address and the memory region requested.
 */
static int myps2pl_remove(struct platform_device *pdev)
{

  struct myps2pl_device *mdev = platform_get_drvdata(pdev);
  
  if (mdev->irq > 0)
    free_irq(mdev->irq, mdev);
  
  /* /\* Remove /proc/myps2pl entry *\/ */
  /* remove_proc_entry(DRIVER_NAME, NULL); */
  /* /\* Release mapped virtual address *\/ */
  /* iounmap(mdev->regs); */
  /* /\* Release the region *\/ */
  /* release_mem_region(io->start, remap_size); */

  misc_deregister(&mdev->miscdev);

  return 0;
}




/* Device Probe function for myps2pl
 * ------------------------------------
 * Get the resource structure from the information in device tree.
 * request the memory regioon needed for the controller, and map it into
 * kernel virtual memory space. Create an entry under /proc file system
 * and register file operations for that entry.
 */
static int myps2pl_probe(struct platform_device *pdev)
{

  struct device_node *node = pdev->dev.of_node;
  struct myps2pl_device *mdev;

  int err = 0;
  
  
  mdev = devm_kzalloc(&pdev->dev, sizeof(*mdev), GFP_KERNEL);
  if (!mdev)
    return -ENOMEM;

  mdev->dev = &pdev->dev;

  /* Request and map I/O memory */
  io = platform_get_resource(pdev, IORESOURCE_MEM, 0);
  
  dev_info(mdev->dev, "addr %x\n", io->start);
 
  mdev->regs = devm_ioremap_resource(&pdev->dev, io);
 
  if (IS_ERR(mdev->regs))
    return PTR_ERR(mdev->regs);

  platform_set_drvdata(pdev, mdev);

  mdev->irq = irq_of_parse_and_map(node, 0);
  
  printk(KERN_INFO DRIVER_NAME " irq 0x%08lx\n",
	 (unsigned long) mdev->irq);
  
  if(mdev->irq > 0){
    /* err = devm_request_threaded_irq(&mdev->dev, mdev->irq, */
    /* 				  NULL, myps2pl_irq_handler, */
    /* 				  IRQF_ONESHOT, */
    /* 				    "myps2pl-controller", mdev); */
  			

    err = request_irq(mdev->irq, myps2pl_irq_handler, IRQF_SHARED,
		      "myps2pl-controller", mdev);
    if (err) {
      dev_err(mdev->dev, "unable to request IRQ %d\n", mdev->irq);
      return err;
    }
  }


  if(flag == 0){
    dev_info(mdev->dev, "my_ps2pl"); 
    mdev->miscdev.minor = 0x33;
    mdev->miscdev.name = "my_ps2pl";
    mdev->miscdev.fops = &my_ps2pl_fops;
    mdev->miscdev.parent = mdev->dev; /* (1) */
    flag = 1;
  }
  else{
    dev_info(mdev->dev, "my_pl2ps"); 
    mdev->miscdev.minor = 0x34;
    mdev->miscdev.name = "my_pl2ps";
    mdev->miscdev.fops = &my_pl2ps_fops;
    mdev->miscdev.parent = mdev->dev; /* (1) */
  }
  
  err = misc_register(&mdev->miscdev);

  if (err < 0) {
    dev_err(&pdev->dev, "Unable to register miscdev to DT\n");
    goto error;
  }
  
  return 0;

 error:
  dev_err(&pdev->dev, "Unable to insert driver\n");

  return err;
}


/* device match table to match with device node in device tree */
static const struct of_device_id myps2pl_of_match[] = {
  {.compatible = "xlnx,instruction-PL2PS-PS2PL-1.0"},
  {},
};

MODULE_DEVICE_TABLE(of, myps2pl_of_match);
/* platform driver structure for myps2pl driver */
static struct platform_driver myps2pl_driver = {
  .driver = {
    .name = DRIVER_NAME,
    .owner = THIS_MODULE,
    .of_match_table = myps2pl_of_match},
  .probe = myps2pl_probe,
  .remove = myps2pl_remove
};
/* Register myps2pl platform driver */
module_platform_driver(myps2pl_driver);
/* Module Infomations */
MODULE_AUTHOR("jchen.");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(DRIVER_NAME ": MYPS2PL driver (Simple Version)");
MODULE_ALIAS(DRIVER_NAME);
