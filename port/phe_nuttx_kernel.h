/* Copyright The Phorepoc Authors
 * SPDX-License-Identifier: EPL-1.0 */
/* Forced include for NuttX's pinephone_modem.c inside the A64 kernel's NuttX environment
 * (adaptation-a64 display/nuttx/compat): its _err level goes to the kernel log. */
#ifndef PHE_NUTTX_KERNEL_H
#define PHE_NUTTX_KERNEL_H
#include <nuttx/config.h>
#define _err(...) ph_log(__VA_ARGS__)
#define _info(...) ((void)0)
/* The modem's waits are long (PWRKEY 600 ms, STATUS up to 10 s, power-off 6.5 s) and run on the modem's own DFC
 * thread: they sleep (NKern::Sleep, a64uart.cpp) instead of the display environment's busy-wait ph_mdelay. */
#undef up_mdelay
void phe_modem_sleep_ms(unsigned ms);
#define up_mdelay(ms) phe_modem_sleep_ms(ms)
#endif
