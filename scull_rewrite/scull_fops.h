#ifndef _SCULL_FOPS_H_
#define _SCULL_FOPS_H_

#include "scull_head.h"

loff_t scull_llseek (struct file *, loff_t, int);
ssize_t scull_read (struct file *, char __user *, size_t, loff_t *);
ssize_t scull_write (struct file *, const char __user *, size_t, loff_t *);
//unsigned int scull_poll (struct file *, struct poll_table_struct *);
int scull_ioctl (struct inode *, struct file *, unsigned int, unsigned long);
int scull_open (struct inode *, struct file *);
int scull_release (struct inode *, struct file *);
//int scull_fsync (struct file *, struct dentry *, int);
//int scull_fasync (int, struct file *, int);
//ssize_t scull_readv (struct file *, const struct iovec *, unsigned long, loff_t *);
//ssize_t scull_writev (struct file *, const struct iovec *, unsigned long, loff_t *);


#endif
