# modem-eg25

Telephony and mobile data for discrete cellular modems, first the Quectel EG25-G in the PinePhone (also
x86 boards with a USB modem). Shipped inside the base image.

## Licences
- Our own code (`src/`, `port/`, `tests/`, `docs/`): EPL-1.0 (`LICENSE`).
- `third_party/`: upstream files imported **unmodified** at pinned commits, each keeping its own licence
  header (ISC, BSD-2-Clause, BSD-4-Clause, Apache-2.0); see `NOTICE`, `third_party/SOURCES.lock` and
  `third_party/SHA256SUMS`; `sh third_party/check.sh` re-downloads and compares every file.
  No GPL or LGPL code.

## Layout
| Path | What |
|---|---|
| `third_party/openbsd` | `if_umb.c/.h`, `mbim.h` (MBIM data + control, ISC); `umsm.c` (USB serial, ISC); `if_urndis.c/reg.h` + `sys/dev/rndis.h` (RNDIS, second fallback, ISC) |
| `third_party/freebsd` | `u3g.c` (USB serial, BSD-2), `if_cdce.c/.h` (ECM/NCM, first fallback, BSD-4: credit line in `NOTICE`) |
| `third_party/zephyr` | `modem_cellular.c` + `cellular_quectel_eg2x_g.c` + bindings (power state machine, init/CMUX scripts, Apache-2.0) |
| `third_party/nuttx` | `pinephone_modem.c/.h` (PinePhone EG25-G power-on) + `a64_serial.c` (source of the UART3 pin lines), Apache-2.0 |
| `port/` | `phe_modem_power.c/.h` (the kernel's power interface: NuttX on, Zephyr off), `pinephone_modem_pins.h` (verbatim NuttX pin lines, Apache-2.0), `phe_nuttx_kernel.h` + `kinc/` (the A64 kernel's NuttX environment), `build_arm.sh` |
| `src/atcore/` | AT line framer / parsers used by the pheltsy AT back end (ours, from 3GPP 27.007/27.005) |
| `docs/design.md` | the layer plan |

The USB host controller (FreeBSD EHCI + Allwinner USB PHY) is board code and lives in
`adaptation-a64/os/third_party/freebsd-usb`.

## Status
`port/build_arm.sh` compiles NuttX's power-on driver (unmodified) + `phe_modem_power.c` for ARM in the A64 kernel's
NuttX environment (adaptation-a64 `display/nuttx`, whose `a64_pio.c` they call); adaptation-a64 `link_ekern.sh` links
them into `MODEM=1` kernels (`MODEM_EG25` = this checkout). Nothing has run on hardware.
