#ifndef INC_KERNELDEP_H
#define INC_KERNELDEP_H
/* $Id: kernelDep.h 8231 2011-07-07 09:51:20Z jdivakar $ */
/*!
 * \file
 *
 * \brief
 * Macros to encapsulate some differences between 2.4 and 2.6 kernels
 */
/*
 * Copyright (C) 2011 McAfee, Inc. All rights reserved.
 * All rights reserved.
 *
 * $Revision: 8231 $
 *
 */

    #define INCREMENT_MOD_REF_COUNT (void)try_module_get(THIS_MODULE);
    #define DECREMENT_MOD_REF_COUNT module_put(THIS_MODULE);
    #define INCREMENT_OTHER_MOD_REF_COUNT(d) (void)try_module_get(d);
    #define DECREMENT_OTHER_MOD_REF_COUNT(d) module_put(d);
    #define FILE_LIST_LOCK spin_lock(files_lock_addr);
    #define FILE_LIST_UNLOCK spin_unlock(files_lock_addr);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20))
typedef struct kmem_cache kmem_cache_t;
#endif
#endif /* INC_KERNELDEP_H */
