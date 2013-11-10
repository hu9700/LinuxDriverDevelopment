#include "scull_head.h"

ssize_t scull_read (struct file * filp, char __user * buf, size_t count, loff_t * f_pos)
{
	struct scull_dev *dev = filp->private_data;//be saved here in open
	struct scull_qset *dptr;//the first list item
	int quantum = dev->quantum;
	int qset = dev->qset;
	int itemsize = quantum * qset;//how many bytes in the listitme
	int item, s_pos, q_pos, rest;
	ssize_t retval = 0;
	
	if(down_interruptible(&(dev->sem)) == 0)
	{		
		if(*f_pos < dev->size)
		{//if the file position is not out of file
			if(*f_pos + count > dev->size)
			{//if remained size of file is smaller than request size
				count = (dev->size) - (*f_pos);
			}
			
			//find list item, qset index and offset in the quantum
			item = ((long)(*f_pos)) / itemsize;
			rest = ((long)(*f_pos)) % itemsize;
			s_pos = rest / quantum;
			q_pos = rest % quantum;			
			
			//follow the list, up to the right position
			dptr = scull_follow(dev, item);
			
			if(dptr != NULL && dptr->data && dptr->data[s_pos])			
			{
				
			}
			else
			{//if dptr is null, or dptr->data, or dptr->data[s_pos]
				retval = -EFAULT;
			}
		}
		else
		{//if the file position is out of file
			retval = -EFAULT;
		}
	}
	else
	{//if block is interrupt or error
		retval = -ERESTARTSYS;
	}
}

ssize_t scull_write (struct file * filp, const char __user * buff, size_t count, loff_t * offset)
{
	return 0;
}

loff_t scull_llseek (struct file * filp, loff_t offset, int mode)
{
	return 0;
}

int scull_ioctl (struct inode * inodp, struct file * filp, unsigned int cmd, unsigned long arg)
{
	return 0;
}

int scull_open (struct inode * inodp, struct file * filp)
{
	struct scull_dev *dev;
	int result = 0;

	dev = container_of(inodp->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev;//save the dev in private

	//now trim to 0 the length of the device if open was write-only
	if((filp->f_flags & O_ACCMODE) == O_WRONLY)
	{
		result = scull_trim(dev);
	}	
	
	return result;
}

int scull_release (struct inode * inodp, struct file * filp)
{
	return 0;
}




