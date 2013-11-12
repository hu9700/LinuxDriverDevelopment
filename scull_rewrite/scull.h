#ifndef _SCULL_H_
#define _SCULL_H_

#include "scull_head.h"

#define SCULL_QUANTUM_NUM 4000
#define SCULL_QSET_NUM 1000

//quantum and qset
struct scull_qset
{
	unsigned char * data;
	struct scull_qset *pqsetNext;
};

//define struct scull_dev, every scull_dev indicate one device
struct scull_dev
{
	struct scull_qset *pqset;		//point to the first quantum set
	int quantumNum;					//then current quantum size
	int qsetNum;					//the current array size
	unsigned long size;			//amount of data stored here
	unsigned int countUsed;		//used by sculluid and scullpriv
	struct semaphore sem;		//mutual exclusion semaphore
	struct cdev cdev;			//char device structure
};

int scull_trim(struct scull_dev *dev);

#endif
