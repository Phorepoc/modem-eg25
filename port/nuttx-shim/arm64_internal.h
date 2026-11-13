/* Copyright The Phorepoc Authors
 * SPDX-License-Identifier: EPL-1.0 */
#ifndef PHE_NX_ARM64_INTERNAL_H
#define PHE_NX_ARM64_INTERNAL_H
/* up_mdelay(): busy-wait milliseconds; Symbian side = phe_nuttx_port.cpp (NKern::Sleep / Kern::NanoWait). */
#ifdef __cplusplus
extern "C" {
#endif
void up_mdelay(unsigned int milliseconds);
#ifdef __cplusplus
}
#endif
#endif
