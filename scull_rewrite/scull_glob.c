#include "scull_head.h"

//define quantum and qset
int glob_scull_quantum_num = SCULL_QUANTUM_NUM;
int glob_scull_qset_num = SCULL_QSET_NUM;

//major and minor number
#ifdef SCULL_MAJOR
int glob_scull_major = SCULL_MAJOR;
#else
int glob_scull_major = 0;
#endif
int glob_scull_minor = 0;

//define device number
unsigned int glob_scull_dev_num = 4;
struct scull_dev glob_scull_device_list[4];

struct file_operations glob_scull_fops = 
{
	.owner	= THIS_MODULE,//define in <linux/module.h>
	.llseek	= scull_llseek,
	.read	= scull_read,
	.write	= scull_write,
	.ioctl	= scull_ioctl,
	.open	= scull_open,
	.release= scull_release
};	
