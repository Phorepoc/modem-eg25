/* Copyright The Phorepoc Authors
 * SPDX-License-Identifier: EPL-1.0 */

/* phe_modem_power.c -- the EG25-G power interface the Phorepoc kernel calls (a64uart.cpp, MODEM=1).
 * Built in the NuttX environment next to Apache NuttX's pinephone_modem.c (unmodified, third_party/nuttx):
 *   on     = pinephone_modem_init() (NuttX)
 *   off    = the Zephyr modem_cellular power-off sequence with the EG2x-G vendor timing
 *            (third_party/zephyr .../cellular_quectel_eg2x_g.c: power_pulse_duration_ms 1500,
 *            shutdown_time_ms 5000), then PWR_BAT off = the inverse of NuttX's first step
 *   status = MODEM_STATUS low = on (NuttX pinephone_modem_wait)
 *   uart   = the UART3 pins (NuttX a64_serial.c)
 * Pins: pinephone_modem_pins.h (verbatim NuttX copies). */
#include <stdbool.h>
#include <stdint.h>
#include "pinephone_modem.h"
#include "pinephone_modem_pins.h"
#include "phe_nuttx_kernel.h"

#define ZEPHYR_EG2X_POWER_PULSE_MS 1500
#define ZEPHYR_EG2X_SHUTDOWN_MS    5000

int phe_modem_power_on(void)
{
  return pinephone_modem_init();
}

void phe_modem_power_off(void)
{
  a64_pio_write(MODEM_PWRKEY, true);
  up_mdelay(ZEPHYR_EG2X_POWER_PULSE_MS);
  a64_pio_write(MODEM_PWRKEY, false);
  up_mdelay(ZEPHYR_EG2X_SHUTDOWN_MS);
  a64_pio_write(MODEM_PWR_BAT, false);
}

bool phe_modem_status_on(void)
{
  return !a64_pio_read(MODEM_STATUS);
}

void phe_modem_uart_pins(void)
{
  a64_pio_config(PIO_UART3_TX);
  a64_pio_config(PIO_UART3_RX);
}
