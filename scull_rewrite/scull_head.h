#ifndef _SCULL_HEAD_H_
#define _SCULL_HEAD_H_
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>//include some file for character device
#include <linux/semaphore.h>//to enable Semaphore
#include <linux/types.h>//dev_t inside
#include <linux/fs.h>//for file operation
#include "scull.h"
#include "scull_glob.h"
#include "scull_fops.h"
#endif
