/* Copyright The Phorepoc Authors
 * SPDX-License-Identifier: EPL-1.0 */
#ifndef PHE_NX_DEBUG_H
#define PHE_NX_DEBUG_H
/* NuttX _err/_info -> the kernel log (Kern::Printf) via phe_nuttx_log. */
#ifdef __cplusplus
extern "C" {
#endif
void phe_nuttx_log(const char* fmt, ...);
#ifdef __cplusplus
}
#endif
#define _err(...) phe_nuttx_log(__VA_ARGS__)
#define _info(...) phe_nuttx_log(__VA_ARGS__)
#endif
