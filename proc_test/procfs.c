/* $Id: procfs.c 7008 2010-01-07 10:22:30Z rkakara $ */
/*
 * Copyright (C) 2009 McAfee, Inc.  All rights reserved.
 *
 * $Revision: 7008 $
 *
 */

#define __NO_VERSION__
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18))
#include <linux/config.h>
#endif
#include <asm/uaccess.h>

#include <linux/fs.h>
#include <linux/seq_file.h>
#include "kernelDep.h"

//#include <nails/assert.h>
#include "kconst.h"
#include "lsprocfs.h"

#ifndef CONFIG_PROC_FS
#error CONFIG_PROC_FS must be defined
#endif

/* ========================= Private definitions ========================== */

/* =========================== Local prototypes =========================== */

static ssize_t PF_readLong(struct file *file, char __user *buffer, size_t count, loff_t *off);
static ssize_t PF_writeLong(struct file *file, const char __user *buffer, size_t count, loff_t *off);

/* ============================== Local Data ============================== */
static struct proc_dir_entry *linuxshieldProcDir;

/* ============================= Global Data ============================== */

/* ================================= Code ================================= */


/*!
 * \brief Initialise Linuxshield procfs subsystem
 *
 * \param dir "root" directory under /proc for this module. If NULL
 * 	  create a /proc/PROCFS_ROOT directory.
 *
 * \return zero on sucess, negative of failure.
 *
 */
int
PF_init(struct proc_dir_entry *dir)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30))
    if (dir)
    {
	INCREMENT_OTHER_MOD_REF_COUNT(dir->owner);
	linuxshieldProcDir = dir;
#ifdef PROCFS_DEBUG
	printk(KERN_WARNING "Using existing procfs directory %p owned by %s\n",
	       linuxshieldProcDir, linuxshieldProcDir->owner->name);
#endif
    }
    else
    {
	linuxshieldProcDir = proc_mkdir(PROCFS_ROOT, NULL);
	if (linuxshieldProcDir)
	{
	    linuxshieldProcDir->owner = THIS_MODULE;
#ifdef PROCFS_DEBUG
	    printk(KERN_WARNING "Created new procfs directory %p owned by %s\n",
		   linuxshieldProcDir, linuxshieldProcDir->owner->name);
#endif
	}
    }
#else
    if (dir)
    {
	linuxshieldProcDir = dir;
    }
    else
    {
	linuxshieldProcDir = proc_mkdir(PROCFS_ROOT, NULL);
    }
#endif
    if (linuxshieldProcDir)
	return 0;
    return -1;
}

static ssize_t PF_readLong(struct file *file, char __user *buffer, size_t count, loff_t *off)
{
	static int finished = 0;
	char buff[PF_WRITE_LONG_BUFLEN];
	int len;
        void *data = PDE_DATA(file_inode(file));

	printk(KERN_INFO "Before snprintf\n");
	len = snprintf(buff, PF_WRITE_LONG_BUFLEN, "%ld\n", *(long *)data);
	//len = snprintf(buff, PF_WRITE_LONG_BUFLEN, "%p\n", (char*)data);

	printk(KERN_INFO "After snprintf\n");

	/* 
	 * We return 0 to indicate end of file, that we have
	 * no more information. Otherwise, processes will
	 * continue to read from us in an endless loop. 
	 */
	if ( finished ) {
		printk(KERN_INFO "procfs_read: END\n");
		finished = 0;
		return 0;
	}
	
	finished = 1;

	if ( copy_to_user(buffer, buff, len) ) {
		return -EFAULT;
	}

	printk(KERN_INFO "procfs_read: read %d bytes\n", len);

	return len;	// Number of bytes 'read'
}

static ssize_t PF_writeLong(struct file *file, const char __user *buffer, size_t count, loff_t *off)
{
	char *endp = NULL; //output param
	long val;
	char buff[PF_WRITE_LONG_BUFLEN];
        void *data = PDE_DATA(file_inode(file));

	if (count > sizeof(buff) - 1)
		return -EINVAL;

	if (copy_from_user(buff, buffer, count))
		return -EINVAL;

	buff[count] = '\0';
	val = simple_strtol(buff, &endp, 10);

	*(long *) data = val;
		printk(KERN_INFO "Received data:%ld\n", val);

	return count;
}

static const struct file_operations lsh_proc_fops = {
        .owner          = THIS_MODULE,
        .read           = PF_readLong,
        .write          = PF_writeLong,
};

/* TODO: doxygen me */
void
PF_term(void)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30))
    if (linuxshieldProcDir)
    {
	if (linuxshieldProcDir->owner == THIS_MODULE)
        {
	        remove_proc_entry(PROCFS_ROOT, NULL);
            //PF_getFreeAllFopsEntries();
        }

	else
	    DECREMENT_OTHER_MOD_REF_COUNT(linuxshieldProcDir->owner);
    }
#endif
    remove_proc_entry(PROCFS_ROOT, NULL);
    return;
}

/* TODO: doxygen me */
struct proc_dir_entry * PF_getDir(void)
{
    return linuxshieldProcDir;
}

/* TODO: doxygen me */
void PF_createEntry(const char *name, void *data, const struct file_operations *fops)
{
	mode_t mode;
	struct proc_dir_entry *pde;
	mode = S_IFREG;

	if(fops == NULL)
		fops = &lsh_proc_fops;

	if (fops->read)
		mode |= S_IRUSR;
	if (fops->write)
		mode |= S_IWUSR;

	pde = proc_create_data(name, mode, linuxshieldProcDir, fops, data);
	if(pde != NULL)
    {
		printk(KERN_INFO "Created pde:%p; Value : %ld\n", data, *(long*)(data));
    }
}

/* TODO: doxygen me */
void PF_createSymlink(const char *name, const char *dest)
{
    struct proc_dir_entry *ent;

    ent = proc_symlink(name, linuxshieldProcDir, dest);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30))
    if (ent)
    {
	ent->owner = THIS_MODULE;
    }
#endif
}

/* TODO: doxygen me */
void PF_removeEntry(const char *name)
{
    if (linuxshieldProcDir)
    {
	remove_proc_entry(name, linuxshieldProcDir);
    }
}

