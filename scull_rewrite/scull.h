#ifndef _SCULL_H_
#define _SCULL_H_

#include "scull_head.h"

#define SCULL_QUANTUM 4000
#define SCULL_QSET 1000

//quantum and qset
struct scull_qset
{
	void **data;
	struct scull_qset *next;
};

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
};

int scull_trim(struct scull_dev *dev);

#endif
