/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2015 Neil Horman <nhorman@tuxdriver.com>.
 * All rights reserved.
 */

#ifndef _RTE_COMPAT_H_
#define _RTE_COMPAT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ALLOW_EXPERIMENTAL_API
#if defined(__APPLE__)
#define __rte_experimental \
__attribute__((deprecated("Symbol is not yet part of stable ABI"), \
section("__TEXT,.experimental")))
#else
#define __rte_experimental \
__attribute__((deprecated("Symbol is not yet part of stable ABI"), \
section(".text.experimental")))
#endif
#else

#if defined(__APPLE__)
#define __rte_experimental \
__attribute__((section("__TEXT,.experimental")))
#else
#define __rte_experimental \
__attribute__((section(".text.experimental")))
#endif
#endif

#ifndef __has_attribute
/* if no has_attribute assume no support for attribute too */
#define __has_attribute(x) 0
#endif

#if !defined ALLOW_INTERNAL_API && __has_attribute(error) /* For GCC */

#if defined(__APPLE__)
#define __rte_internal \
__attribute__((error("Symbol is not public ABI"), \
section("__TEXT,.internal")))
#else
#define __rte_internal \
__attribute__((error("Symbol is not public ABI"), \
section(".text.internal")))
#endif
#elif !defined ALLOW_INTERNAL_API && __has_attribute(diagnose_if) /* For clang */

#define __rte_internal \
_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wgcc-compat\"") \
__attribute__((diagnose_if(1, "Symbol is not public ABI", "error"), \
section("__TEXT,.internal"))) \
_Pragma("GCC diagnostic pop")

#else

#if defined(__APPLE__)
#define __rte_internal \
__attribute__((section("__TEXT,.internal")))
#else
#define __rte_internal \
__attribute__((section(".text.internal")))
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* _RTE_COMPAT_H_ */
