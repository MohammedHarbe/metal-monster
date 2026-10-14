# Electrical Designs

Griffen has two separate circuits communicating over ESP-NOW:

| Circuit | Location and role | Design status |
| --- | --- | --- |
| [Receiver](receiver/README.md) | Installed on the mobile robot; controls drive-motor interfaces and the spinner relay | KiCad sources, Gerbers/drills, PDFs, and PCB renders supplied |
| [Remote](remote/README.md) | Handheld controller for manual driving and spinner commands | KiCad review ZIP, PCB mechanical package, Gerbers/drills, PDFs, and renders supplied |

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
    source/
    mechanical/
    manufacturing/
    schematics/
    images/
```

Both circuits now have supplied design packages: the **robot receiver** and the **handheld remote**. Their files are kept in separate folders.

## Firmware pairing

- [Remote transmitter sketch](../firmware/esp-now/trans/trans.ino)
- [Robot receiver sketch](../firmware/esp-now/reciver/reciver.ino)

See the [firmware guide](../firmware/README.md) for setup and current code issues.
