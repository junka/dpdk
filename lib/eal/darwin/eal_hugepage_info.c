/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/mman.h>
#include <string.h>

#include <rte_log.h>
#include <fcntl.h>

#include "eal_private.h"
#include "eal_hugepages.h"
#include "eal_internal_cfg.h"
#include "eal_filesystem.h"

/*
 * Uses mmap to create a shared memory area for storage of data
 * Used in this file to store the hugepage file map on disk
 */
static void *
map_shared_memory(const char *filename, const size_t mem_size, int flags)
{
	void *retval;
	int fd = open(filename, flags, 0600);
	if (fd < 0)
		return NULL;
	if (ftruncate(fd, mem_size) < 0) {
		close(fd);
		return NULL;
	}
	retval = mmap(NULL, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);
	return retval == MAP_FAILED ? NULL : retval;
}

static void *
open_shared_memory(const char *filename, const size_t mem_size)
{
	return map_shared_memory(filename, mem_size, O_RDWR);
}

/*
 * No hugepage support on Darwin
 */
int
eal_hugepage_info_init(void)
{
	return -1;
}

/* copy stuff from shared info into internal config */
int
eal_hugepage_info_read(void)
{
	struct internal_config *internal_conf =
		eal_get_internal_configuration();

	struct hugepage_info *hpi = &internal_conf->hugepage_info[0];
	struct hugepage_info *tmp_hpi;

	internal_conf->num_hugepage_sizes = 1;

	tmp_hpi = open_shared_memory(eal_hugepage_info_path(),
				  sizeof(internal_conf->hugepage_info));
	if (tmp_hpi == NULL) {
		RTE_LOG(ERR, EAL, "Failed to open shared memory!\n");
		return -1;
	}

	memcpy(hpi, tmp_hpi, sizeof(internal_conf->hugepage_info));

	if (munmap(tmp_hpi, sizeof(internal_conf->hugepage_info)) < 0) {
		RTE_LOG(ERR, EAL, "Failed to unmap shared memory!\n");
		return -1;
	}
	return 0;
}
