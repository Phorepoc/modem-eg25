# Design

```
 Phone / SysAp / Messaging / Contacts      (platform, unchanged)
   -> ETel -> phonetsy.tsy -> commontsy (CTSY, classic licensee interface)
     -> licenseetsy.dll = pheltsy          (this repo, EPL)
          router: CTSY IPC -> AT handler; completions via MessageManager()->Complete
          atcore: framing, classification, parsers   (src/atcore)
          MTelephonyAudioControl -> call audio route on/off
     -> RBusDevComm (ECOMM.LDD)
          Comm.A64 unit 3 = UART3 to the modem    (adaptation-a64, exists)
          later: a USB "vendor bulk serial" PDD on the A64 USB1 host (EHCI), interfaces 2 (AT) and 3 (AT/PPP)
 Packet data: ETel Packet (etelpacketdata) + SPUD (pdplayer/umts) -> PPP NIF over a second AT
   channel (27.010 CMUX via csy27010 on the UART, or USB interface 3); raw-IP/QMI not planned.
 Call audio: modem PCM -> A64 codec AIF2, routed codec-internally (no samples through Symbian).
```

Order:
1. pheltsy moves into `src/pheltsy/`, its hand-written line parsing replaced by `atcore`.
2. A scripted AT peer (same `atcore`) for QEMU/VirtualBox, so the x86 image runs the real TSY.
3. Hold/swap/conference (`+CHLD`), DTMF (`+VTS`), emergency calls, `+CDS` status reports.
4. Packet data over CMUX on UART3.
5. USB transport once the A64 EHCI host exists.
