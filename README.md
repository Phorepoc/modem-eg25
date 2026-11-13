# modem-eg25

Telephony adaptation for the Quectel EG25-G modem in the PinePhone: the pieces that turn the
modem's AT interface into the Symbian telephony stack (ETel -> CTSY -> licensee TSY).

Licence: EPL-1.0 (see `LICENSE`), the same as the Symbian platform it plugs into. Everything here
is written from public specifications (3GPP TS 27.007 / 27.005, the Quectel EG25-G AT command
manual, the USB 2.0 specification) and the EPL Symbian sources. No GPL or LGPL code is used, so
the result can ship inside the base image: telephony is a core function and must not depend on
an installable package.

## Layout

| Path | What |
|---|---|
| `src/atcore/` | Portable AT core: byte-stream line framer, result/URC classification, parsers for `+CREG/+CGREG/+CEREG`, `+CSQ`, `+CLCC`, `+CMTI`, `+CPIN`. Freestanding C++ (no library calls); shared by the TSY, the scripted AT peer for emulators, and the host tests. |
| `tests/` | Host tests (`sh tests/run.sh` with a Linux g++). |
| `docs/design.md` | The layer plan and what comes next. |

## Status

`atcore` builds (host g++, `arm-none-eabi-g++ -ffreestanding`) and its host test passes. The
licensee TSY (pheltsy) currently lives in the `core` repository under
`os/toolchain/shims/licenseetsy_board/`; it moves here and switches to `atcore` next.
