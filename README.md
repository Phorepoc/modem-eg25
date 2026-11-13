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
| `third_party/openbsd` | `if_umb.c/.h`, `mbim.h` (MBIM data + control, ISC); `umsm.c` (USB serial, ISC) |
| `third_party/freebsd` | `u3g.c` (USB serial, BSD-2), `if_cdce.c/.h` (ECM/NCM fallback, BSD-4) |
| `third_party/zephyr` | `modem_cellular.c` + `cellular_quectel_eg2x_g.c` + bindings (power state machine, init/CMUX scripts, Apache-2.0) |
| `third_party/nuttx` | `pinephone_modem.c/.h` + `a64_pio.h` (PinePhone EG25-G power-on, Apache-2.0) |
| `port/` | glue that runs the imports on the Phorepoc kernel (`nuttx-shim/`, `phe_nuttx_port.cpp`, `build_arm.sh`) |
| `src/atcore/` | AT line framer / parsers used by the pheltsy AT back end (ours, from 3GPP 27.007/27.005) |
| `docs/design.md` | the layer plan |

The USB host controller (FreeBSD EHCI + Allwinner USB PHY) is board code and lives in
`adaptation-a64/os/third_party/freebsd-usb`.

## Status
`port/build_arm.sh` builds NuttX's power-on driver unmodified + the glue for ARMv7 (`phemodempower.o`); the
board GPIO/delay/log hooks are next. Nothing has run on hardware.
