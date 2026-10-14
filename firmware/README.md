# Firmware

Metal Monster uses ESP-NOW. The robot receiver board is an **ESP32 DevKitC V4**, as confirmed by the project owner.

## Active sketches

| Sketch | Role | Dependencies visible in source |
| --- | --- | --- |
| [ESP-NOW transmitter](esp-now/trans/trans.ino) | Reads joystick and switches; sends movement and spinner commands | ESP32 Arduino environment, WiFi, ESP-NOW |
| [ESP-NOW receiver](esp-now/reciver/reciver.ino) | Receives commands; controls two motor outputs and spinner relay | ESP32 Arduino environment, WiFi, ESP-NOW, CytronMotorDriver |

Each sketch is separate, with a folder matching its original filename. The original spelling of `reciver.ino` is preserved. Do not combine these sketches into one Arduino sketch.

## Opening and configuring

1. Open the chosen linked `.ino` file in Arduino IDE.
2. Select the actual ESP32 board and serial port.
3. Install the dependencies listed above. Exact working board-package and library versions have not been recorded in this repository.
4. For ESP-NOW, verify the receiver station MAC, both devices' Wi-Fi channels, and callback signatures against the installed ESP32 package.
5. Review the [electrical pin reference](../electrical/README.md) before connecting hardware.

## ESP-NOW packet and inputs

Both ESP-NOW sketches declare the fields in this order: `int x`, `int y`, `int swit`, `int order`.

The transmitter reads GPIO 35 for x and GPIO 34 for y. GPIO 12 supplies the spinner switch state; GPIOs 15 and 13 request left and right rotation. GPIO 14 is read as a system switch but does not currently affect control.

Orders are: 1 stop, 2 right, 3 left, 4 backward, 5 forward, 6 forward-left, 7 forward-right, 8 backward-left, 9 backward-right, 10 rotate-left, and 11 rotate-right.

Inputs are sampled approximately every 100 ms. The send task attempts transmission every 10 ms when its callback gate allows it.

## Known issues and validation boundaries

- The transmitter references `receiverAddress` but does not declare it. Its printed MAC address is not a configured peer address.
- Diagonal joystick regions conflict with the straight-direction mapping. Some receiver speed calculations consequently produce unintended negative speeds.
- Input and send tasks share a packet without synchronization. The send gate is also cleared after the send call, leaving a callback-ordering race.
- `receiverConfirmed` remains true after the first successful transmission, so the displayed link status is not a live connection check.
- The current ESP-NOW receiver checks packet length and has a 500 ms packet timeout. The supplied BC338 relay-driver schematic indicates HIGH energizes the coil and LOW releases it; physical operation has not been bench-verified here.
- Firmware contents were preserved during repository organization. Compilation, uploads, motor direction, and relay behavior have not been validated by this documentation change.

For initial drive checks, raise the wheels and disconnect spinner power. Confirm each motor's forward direction and the relay's actual inactive state before powered operation.

## Preserved legacy file

The unused SBUS sketch is retained under [legacy](legacy/README.md) for history only. It is not an alternative setup recommended for this robot.
