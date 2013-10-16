//int (*open)(struct inode *inode, struct file *filp);
//What open will do
//1, check the specified error for device(such as whether device is ready, more those kind of hardware error)
//2, if it's the first time to open, initialize the device
//3, if it's necessarry, update f_op pointer
//4, alloc and fill the data need to put in filp->private_data
int scull_open(struc inode *inode, struct file* filp)
{
	struct scull_dev *dev;
	//the macro to get the container of one variable
	//container_of(pointer, container_type, containre_field), defined in <linux/kernel.h>
	//we don't want struct of cdev, we want the one containing cdev, struct scull_dev
	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	flip->private = dev;//save the dev in private

	//now trim to 0 the length of the device if open was write-only
	if((filp->f_flags & O_ACCMODE) == O_WRONLY)
	{
		scull_trim(dev);//ignore errors
		return 0;//success
	}
}

//do the oppsite as open, device_close
//1 free the stuff allocated by open in filp->private_data
//2 close the device (scull doesn't have hardware to shut down)
//not everytime call release, only when the counter of file get to 0, then release
int scull_release(struct inode *inode, struct file *filp)
{
	return 0;
}

//ssize_t read(struct file *flip, char __user *buff, size_t count, loff_t * offp);
//ssize_t write(struct file *flip, const char __user *buff, size_t count);
//return one "singed size type"
//count is the size of request data size
//buff is the data zone be writen to or read from
//offp is the file position user is reading

//because buff is the buffer from user space, we need to use below function, defined in <asm/uaccess.h>
//copy an array with some size
//unsigned long copy_to_user(void __user *to, const void *from, unsigned long count);
//unsigned long copy _from_user(void *to, const void __user *from, unsigned long count);
//be careful maybe the pointer in userspace is in virtual memory, and that will make process sleep
//this function will check whether the user space memory is validate, otherwise will not copy
//if meanwhile copy, the memory is invalidate, only copy some data, and return the copyed size

ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	struct scull_dev *dev = filp->private_data;//be saved here in open
	struct scull_qset *dptr;//the first list item
	int quantum = dev->quantum;
	int qset = dev->qset;
	int itemsize = quantum * qset;//how many bytes in the listitme
	int item, s_pos, q_pos, rest;
	ssize_t retval = 0;
	
	//if it's blocked
	if(down_interruptible(&dev->sem))
	{
		return -ERESTARTSYS;
	}
	
	//if the file position is out of file
	if(*f_pos >= dev->size)
	{
		goto out;
	}
	
	//if remained size of file is smaller than request size
	if(*f_pos + count > dev->size)
	{
		count = dev->size - *f_pos;
	}
	
	//find list item, qset index and offset in the quantum
	item = (long)*f_pos / itemsize;
	rest = (long)*f_pos % itemsize;
	s_pos = rest / quantum;
	q_pos = rest % quantum;
	
	//follow the list, up to the right position
	dptr = scull_follow(dev, item);
	if(dptr == NULL || !dptr->data || !dptr->data[s_pos])
	{//if the device point is null, or the first data point is null, or the data what to access is null
		goto out;//don't fill holes
	}
	
	//read only up to the end of this quantum
	if(count > quantum - q_pos)
	{
		count = quantum - q_pos;
	}
	
	if(copy_to_user(buf, dptr->data[s_pos] + q_pos, count))
	{
		retval = -EFAULT;
		goto out;
	}
	
	//we need to update f_pos
	*f_pos += count;
	retval = count;
	
ount:
	up(&dev->sem);
	return retval;
	
	
}
