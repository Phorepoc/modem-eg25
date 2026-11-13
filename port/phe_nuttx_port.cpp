/* Copyright The Phorepoc Authors
 * SPDX-License-Identifier: EPL-1.0 */

// phe_nuttx_port.cpp -- runs NuttX's pinephone_modem.c (Apache-2.0, unmodified, third_party/nuttx) on the
// Phorepoc A64 kernel. NuttX's pin encoding (a64_pio.h: port in bits 5-8, pin in bits 0-4, mode in the
// PIO_MODE field, initial level in PIO_OUTPUT_SET) is decoded here and handed to the board's GPIO hooks,
// which adaptation-a64 provides (the same PIO block the existing kernel drivers use).
// up_mdelay and the log map to the kernel services through the two hooks below as well, so this file
// builds without Symbian headers and the hooks are the whole porting surface.

extern "C" {
#include "a64_pio.h"
}

// --- hooks provided by the board (adaptation-a64) ---
extern "C" void PheBoardGpioConfig(unsigned aPort, unsigned aPin, bool aOutput, bool aInitialHigh);
extern "C" void PheBoardGpioWrite(unsigned aPort, unsigned aPin, bool aHigh);
extern "C" bool PheBoardGpioRead(unsigned aPort, unsigned aPin);
extern "C" void PheBoardDelayMs(unsigned aMs);			// NKern::Sleep in thread context
extern "C" void PheBoardLog(const char* aText);			// Kern::Printf("%s")

namespace {
inline unsigned Port(pio_pinset_t s) { return (s & PIO_PORT_MASK) >> PIO_PORT_SHIFT; }
inline unsigned Pin(pio_pinset_t s) { return (s & PIO_PIN_MASK) >> PIO_PIN_SHIFT; }
}

extern "C" int a64_pio_config(pio_pinset_t aSet)
	{
	unsigned mode = aSet & PIO_MODE_MASK;
	if (mode != PIO_INPUT && mode != PIO_OUTPUT)
		return -22;						// -EINVAL: the modem driver uses plain GPIO only
	PheBoardGpioConfig(Port(aSet), Pin(aSet), mode == PIO_OUTPUT, (aSet & PIO_OUTPUT_SET) != 0);
	return 0;
	}

extern "C" void a64_pio_write(pio_pinset_t aSet, bool aValue) { PheBoardGpioWrite(Port(aSet), Pin(aSet), aValue); }
extern "C" bool a64_pio_read(pio_pinset_t aSet) { return PheBoardGpioRead(Port(aSet), Pin(aSet)); }
extern "C" void up_mdelay(unsigned int aMs) { PheBoardDelayMs(aMs); }

extern "C" void phe_nuttx_log(const char* aFmt, ...)
	{
	// The driver's messages are fixed strings with at most one %d; the format text alone is enough
	// to tell which step failed, so it is logged as-is (no vsnprintf in the kernel side).
	PheBoardLog(aFmt);
	}
