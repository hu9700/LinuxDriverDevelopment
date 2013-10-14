#include <linux/init.h>
#include <linux/module.h>

static char * whom = "world";
static int howmany = 1;
//receive parameter from insmod or modprobe
//defined in moduleparam.h
//parameter type can be bool, invbool, charp, int, long, short, uint, ulong, ushort
//can be one array module_param_array(name , type, num, perm); it can deny the array is bigger than num
//perm defined in <linux/stat.h>, it indicate the permission in sysfs, /sys/module
module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);


//(__initdata) indecate the data is used for init
//after init, the data will be free

//in some module use __devinit and __devinitdata
//as __init and __initdata in hotplay device

//=========this two function will be export to outside of module
void hello_say(void)
{
	printk(KERN_ALERT "Hello!\n");
}

void hello_show(void)
{
	printk(KERN_ALERT "Waving Hand\n");
}

static int __init hello_init(void)
{
	int counter = 0;
	
	for(counter = 0; counter < howmany; counter ++)
	{
		printk(KERN_ALERT "Hello, %s\n", whom);
	}
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, %s\n", whom);
}
//module init and exit function register
module_init(hello_init);
module_exit(hello_exit);
//Export symbol for other module
EXPORT_SYMBOL(hello_say);
EXPORT_SYMBOL_GPL(hello_show);//only for GPL licence module

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("hu");
MODULE_DESCRIPTION("It's a Hello World demo for module");
MODULE_VERSION("0:0.1-test");//The template is in <linux/module.h>
MODULE_ALIAS("Hello World");
//MODULE_DEVICE_TABLE();
