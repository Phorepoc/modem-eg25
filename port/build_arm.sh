#!/bin/sh
# Build the modem power driver (NuttX pinephone_modem.c, unmodified) + its port glue for ARMv7 (Symbian A64 kernel ABI).
# From Windows: tar c third_party/nuttx port | MSYS_NO_PATHCONV=1 docker run --rm -i -v modem-wk:/wk --entrypoint sh \
#   ghcr.io/phorepoc/mesa:latest -c 'rm -rf /wk/src && mkdir -p /wk/src && cd /wk/src && tar x && sh port/build_arm.sh'
set -e
T=${T:-/opt/arm-gnu/bin/arm-none-eabi-}
N=third_party/nuttx
F="-Os -Wall -ffreestanding -march=armv7-a -marm -mfloat-abi=soft -fno-short-enums -Iport/nuttx-shim -I$N/arch/arm64/src/a64 -I$N/boards/arm64/a64/pinephone/src"
mkdir -p out
${T}gcc -std=gnu11 $F -c $N/boards/arm64/a64/pinephone/src/pinephone_modem.c -o out/pinephone_modem.o
${T}g++ -std=gnu++11 -fno-exceptions -fno-rtti $F -c port/phe_nuttx_port.cpp -o out/phe_nuttx_port.o
${T}ld -r out/pinephone_modem.o out/phe_nuttx_port.o -o out/phemodempower.o
echo "undefined (the board hooks):"; ${T}nm -u out/phemodempower.o
echo "defined:"; ${T}nm out/phemodempower.o | grep ' T '
