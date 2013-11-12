#include "scull_head.h"

module_param(glob_scull_quantum_num, int, S_IRUGO);
module_param(glob_scull_qset_num, int, S_IRUGO);
module_param(glob_scull_major, int, S_IRUGO);

static int scull_register_major_minor(void)
{
	int result = -1;
	
	dev_t dev_major_minor;
	
	if(glob_scull_major)
	{//if user has set the major number
		dev_major_minor = MKDEV(glob_scull_major, glob_scull_minor);
		result = register_chrdev_region(dev_major_minor, glob_scull_dev_num, "scull");
	}
	else
	{//if user didn't set the major number, we will dynamic allocate one
		result = alloc_chrdev_region(&dev_major_minor, glob_scull_minor, glob_scull_dev_num, "scull");
		glob_scull_major = MAJOR(dev_major_minor);
	}

	if(result < 0)
	{
		printk(KERN_WARNING "hu: scull: can't get major %d\n", glob_scull_major);
	}
	
	return result;
}

static int scull_setup_cdev(struct scull_dev *psculldev, int index)
{
	int err;
	dev_t dev_num = MKDEV(glob_scull_major, glob_scull_minor + index);

	//create cdev
	cdev_init(&(psculldev->cdev), &glob_scull_fops);
//	(psculldev->cdev).owner = THIS_MODULE;
//	(psculldev->cdev).ops = &glob_scull_fops;//do we really need that? we already did it in cdev_init
	err = cdev_add(&(psculldev->cdev), dev_num, 1);

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
	
	for(i = 0; i < glob_scull_dev_num && result == 0; i++)
	{
		(glob_scull_device_list[i]).quantumNum = glob_scull_quantum_num;
		(glob_scull_device_list[i]).qsetNum = glob_scull_qset_num;
		init_MUTEX(&((glob_scull_device_list[i]).sem));
		result = scull_setup_cdev(&(glob_scull_device_list[i]), i);
	}
	
	return result;
}

struct scull_qset * scull_follow(struct scull_dev *psculldev, int item)
{

}

//to free all the memory in scull for quantum
int scull_trim(struct scull_dev *psculldev)
{
	struct scull_qset *pqsetNext, *pqsetCurrent, *pqsetTemp;
	int result = 0;
	
	for(pqsetCurrent = psculldev->pqset; pqsetCurrent; pqsetCurrent = pqsetNext)
	{
		if(pqsetCurrent->data)
		{
			kfree(pqsetCurrent->data);
			pqsetCurrent->data = NULL;
		}
		
		pqsetTemp = pqsetCurrent;
		pqsetNext = pqsetCurrent->pqsetNext;
		kfree(pqsetTemp);
		pqsetTemp = NULL;
	}

	return result;
}

static void scull_cleanup(void)
{
	int i;
	
	//unregister cdev
	for(i = 0; i < glob_scull_dev_num; i ++)
	{
		scull_trim(&(scull_devices[i]));
		cdev_del(&(scull_devices[i].cdev));
	}
	
	//unregister device number
	unregister_chrdev_region(MKDEV(glob_scull_major, glob_scull_minor), glob_scull_dev_num);
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
