#include "scull_head.h"

module_param(scull_quantum, int, S_IRUGO);
module_param(scull_qset, int, S_IRUGO);
module_param(scull_major, int, S_IRUGO);

static int scull_register_major_minor(void)
{
	int result = -1;
	
	dev_t dev;
	
	if(scull_major)
	{//if user has set the major number
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, scull_nr_devs, "scull");
	}
	else
	{//if user didn't set the major number, we will dynamic allocate one
		result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, "scull");
		scull_major = MAJOR(dev);
	}

	if(result < 0)
	{
		printk(KERN_WARNING "hu: scull: can't get major %d\n", scull_major);
	}
	
	return result;
}

static int scull_setup_cdev(struct scull_dev *dev, int index)
{
	int err;
	dev_t dev_num = MKDEV(scull_major, scull_minor + index);

	cdev_init(&(dev->cdev), &scull_fops);
//	(dev->cdev).owner = THIS_MODULE;
//	(dev->cdev).ops = &scull_fops;//do we really need that? we already did it in cdev_init
	err = cdev_add(&(dev->cdev), dev_num, 1);

	if(err)
	{
		printk(KERN_NOTICE "hu: Error %d adding scull %d", err, index);
	}
	
	return err;
}

static int scull_dev_init(void)
{
	int i;
	int result = 0;
	
	for(i = 0; i < scull_nr_devs && result == 0; i++)
	{
		(scull_devices[i]).quantum = scull_quantum;
		(scull_devices[i]).qset = scull_qset;
		init_MUTEX(&((scull_devices[i]).sem));
		result = scull_setup_cdev(&(scull_devices[i]), i);
	}
	
	return result;
}

struct scull_qset * scull_follow(struct scull_dev *dev, int item)
{

}

//to free all the memory in scull for quantum
int scull_trim(struct scull_dev *dev)
{
	struct scull_qset *next, *dptr;
	int qset = dev->qset;//"dev" is not NULL
	int i;
	int result = 0;

	for(dptr = dev->data; dptr; dptr = next)
	{
		//all the list item
		if(dptr->data)
		{
			for(i = 0; i < qset; i ++)
			{
				kfree(dptr->data[i]);
			}
			kfree(dptr->data);
			dptr->data = NULL;
		}

		next = dptr->next;
		kfree(dptr);
	}
	
	return result;
}

static void scull_cleanup(void)
{
	int i;
	
	//unregister cdev
	for(i = 0; i < scull_nr_devs; i ++)
	{
		cdev_del(&(scull_devices[i].cdev));
	}
	
	//unregister device number
	unregister_chrdev_region(MKDEV(scull_major, scull_minor), scull_nr_devs);
}

void __init scull_init(void)
{
	int result = 0;
	
	result = scull_register_major_minor();
	
	if(result >= 0)
	{
		scull_dev_init();//register cdev inside
	}
	else
	{
		scull_cleanup();
	}
}

void __exit scull_exit(void)
{
	scull_cleanup();
}

module_init(scull_init);
module_exit(scull_exit);
