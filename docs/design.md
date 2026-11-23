# Design (user decision 2026-11-13: all permissive, ported as-is, in the image)

```
 Phone / Messaging / connection manager            (platform, EPL, unchanged)
   ETel -> CTSY (classic licensee interface) -> pheltsy (Phorepoc's, EPL)
        back-end interface (MPheModemBackend): power, SIM, registration, calls, SMS, packet contexts
          back end 1: AT + MBIM   -- AT over a USB serial port (u3g/umsm) or UART3 (fallback);
                                     MBIM control over the umb device for data sessions
          back end 2 (future):    libhybris + an Android vendor radio HAL (Qualcomm-class phones)
   ESOCK/TCPIP -> NIF (from the 2010 raw-IP NIF, EPL) -> umb (MBIM, primary) | cdce (ECM/NCM, fallback 1) | urndis (RNDIS, fallback 2)
 Kernel: USB host = FreeBSD EHCI + aw_usbphy (adaptation-a64); class drivers umb / cdce / u3g|umsm run on a
         small FreeBSD-USB compatibility layer (usb_request/usbd_transfer onto Phorepoc's HCD).
 Power: NuttX pinephone_modem.c (on) + the Zephyr modem_cellular state machine (off / reset / wake).
 Call audio: modem PCM -> A64 codec AIF2 (FreeBSD's sun8i/a33 codec has AIF1 only: open, see modem-port.md).
```
Order: power (NuttX) on the board hooks -> USB host (EHCI) -> u3g/umsm serial -> pheltsy over USB AT ->
umb + NIF -> cdce fallback -> urndis fallback -> Zephyr off/wake.
