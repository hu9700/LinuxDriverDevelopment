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
}
