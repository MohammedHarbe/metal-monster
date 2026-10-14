# Metal Monster

Metal Monster is a manually controlled combat robotics project that our team has been working on since **2023**. It uses ESP-NOW communication between a handheld remote and an onboard receiver, wheelchair motors for mobility, and a starter-motor-driven metal spinner for robot-versus-robot competition.

The receiver uses an **ESP32 DevKitC V4** mounted on a custom interface PCB.

![Metal Monster robot](mechanical/images/metal-monster-robot.jpg)

## Explore the project

| Section | Contents | Status |
| --- | --- | --- |
| [Mechanical](mechanical/README.md) | CAD, assemblies, drawings, and fabrication exports | Mechanical design files not yet supplied |
| [Firmware](firmware/README.md) | ESP-NOW transmitter and receiver | Existing sketches preserved |
| [Electrical](electrical/README.md) | Editable KiCad project, Gerbers/drills, PDFs, board renders, and relay interface documentation | Source and manufacturing files included |

## Repository layout

```text
metal-monster/
  README.md
  docs/history/             # Early setups, team photo, and first-operation video
  mechanical/README.md
  firmware/
    README.md
    esp-now/trans/trans.ino
    esp-now/reciver/reciver.ino
    legacy/                 # Unused original SBUS sketch
  electrical/
    README.md
    receiver/
      README.md
      source/               # KiCad project, schematic, PCB, original archive
      manufacturing/        # Gerbers, drills, original archive
      schematics/Reciver_pdf.pdf
      pcb/PCB_reciver.pdf
      images/               # Three PCB renders and starter reference photo
    remote/
      README.md             # Reserved for handheld circuit designs
```

## Hardware and Power

The following configuration is supplied by the project team:

| Subsystem | Hardware / supply |
| --- | --- |
| Drive motors | Two wheelchair motors |
| Drive motor controller | MDD20A motor driver |
| Drive battery | 25 V lithium-ion battery, as reported by the team |
| Spinner motor | Starter motor driving a metal spinner |
| Spinner battery | 12 V, 7.5 Ah LiPo battery, as reported by the team |
| Robot receiver | ESP32 DevKitC V4 on a custom PCB |
| Wireless control | ESP-NOW handheld remote and robot receiver |
| Spinner switching | BC338 transistor drives a 5 V relay coil; relay contacts switch starter motor power directly |

Battery voltages are recorded as supplied; nominal/full-charge values and cell counts have not been established. The motor reference image is labelled MY1016Z, 24 V, 250 W, but the installed motors' exact model and nameplate ratings have not been confirmed.

## Two circuits

- **[Receiver](electrical/receiver/README.md):** Installed on the mobile robot. All currently supplied circuit files belong here.
- **[Remote](electrical/remote/README.md):** Separate handheld circuit for manual control. Its design files are still to be added.

## How it works

The transmitter reads joystick and switch inputs and sends commands over ESP-NOW. The receiver drives two external motor-driver interfaces. Its spinner GPIO controls a BC338 transistor stage that switches a 5 V relay coil. The project owner confirms that the relay contacts switch the starter motor power directly.

The relay coil uses 5 V; the starter motor uses the separate, team-reported 12 V, 7.5 Ah LiPo supply. Starter current and relay contact ratings remain undocumented.

## Getting started

1. Read the [firmware guide](firmware/README.md).
2. Review the [receiver electrical design](electrical/receiver/README.md), including pin assignments and relay operation.
3. Open each ESP-NOW sketch separately in its matching folder.
4. Resolve the documented transmitter issues before attempting a build or robot test.

## Status

Repository organization does not change firmware logic. Compilation and hardware validation have not been performed as part of this update. The transmitter still has a missing receiver-address declaration and direction-mapping issues documented in the firmware guide.

The original SBUS file is preserved as legacy material only; **SBUS is not used on this robot**. Electrical files include editable KiCad sources, Gerber/drill exports, PDFs, and images. Mechanical CAD has not been added.

## Our Team and Project History

We have been working on Metal Monster since **2023**, bringing together the robot's mechanical structure, electronics, and wireless control.

![Metal Monster team](docs/history/images/team.jpg)

Our [project history](docs/history/README.md) includes the robot before painting and later modifications, the old robot receiver, early handheld remote, and the video identified by the team as the robot's first integrated operation.

[Watch or download the first-operation video](docs/history/videos/first-integrated-operation.mp4).
