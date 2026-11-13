/****************************************************************************
 * port/pinephone_modem_pins.h
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/* The EG25-G pin definitions, copied verbatim from Apache NuttX (pin
 * b66e7a65da9cf8609d62475785acdd793be34691):
 *   boards/arm64/a64/pinephone/src/pinephone_modem.c (the MODEM_* pins,
 *     which are file-local there) and
 *   arch/arm64/src/a64/a64_serial.c (the UART3 pins).
 * Needed by the Phorepoc power-off path and the UART3 set-up, which the
 * NuttX file does not export. Both originals are in third_party/nuttx.
 */

#ifndef __PHE_PINEPHONE_MODEM_PINS_H
#define __PHE_PINEPHONE_MODEM_PINS_H

#include "a64_pio.h"

/* Shortcut for Output Pin */

#define OUTPUT_PIN (PIO_OUTPUT | PIO_PULL_NONE | PIO_DRIVE_MEDLOW | \
                    PIO_INT_NONE | PIO_OUTPUT_SET)

/* LTE Modem Status at PH9 (Input) */

#define MODEM_STATUS    (PIO_INPUT | PIO_PORT_PIOH | PIO_PIN9)

/* LTE Modem Power at PL7 (Output) */

#define MODEM_PWR_BAT   (OUTPUT_PIN | PIO_PORT_PIOL | PIO_PIN7)

/* LTE Modem Reset at PC4 (Output, High-Low Inverted) */

#define MODEM_RESET_N   (OUTPUT_PIN | PIO_PORT_PIOC | PIO_PIN4)

/* LTE Modem AP Ready at PH7 (Output) */

#define MODEM_AP_READY  (OUTPUT_PIN | PIO_PORT_PIOH | PIO_PIN7)

/* LTE Modem DTR at PB2 (Output) */

#define MODEM_DTR       (OUTPUT_PIN | PIO_PORT_PIOB | PIO_PIN2)

/* LTE Modem Power Key at PB3 (Output, High-Low Inverted) */

#define MODEM_PWRKEY    (OUTPUT_PIN | PIO_PORT_PIOB | PIO_PIN3)

/* LTE Modem Wireless Disable at PH8 (Output) */

#define MODEM_W_DISABLE (OUTPUT_PIN | PIO_PORT_PIOH | PIO_PIN8)

/* UART3: PD0 and PD1 (Peripheral 3) */

#  define PIO_UART3_TX (PIO_PERIPH3 | PIO_PORT_PIOD | PIO_PIN0)
#  define PIO_UART3_RX (PIO_PERIPH3 | PIO_PORT_PIOD | PIO_PIN1)

#endif /* __PHE_PINEPHONE_MODEM_PINS_H */
