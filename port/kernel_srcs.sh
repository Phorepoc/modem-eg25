# SPDX-License-Identifier: EPL-1.0
# kernel_srcs.sh -- sourced by the SoC kernel link (adaptation-a64 link_ekern.sh, MODEM=1) with ME = this checkout.
# Sets the C sources of the kernel-side modem power code and their extra include directories, so the SoC
# repository names no board: NuttX's PinePhone modem power driver (unmodified) + our power hook.
MODEM_KSRC="$ME/third_party/nuttx/boards/arm64/a64/pinephone/src/pinephone_modem.c $ME/port/phe_modem_power.c"
MODEM_KINC="$ME/port $ME/port/kinc $ME/third_party/nuttx/boards/arm64/a64/pinephone/src"
