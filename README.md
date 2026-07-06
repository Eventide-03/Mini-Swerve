# Swerve Drivetrain

A fully custom four-module swerve drivetrain built to replicate the FRC/FTC swerve drivetrains. Every part of this project was designed/planned by me.

## Project Resources

- **[CAD (Onshape)](https://cad.onshape.com/documents/22f3425ea7c3c5baea4db72a/w/41cf12666249611bfbd4a57b/e/570cbec20c3995e10be4fe6f?renderMode=0&uiState=6a05716b29587ab90c126fd3)** — full mechanical assembly and exploded views
- **[Bill of Materials (Google Sheets)](https://docs.google.com/spreadsheets/d/13JzB3kvO0vG1ZuJ61luXd1x6dMnEUsCQSgEZeQXHTig/edit?usp=sharing)** — every part, quantity, and source
- **[Calculations (Google Docs)](https://docs.google.com/document/d/1093Fhib_KALhrss7tIt823cWR-YH7OZk02LRxrgmfjs/edit?usp=sharing)** — gear ratios, motor sizing, and drivetrain math

## Overview

Swerve drive is an omnidirectional drivetrain, which means each wheel module can independently steer and drive, allowing a robot to move and rotate at the same time. I thought it would be nice to make a tinier at-home version for my own enjoyment, normally they are pretty big, but this one is fun sized.

This build has the full swerve control system on a 12.25 × 12.25 inch chassis with four independent modules. Each module uses 5010 brushless motors for driving and steering, using gear reductions to achieve the required torque. This leaves us with a miniature platform that actslike a full-size competition swerve, but small enough to use in my own house.

In order to save money I designed a custom PCB in order to hold everything, making it much easier to wire. In addition I will be using my SpeedyBee f405 to control the drive motors to avoid having to pay for 4 extra motor controllers.

## Specifications

| Attribute | Detail |
|---|---|
| Chassis footprint | 12.25 × 12.25 in |
| Modules | 4 (independent steer + drive) |
| Motors | 8 × 5010 360 kV brushless (4 drive, 4 steer) |
| Reduction | 8:1 per axis |
| Master controller | ESP32-S3-DevKitC-1 |
| Steering control | 2 × Makerbase (MKS) ESP32 Dual FOC ESC, controlled over UART via SimpleFOC Commander |
| Drive control | SpeedyBee F405 4-in-1 ESC (DSHOT600) |
| Steering feedback | 4 × AS5600 magnetic encoders (I²C) |
| Radio control | RadioMaster Pocket ELRS (TX + receiver) |
| Power | 4S LiPo (14.8 V) → Mini-360 buck converter (5 V) for logic |

## Architecture

The ESP32-S3-DevKitC-1 runs most of the code and is the brain reading commands from the ELRS receiver, and running the swerve kinematics, and sending the commands to the two motor-control subsystems:

- **Driving:** The master sends DSHOT600 signals to the SpeedyBee F405 4-in-1 ESC, which drives the four drive motors.
- **Steering:** The master sends addressed commands over a shared UART bus to two MKS ESP32 Dual FOC boards (using [SimpleFOC](https://simplefoc.com/) Commander). Each board runs field-oriented control on two steering motors, using an AS5600 magnetic encoder per module for absolute angle feedback.

All of this lives on the carrier PCB that seats the ESP32-S3 and breaks out clean, labeled connectors for the ESC, MKS boards, encoders, receiver, and power.

> **Important:** The steering magnets must be diametrically magnetized (poles across the diameter, not top-to-bottom), 6 × 2.5 mm, and mounted on the steering rotation axis, not the motor shaft or drive shaft. The AS5600 reports an absolute 0–360°, so it must rotate 1:1 with the module's steering angle.

---

## Assembly Instructions

Work through these in order. Use the **[CAD exploded views](https://cad.onshape.com/documents/22f3425ea7c3c5baea4db72a/w/41cf12666249611bfbd4a57b/e/570cbec20c3995e10be4fe6f?renderMode=0&uiState=6a05716b29587ab90c126fd3)** go to the assembly and click the exploded views to the left.

### 1. Gather parts
Order everything on the **[BOM](https://docs.google.com/spreadsheets/d/13JzB3kvO0vG1ZuJ61luXd1x6dMnEUsCQSgEZeQXHTig/edit?usp=sharing)** and 3D-print the printed components from the CAD. Confirm you have all fasteners, belts, bearings, and magnets before starting.

### 2. Populate and solder the PCB
- Solder the through-hole headers and connectors to the custom carrier board, following the silkscreen labels.
- Solder the decoupling capacitors near the ESP32 5 V pin.
- Solder the pin-socket rows that the ESP32-S3-DevKitC-1 plugs into.
- **Before powering anything, use a multimeter to verify there are no shorts** between 5 V, GND, and the battery rails.

### 3. Wire the drive ESC (SpeedyBee F405)
- Connect the four drive-motor signal lines and a shared ground from the SpeedyBee ESC to the DSHOT header on the carrier board.
- Use only the M1–M4 signal pads plus one GND pad (verify GND with a multimeter first — M5–M8 pads on the board have no hardware behind them).

### 4. Wire the steering controllers (MKS ESP32 Dual FOC)
- Connect each MKS board's UART (TX/RX/GND) to the shared steering UART nets on the carrier board.
- Both boards share the bus and are addressed individually in firmware via SimpleFOC Commander.

### 5. Set up the encoders (AS5600)
- Mount each AS5600 so its chip faces the diametrically-magnetized magnet on the **steering rotation axis** of each module.
- Wire each encoder's VCC / GND / SDA / SCL to its MKS board's encoder header, and tie DIR to GND.
- Before final assembly, flash SimpleFOC firmware to the MKS boards, configure the motors, and run encoder calibration for each module.

### 6. Assemble the mechanical drivetrain
- Build the four swerve modules and mount them to the chassis per the **[CAD exploded views](https://cad.onshape.com/documents/22f3425ea7c3c5baea4db72a/w/41cf12666249611bfbd4a57b/e/570cbec20c3995e10be4fe6f?renderMode=0&uiState=6a05716b29587ab90c126fd3)**.
- Install belts, bearings, and wheels; verify each module steers freely and each drive wheel spins without binding.

### 7. Power distribution
- Wire the 4S LiPo through the switch and fuse, then split to the SpeedyBee ESC, both MKS boards, and the Mini-360 buck converter input.
- The buck's 5 V output feeds the carrier board's logic rail (ESP32 + receiver). **The battery's 14.8 V never reaches the carrier board directly** — only the buck input does.

### 8. Flash the ESP32-S3 and bring it up
- Open the firmware in **[Arduino IDE 2.x](https://www.arduino.cc/en/software)** and select the **ESP32S3 Dev Module** board.
- Bring the system up progressively rather than all at once:
  1. **Smoke test** — confirm the board powers and blinks.
  2. **Drive test** — verify DSHOT control of a single drive motor.
  3. **Steering test** — send a UART target to an MKS board and confirm the steering motor holds position.
  4. **Receiver test** — confirm the ESP32 reads ELRS channels.
- Once each subsystem checks out, run the full firmware to read the transmitter, compute swerve kinematics, and drive all eight motors together.

---

## Notes

- Some ESP32-S3-DevKitC-1 revisions have different row spacing (22.86 mm vs 25.4 mm), measure your board with calipers before finalizing the PCB.
- Measure the Mini-360 buck module's pad spacing with calipers as well before ordering the board.
- The library APIs (DSHOT, SimpleFOC, ELRS) may need small adjustments to match current versions, adapt from each library's examples.


<img width="1440" height="857" alt="image" src="https://github.com/user-attachments/assets/ae393e84-b265-4a1d-abb6-01594c5f71ea" />
<img width="984" height="697" alt="image" src="https://github.com/user-attachments/assets/7862db91-1779-4b8a-a21c-b66678d3aee7" />
<img width="730" height="716" alt="image" src="https://github.com/user-attachments/assets/7a225888-6da3-4ceb-adf9-0d4aa24d41ed" />
<img width="630" height="625" alt="image" src="https://github.com/user-attachments/assets/2b350a39-4ffd-4dbb-90e6-19a3554e8875" />
