/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <errno.h>

#include <rte_common.h>
#include <rte_log.h>
#include <rte_cycles.h>
#include <rte_memory.h>
#include <rte_eal.h>
#include <rte_debug.h>

#include "eal_private.h"
#include "eal_internal_cfg.h"

#ifdef RTE_LIBEAL_USE_HPET
#warning HPET is not supported in FreeBSD
#endif

enum timer_source eal_timer_source = EAL_TIMER_TSC;

uint64_t
get_tsc_freq(void)
{
	return 0;
}

int
rte_eal_timer_init(void)
{
	set_tsc_freq();
	return 0;
}
