/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2019 Intel Corporation
 */

#ifndef _RTE_OS_H_
#define _RTE_OS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This header should contain any definition
 * which is not supported natively or named differently in FreeBSD.
 */

#include <pthread.h>
#include <mach/thread_policy.h>
#include <mach/thread_act.h>
#include <sys/sysctl.h>
#include <sys/queue.h>
#include <sched.h>

/* These macros are compatible with system's sys/queue.h. */
#define RTE_TAILQ_HEAD(name, type) TAILQ_HEAD(name, type)
#define RTE_TAILQ_ENTRY(type) TAILQ_ENTRY(type)
#define RTE_TAILQ_FOREACH(var, head, field) TAILQ_FOREACH(var, head, field)
#define RTE_TAILQ_FIRST(head) TAILQ_FIRST(head)
#define RTE_TAILQ_NEXT(elem, field) TAILQ_NEXT(elem, field)
#define RTE_STAILQ_HEAD(name, type) STAILQ_HEAD(name, type)
#define RTE_STAILQ_ENTRY(type) STAILQ_ENTRY(type)

#ifndef CPU_SETSIZE
#define CPU_SETSIZE RTE_MAX_LCORE
#endif
#define SYSCTL_CORE_COUNT   "machdep.cpu.core_count"

typedef struct cpu_set {
  uint32_t count;
} cpu_set_t;
typedef cpu_set_t rte_cpuset_t;

static inline void
CPU_ZERO(cpu_set_t *cs)
{
	cs->count = 0;
}

static inline void
CPU_SET(int num, cpu_set_t *cs)
{
	cs->count |= (1 << num);
}

static inline int
CPU_ISSET(int num, const cpu_set_t *cs)
{
	return (cs->count & (1 << num));
}

static inline int
count_cpu(const rte_cpuset_t *s)
{
	unsigned int _i;
	int count = 0;

	for (_i = 0; _i < CPU_SETSIZE; _i++)
		if (CPU_ISSET(_i, s) != 0LL)
			count++;
	return count;
}
#define CPU_COUNT(s) count_cpu(s)

static inline int pthread_setaffinity_np(pthread_t thread, size_t cpu_size,
                           const cpu_set_t *cpu_set)
{
	mach_port_t mach_thread;
	size_t core = 0;

	for (core = 0; core < 8 * cpu_size; core++) {
		if (CPU_ISSET(core, cpu_set))
			break;
	}
	thread_affinity_policy_data_t policy = { core };
	mach_thread = pthread_mach_thread_np(thread);
	thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY,
				(thread_policy_t)&policy, 1);
	return 0;
}

static inline int pthread_getaffinity_np(pthread_t threadid, size_t cpuset_size,
			rte_cpuset_t *cpuset)
{
	size_t core = 0;
	mach_port_t mach_thread;
	mach_msg_type_number_t num;
	boolean_t get_default = FALSE;

	thread_affinity_policy_data_t policy;
	mach_thread = pthread_mach_thread_np(threadid);
	thread_policy_get(mach_thread, THREAD_AFFINITY_POLICY,
				(thread_policy_t)&policy, &num, &get_default);
	for (core = 0; core < 8 * cpuset_size; core++) {
		CPU_SET(core, cpuset);
	}
	return 0;
}

#define RTE_HAS_CPUSET

#define CPU_AND(dst, src1, src2) \
do { \
	(dst)->count = (src1)->count & (src2)->count; \
} while (0)

#define CPU_OR(dst, src1, src2) \
do { \
	(dst)->count = (src1)->count | (src2)->count; \
} while (0)

#define CPU_FILL(s) \
do { \
	(s)->count = -1; \
} while (0)

#define CPU_NOT(dst, src) \
do { \
	(dst)->count = (src)->count ^ -1LL; \
} while (0)

#define RTE_CPU_AND CPU_AND
#define RTE_CPU_OR CPU_OR
#define RTE_CPU_FILL CPU_FILL
#define RTE_CPU_NOT CPU_NOT

#ifdef __cplusplus
}
#endif

#endif /* _RTE_OS_H_ */
