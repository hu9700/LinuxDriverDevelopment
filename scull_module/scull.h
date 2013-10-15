#ifndef _SCULL_H_
#define _SCULL_H_

//you can change the size of qset and quantum
//by changing following define
//or give parameter while insmod, scull_quantum and scull_qset
//or use ioctl change it while running
#define SCULL_QUANTUM 4000
#define SCULL_QSET 1000


//now, each quantum gets maxium 4000byte size, and each qset got maxium 1000 quantums

//in scull, each device is one element in link list.
//every device point to one scull_dev
//every scull_dev got maxium 4MB memory

struct scull_qset
{
	void **data;
	struct scull_qset *next;
}

//define struct scull_dev, every scull_dev indicate one device
struct scull_dev
{
	struct scull_qset *data;		//point to the first quantum set
	int quantum;					//then current quantum size
	int qset;					//the current array size
	unsigned long size;			//amount of data stored here
	unsigned int accesskey;		//used by sculluid and scullpriv
	struct semaphore sem;		//mutual exclusion semaphore
	struct cdev cdev;			//char device structure
}


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

#endif
