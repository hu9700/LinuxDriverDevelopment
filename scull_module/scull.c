#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>//include some file for character device
#include "scull.h"

//init file operation structure for scull
//struct file_operations defined in <linux/fs.h>
struct file_operations scull_fops = 
{
	.owner	= THIS_MODULE,//define in <linux/module.h>
	.llseek	= scull_llseek,
	.read	= scull_read,
	.write	= scull_write,
	.ioctal	= scull_ioctl,
	.open	= scull_open,
	.release= scull_release
};

//struct file, defined in <linux/fs.h>
//indicate one opened file. 
//When kernel open one file, it create one struct file, 
//and send struct file_operation to struct file
//some important member of struct file
//model_t f_mode
//loff_s f_pos;//long offset type
//unsigned int f_flags
//struct file_operations *f_op;
//void *private_data;//used to pointed to the data alloc
//struct dentry *f_dentry;//as usual, use filp->f_dentry->d_inode to save and load inode struct

//struct inode
//in kernel, there will be several filp, but only point to one inode
//two important member in inode
//dev_t i_rdev;
//struct cdev *i_cdev;//means char device

//to get major and minor from inode
//unsigned int imajor(struct inode *inode);
//unsigned int iminor(struct inode *inode);

//how to get the major and minor number from device
//defined in <linu/kdev_t.h>
//MAJOR(dev_t dev);
//MINOR(dev_t dev);

//make one dev_t from it's major and minor number
//MKDEV(int major, int minor);

//static register device number for several device
//defined in <linux/fs.h>
//char *name will appear in /proc/device and sysfs
//int register_chrdev_region(dev_t first, unsigned int count, char *name);

//dynamic allocate device number fo several device
//int alloc_chrdev_region(dev_t *dev, unsigned int first minor, unsigned int count, char *name);

//release/free the device number, as usual used in module clean up
//void unregister_chrdev_region(dev_t first, unsigned int count);

//user can define major in file
#ifdef SCULL_MAJOR
int scull_major = SCULL_MAJOR;
#else
int scull_major = 0;
#endif

int scull_minor = 0;
unsigned int scull_nr_devs = 4;
dev_t dev;

//user can also use insmod or use scull_load cal insmod to transfer major number
module_param(scull_major, int, S_IRUGO);

//we can give device major number by set by user or alloc dynamically
int scull_register_major_minor(void)
{
	int result = -1;

	if(scull_major)
	{//if user has set the major number
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, scull_nr_devs, "scull")
	}
	else
	{//if user didn't set the major number, we will dynamic allocate one
		result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, "scull");
		scull_major = MAJOR(dev);
	}

	if(result < 0)
	{
		printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
		return result;
	}
}

//init struct scull_dev, and add it to system
//we use cdev to help use to initialize sucll
//1, To ini cdev
//one way, get one isolated cdev
//struct cdev *my_dev = cdev_allco();
//my_cdev->ops = &my_fops;
//Other way, to put one cdev in our own structure
//void cdev_init(struct cdev *cdev, struct file_operations *fops);
//it init cder, set owner as THIS_MODULE, put fops in cdev
//2, to add it to kernel
//int cdev_add(struct cdev *cdev, dev_t dev, unsinged int count);
//dev is the first device related to cdev, count is the number of devices will attach to cdev
//cdev_add maybe will fail
//if cdev_add success, it's alive, then kernel can operate with that device
//so unless you finish all your preparation, otherwise do not call cdev_add
//3 remove character device
//void cdev_del(struct cdev *dev);you should not use cdev after that function XD
static void scull_setup_dev(struct scull_dev, dev_t *dev, int index)
{
	int err;
	dev_t dev_num = MKDEV(scull_major, scull_minor + index);

	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scull_fops;//do we really need that? we already did it in cdev_init
	err = cdev_add(&dev->cdev, dev_num, 1);

	if(err)
	{
		printk(KERN_NOTICE "Error %d adding scull %d", err, index);
	}
}

//some old way to add device, and do not use cdev
static void scull_setup_dev_old()
{
	register_chrdev(scull_major, "scull", &scull_fops);//"scull" will appear in /proc/devices
	//to unregister
	//unregister_chrdev(scull_major, "scull");
}

void scull_cleanup(void)
{
	unregister_chrdev_region(dev, scull_nr_devs);
}
