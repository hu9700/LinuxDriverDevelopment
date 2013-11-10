#include "scull_head.h"

//define quantum and qset
int scull_quantum = SCULL_QUANTUM;
int scull_qset = SCULL_QSET;

//major and minor number
#ifdef SCULL_MAJOR
int scull_major = SCULL_MAJOR;
#else
int scull_major = 0;
#endif
int scull_minor = 0;

//define device number
unsigned int scull_nr_devs = 4;
struct scull_dev scull_devices[4];

struct file_operations scull_fops = 
{
	.owner	= THIS_MODULE,//define in <linux/module.h>
	.llseek	= scull_llseek,
	.read	= scull_read,
	.write	= scull_write,
	.ioctl	= scull_ioctl,
	.open	= scull_open,
	.release= scull_release
};	
