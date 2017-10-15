#ifndef INC_LSPROCFS_H
#define INC_LSPROCFS_H
/* $Id: lsprocfs.h 3927 2004-04-20 20:37:22Z mhopkins $ */
/*!
 * \file
 *
 * \brief
 * Linux procfs helper functions
 *
 */
/*
 * Copyright (C) 2003 Networks Associates Technology Inc.
 * All rights reserved.
 *
 * $Revision: 3927 $
 *
 */


#ifndef __KERNEL__
#error This header file is for kernel use only
#endif

#include <linux/proc_fs.h>

#define PF_WRITE_LONG_BUFLEN 21 /* Enough to hold 2^64 plus a terminator */

int PF_init(struct proc_dir_entry *root);
void PF_term(void);
struct proc_dir_entry *PF_getDir(void);
void PF_createEntry(const char *name, void *data, const struct file_operations *fops);
void PF_createSymlink(const char *name, const char *dest);
void PF_removeEntry(const char *name);


#endif /* INC_LSPROCFS_H */
