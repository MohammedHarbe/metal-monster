# Receiver Circuit — Mobile Robot

This circuit is installed on the mobile robot and receives manual-control commands from the handheld remote over ESP-NOW.

[Electrical overview](../README.md) · [Remote circuit](../remote/README.md) · [Receiver firmware](../../firmware/esp-now/reciver/reciver.ino)

## Receiver hardware

The receiver uses an **ESP32 DevKitC V4**, confirmed by the project owner. The custom PCB provides board headers, two motor-driver signal connectors, a 5 V relay-driver stage, and relay contact terminals.

### Supplied files

- [Editable KiCad project](source/Reciever_metal_monster.kicad_pro)
- [Editable schematic](source/Reciever_metal_monster.kicad_sch)
- [Editable PCB](source/Reciever_metal_monster.kicad_pcb)
- [Original KiCad archive](source/Reciever_metal_monster_PCB.rar)
- [Manufacturing archive](manufacturing/METAL_MONSTER_RECIEVER_GBR.rar)
- [Extracted manufacturing files](manufacturing/METAL_MONSTER_RECIEVER_GBR)

- [Receiver schematic](schematics/Reciver_pdf.pdf)
- [PCB layout export](pcb/PCB_reciver.pdf)
- [Top PCB render](images/Reciever_metal_monster.png)
- [Angled PCB render](images/Reciever_metal_monster_2.png)
- [Underside PCB render](images/Reciever_metal_monster_3.png)
- [Starter motor reference photo](images/starter-motor-reference.jpg)

The photo is a user-supplied reference, not an identified manufacturer part or a verified image of the installed assembly. Original filenames are retained for the design exports. Editable KiCad sources and Gerber/drill exports are now included. A component BOM has not been supplied.

## Spinner relay interface

The supplied schematic shows:

| Item | Value / role |
| --- | --- |
| Control net | `Spiner_ESP` (original schematic spelling) |
| Transistor | BC338, low-side coil switch |
| Base series resistor | 470 ohms |
| Base pull-down | 10 kilohms to ground |
| Relay | SPDT, coil connected to +5 V |
| Coil diode | 1N4007 across the coil |
| Indicator | LED with 220-ohm series resistor |
| Contact outputs | COM, NO, NC |

Control path:

```text
ESP32 spinner GPIO -> 470-ohm resistor -> BC338 base
+5 V -> relay coil -> BC338 collector/emitter -> GND
Relay contacts -> starter motor power circuit (direct switching, owner-confirmed)
```

The transistor provides coil-current drive; the ESP32 GPIO does not supply the coil's 5 V power. Based on the schematic, a HIGH control signal turns on the transistor and energizes the relay; LOW releases it. The 1N4007 is across the relay coil, not across the external starter motor.

The project owner confirms that the relay contacts switch motor power directly, rather than energizing the starter solenoid. External battery wiring and the selected COM/NO/NC connection are not shown in the supplied schematic. The team reports a separate 12 V, 7.5 Ah LiPo supply for the starter motor. The relay's 5 V rating describes its coil supply.

**Load capacity is unverified:** the PDF names a generic relay; the editable source uses a SANYOU SRD-series symbol/footprint and does not specify its DC motor-load contact rating, starter inrush/stall current, or PCB current-carrying capacity. The presence of a coil driver alone does not establish that the contact path is suitable for switching this starter motor.

## ESP-NOW firmware GPIO reference

These values come from the preserved sketches. They are GPIO numbers, not the 1-19 header-position labels in the schematic.

| Receiver signal | GPIO |
| --- | --- |
| Motor 1 PWM | 12 |
| Motor 1 DIR | 22 |
| Motor 2 PWM | 13 |
| Motor 2 DIR | 18 |
| Spinner control | 16 |

The receiver sets GPIO 16 HIGH when `swit == 0`; boot setup and the 500 ms packet timeout set it LOW. This matches the intended active-high transistor stage in the schematic, subject to verifying board orientation and actual wiring.

The drawing uses two numbered 19-pin headers. Verify their orientation against the actual DevKitC V4 before treating header numbers as GPIO assignments. [Espressif board and pin-layout reference](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/user_guide.html).

## Information still needed

Add the relay part number and ratings, starter specifications, actual motor-power wiring, protection and power-distribution details, and a component BOM. Record the physical motor 1/motor 2 sides and verified forward direction. Mechanical CAD belongs in the mechanical section.

These documents describe supplied evidence and owner-confirmed configuration; they do not certify PCB fabrication, relay load capacity, or bench operation.

## Opening the source and manufacturing package

Open the `.kicad_pro` file in `source/`, keeping the schematic and PCB beside it. The board references KiCad 9 standard 3D model library paths (`KICAD9_3DMODEL_DIR`). Original archives are preserved alongside the extracted files.

The manufacturing archive contains 10 Gerber layers, a Gerber job file, and separate plated/non-plated drill files (13 files total). Archive extraction and file presence were checked; ERC, DRC, fabrication readiness, and consistency between source and manufacturing exports have not been validated.

## Robot drive and power configuration

The team identifies the base motors as wheelchair motors controlled by an MDD20A motor driver, powered by a reported 25 V lithium-ion battery. A separate reported 12 V, 7.5 Ah LiPo battery powers the starter motor driving the metal spinner for robot-versus-robot competition.

These are owner-reported hardware details, not a completed compatibility assessment. Battery cell counts and nominal/full-charge voltages, motor nameplate details, current measurements, and the receiver/relay 5 V supply arrangement still need documentation. The supplied schematic does not show the complete two-battery power wiring.

## Earlier hardware

See the [old receiver setup](../../docs/history/README.md#early-receiver-setup) for the earlier enclosure-mounted prototype. The design files in this folder document the supplied receiver PCB.
