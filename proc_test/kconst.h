#ifndef INC_NAILS_KCONST_H
#define INC_NAILS_KCONST_H
/* $Id: */
/*!
 * Copyright (C) 2009, McAfee Inc. All rights reserved.
 *
 * $Revision: 6928 $
 *
 */


#define NAILS_COPYRIGHT_STRING "Copyright (C) 2003,2004 Networks Associates Technology Inc.\nAll rights reserved.\n"

#define DEFAULT_KAPIheartbeat		30

#define KERNEL_MODULE_VERSION	(VER_MAJOR(1) | VER_MINOR(0) | VER_PATCH(0))
#define DEVICE_NAME		"linuxshield"
#define DEFAULT_KERNEL_TIMEOUT	300
#define DEFAULT_KERNEL_SCANNER_WATCHDOG	(DEFAULT_KAPIheartbeat * 2)
#define DEFAULT_KERNEL_CLOSE_DELAY 1000

#define DEFAULT_KERNEL_SCAN_FAIL_DENY	    1
#define DEFAULT_KERNEL_SCAN_INTERRUPTIBLE   1

#define KCACHE_HASH_CHAIN_BITS	8	
#define KCACHE_HASH_CHAINS	(1 << KCACHE_HASH_CHAIN_BITS)
#define KCACHE_HASH_MASK	(KCACHE_HASH_CHAINS - 1)
#define KCACHE_DEFAULT_SIZE	(KCACHE_HASH_CHAINS << 8)
#define KCACHE_NODEV		0

#define KCACHE_FLUSH_CMD	"flush"
#define KCACHE_FLUSH_CMD_LEN	(sizeof(KCACHE_FLUSH_CMD) - 1)
#define KCACHE_SIZE_CMD		"size"
#define KCACHE_SIZE_CMD_LEN	(sizeof(KCACHE_SIZE_CMD) - 1)

#define SCAN_STATS_RESET_CMD	"reset"
#define SCAN_STATS_RESET_CMD_LEN (sizeof(SCAN_STATS_RESET_CMD) - 1)

#define PROCFS_ROOT		"proctest"
#define PROCFS_ROOT_LEN		(sizeof(PROCFS_ROOT) - 1)
#define PROCFS_AUTH_DEV_LINK	"..."

#define REQUEUE_SCAN_MAX	2

#endif /* INC_NAILS_KCONST_H */
