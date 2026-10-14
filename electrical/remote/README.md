# Remote Circuit — Handheld Controller

The remote is the separate handheld circuit used to manually control the mobile robot. It reads joystick and switch inputs and transmits commands over ESP-NOW to the [robot receiver](../receiver/README.md).

**Status:** Remote circuit design files have not been supplied yet. The existing [transmitter firmware](../../firmware/esp-now/trans/trans.ino) is available. The receiver's ESP32 DevKitC V4 identification does not establish the remote's exact board model.

## Design files to add

| Folder | Intended contents |
| --- | --- |
| `source/` | Editable remote KiCad project, schematic, and PCB |
| `manufacturing/` | Remote Gerbers and drill files |
| `schematics/` | Remote schematic PDFs |
| `pcb/` | Remote PCB layout PDFs |
| `images/` | Remote PCB renders and photos |

Create these subfolders when their files are supplied. All receiver design files remain in the separate receiver folder.

## Input reference from transmitter firmware

These assignments describe the current code; they have not been checked against a remote schematic.

| Transmitter input | GPIO |
| --- | --- |
| Joystick x | 35 |
| Joystick y | 34 |
| Spinner switch | 12 |
| System switch, unused by current control logic | 14 |
| Rotate left | 15 |
| Rotate right | 13 |


[Back to electrical overview](../README.md)

## Early remote photo

A [photo of the old handheld remote](../../docs/history/images/early-remote.png) is available in the project history. This photo is separate from the editable remote circuit designs, which have not yet been supplied.
