# Electrical Designs

Griffen has two separate circuits communicating over ESP-NOW:

| Circuit | Location and role | Design status |
| --- | --- | --- |
| [Receiver](receiver/README.md) | Installed on the mobile robot; controls drive-motor interfaces and the spinner relay | KiCad sources, Gerbers/drills, PDFs, and PCB renders supplied |
| [Remote](remote/README.md) | Handheld controller for manual driving and spinner commands | Place reserved; design files not yet supplied |

```text
electrical/
  README.md
  receiver/
    README.md
    source/
    manufacturing/
    schematics/
    pcb/
    images/
  remote/
    README.md
```

The currently supplied electrical designs all belong to the **robot receiver**. The remote has its own documentation and upload location.

## Firmware pairing

- [Remote transmitter sketch](../firmware/esp-now/trans/trans.ino)
- [Robot receiver sketch](../firmware/esp-now/reciver/reciver.ino)

See the [firmware guide](../firmware/README.md) for setup and current code issues.
