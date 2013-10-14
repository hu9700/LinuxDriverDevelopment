#include <linux/init.h>
#include <linux/module.h>
/*global variable "current" is point to current process,
	with a type of task_struct defined in <linux/sched.h>, 
	it's defined in <asm/current.h>*/
static void checkCurrentProcess_do(void);

static int checkCurrentProcess_init(void)
{
	printk(KERN_ALERT "Check the current Process\n");
	checkCurrentProcess_do();
	return 0;
}

static void checkCurrentProcess_do(void)
{
	printk(KERN_INFO "The process is \"%s\" (pid %i)",current->comm, current->pid);
}

static void checkCurrentProcess_exit(void)
{
	printk(KERN_ALERT "exit checking process\n");
}

module_init(checkCurrentProcess_init);
module_exit(checkCurrentProcess_exit);


