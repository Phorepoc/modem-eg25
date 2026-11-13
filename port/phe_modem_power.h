/* Copyright The Phorepoc Authors
 * SPDX-License-Identifier: EPL-1.0 */
/* The EG25-G power interface (phe_modem_power.c); callable from C++ kernel code. */
#ifndef PHE_MODEM_POWER_H
#define PHE_MODEM_POWER_H
#ifdef __cplusplus
extern "C" {
#endif
int phe_modem_power_on(void);		/* 0 = STATUS reached "on" (NuttX pinephone_modem_init) */
void phe_modem_power_off(void);		/* blocking: about 6.5 s */
#ifdef __cplusplus
bool phe_modem_status_on(void);
#else
_Bool phe_modem_status_on(void);
#endif
void phe_modem_uart_pins(void);
#ifdef __cplusplus
}
#endif
#endif
