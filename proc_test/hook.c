
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include "lsprocfs.h"


static long releaseOnFailedOpen = 1L;
static long debug = 0L;
static long enabled = 1L; 

static int __init initHook(void)
{
	printk(KERN_INFO "Loading proc test module...\n");  

	if (PF_init(NULL) < 0)
		goto cleanup;

	PF_createEntry("hook-debug", &debug, NULL);
	PF_createEntry("hook-enabled", &enabled, NULL);
	//PF_createEntry("hook-releaseOnFailedOpen", &releaseOnFailedOpen, NULL);

	printk(KERN_INFO "...SUCCESS!\n");

	goto exit;

cleanup:
	printk(KERN_INFO "...FAILED\n");
exit:
    return 0;
}

static void __exit termHook(void)
{
    //PF_removeEntry("hook-releaseOnFailedOpen");
    printk(KERN_INFO "Removing hook module...\n");
    PF_removeEntry("hook-enabled");
    PF_removeEntry("hook-debug");
    PF_term();
}


module_init(initHook)
module_exit(termHook)

MODULE_AUTHOR("Binoy");
MODULE_DESCRIPTION("Proc fs test module");
MODULE_LICENSE("GPL");

