/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/queue.h>

#include <rte_debug.h>
#include <rte_atomic.h>
#include <rte_launch.h>
#include <rte_log.h>
#include <rte_memory.h>
#include <rte_per_lcore.h>
#include <rte_eal.h>
#include <rte_lcore.h>

#include "eal_private.h"
#include "eal_thread.h"

/* require calling thread tid by gettid() */
int rte_sys_gettid(void)
{
	unsigned long tid;
	pthread_threadid_np(NULL, &tid);
	return (int)tid;
}

void rte_thread_set_name(rte_thread_t thread_id, const char *thread_name)
{
	char truncated[RTE_MAX_THREAD_NAME_LEN];
	const size_t truncatedsz = sizeof(truncated);

	if (strlcpy(truncated, thread_name, truncatedsz) >= truncatedsz)
		RTE_LOG(DEBUG, EAL, "Truncated thread name\n");

	pthread_setname_np(truncated);
}

int rte_thread_setname(pthread_t id, const char *name)
{
	/* this BSD function returns no error */
	pthread_setname_np(name);
	return 0;
}
