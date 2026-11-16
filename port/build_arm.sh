#!/bin/sh
# SPDX-License-Identifier: EPL-1.0
# Compile check of the modem power code for the A64 kernel, in the kernel's NuttX environment
# (adaptation-a64/os/src/phorepoc-arm/a64/display/nuttx: its vendored a64_pio.c/.h + compat), with the same flags
# link_ekern.sh uses for NuttX C sources. From Windows (no mounts; both repos as siblings):
#   cd C:/Users/memph/Git/Phorepoc && tar c modem-eg25/third_party/nuttx modem-eg25/port adaptation-a64/os/src/phorepoc-arm/a64/display/nuttx |
#   MSYS_NO_PATHCONV=1 docker run --rm -i -v modem-wk:/wk --entrypoint sh ghcr.io/phorepoc/mesa:latest \
#     -c 'rm -rf /wk/src && mkdir -p /wk/src && cd /wk/src && tar x && sh modem-eg25/port/build_arm.sh'
set -e
T=${T:-/opt/arm-gnu/bin/arm-none-eabi-}
ME=${ME:-modem-eg25}; DN=${DN:-adaptation-a64/os/src/phorepoc-arm/a64/display/nuttx}
F="-c -O2 -march=armv5te -marm -mthumb-interwork -fno-short-enums -ffreestanding -fno-strict-aliasing -Wall
   -I$DN/compat -I$DN -I$ME/port -I$ME/port/kinc -I$ME/third_party/nuttx/boards/arm64/a64/pinephone/src -include $ME/port/phe_nuttx_kernel.h"
mkdir -p out
${T}gcc $F $ME/third_party/nuttx/boards/arm64/a64/pinephone/src/pinephone_modem.c -o out/nx_pinephone_modem.o
${T}gcc $F $ME/port/phe_modem_power.c -o out/phe_modem_power.o
${T}ld -r out/nx_pinephone_modem.o out/phe_modem_power.o -o out/phemodempower.o
echo "undefined (from the kernel):"; ${T}nm -u out/phemodempower.o
echo "defined:"; ${T}nm out/phemodempower.o | grep ' T '
