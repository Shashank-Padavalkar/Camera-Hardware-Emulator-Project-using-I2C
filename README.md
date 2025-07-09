# I2C Camera Hardware Emulator Project

## Overview
This project emulates an I2C camera system using an **Arduino Uno R4 WiFi (Emulator)** and an **Jetson AGX Orin Dev Kit (Controller)**. The system mimics a camera's behavior by sending and receiving data over I2C, and control bytes transmission over a custom 3 GPIO pin custom protocol, with the Arduino functioning as a slave device and the Jetson AGX Orin as the master. It includes an I2C control register, interrupt register, and a user input based error simulation.

## System Setup Overview
![system-overview](https://github.com/user-attachments/assets/e03c9de9-d741-43dd-8843-c7a7d7f2d299)

## Features
- **I2C Communication:** Implements a I2C protocol for data exchange between an Arduino Uno R4 WiFi (slave) and a Jetson (master).
- **Control and Interrupt Registers:** Uses two key registers for controlling communication and handling interrupts:
  - **Control Register (CR)**
  - **Interrupt Register (IR)**
- **GPIO Signaling:** Manages GPIO signals for acknowledgment and communication control between the Arduino and Jetson.
- **Error Simulation:** Allows the user to pick and simulate various I2C communication errors via UART input.

## Hardware Setup
- **Arduino Uno R4 WiFi** (Emulator)
- **Jetson AGX Orin Dev Kit** (Controller)
- **Bi-directional Logic Level Shifter** (To step down 5V to 3.3V)
- **GPIO Pins:**
  - **Arduino**:
    - GPIO 2 (Input Trigger)
    - GPIO 4, 5, 6 (GCP-3 Pins)
    - GPIO 7 (Slave ACK Pin)
  - **Jetson**:
    - GPIO 29 (Input Trigger)
    - GPIO 33, 35, 37 (GCP-3 Pins)
    - GPIO 31 (Slave ACK Pin)

## Software Setup

### Arduino Sketch
The Arduino sketch implements the I2C slave communication, handling the control and interrupt registers. The communication is controlled via specific GPIOs.

### Jetson Script
The Jetson script handles the I2C master communication, controlling the communication configuration and sending commands to the Arduino slave.

## Error Configuration:
These errors are configured by the user through UART input on the Arduino, which sets the corresponding bits in the Interrupt Register (IR) to high.
  - ALWAYS_NACK: Causes the slave to always return a NACK (No Acknowledge) in communication.
  - DELAYS: Introduces intentional delays between data transactions.
  - EXTRA_BYTE: Inserts an extra byte into the communication stream.
  - MISSING_BYTE: Omits a byte from the communication stream.
  - BIT_FLIPS: Randomly flips bits in the data being transmitted.

| UART Input  | Error Type            |
|------|------------------------------|
| A | NO_ERROR  |
| 0 | ALWAYS_NACK |
| 1 | DELAYS |
| 2 | EXTRA_BYTE |
| 3 | MISSING_BYTE  |
| 4 | BIT_FLIPS |

## Custom Registers

### Register Map Overview

| Register | Address       |
|----------|---------------|
| CTRL_REG | 0x00          |
| INTR_REG | 0x01          |
| DATA_REG | 0x02 ownwards |

### Control Register (CR)
The **Control Register (CR)** is used to control various aspects of communication and status. It is written and read by the master (Jetson) during I2C operations.

| Bit  | Description                  |
|------|------------------------------|
| Bit 0 | READ_ONLY (Read Only Mode) |
| Bit 1 | NACK (No Acknowledgement) |
| Bit 2 | MODE_16B (16-bit Data Mode) |
| Bit 3 | READ_ACK (Read Acknowledgement) |
| Bit 4 | WRITE_ACK (Write Acknowledgement) |
| Bit 5 | READ_RQ (Read Request) |
| Bit 6 | WRITE_RQ (Write Request) |
| Bit 7 | EN_COM (Enable Communication) |
  
### Interrupt Register (IR)
The **Interrupt Register (IR)** holds flags for simulated communication errors. The error simulation bits are set by the user through UART input and the interrupts are triggered randomly through GPIO hardware interrupt.

| Bit  | Description                              |
|------|------------------------------------------|
| Bit 0 | ALWAYS_NACK (Always No Acknowledge)      |
| Bit 1 | DELAYS (Insert Delays)                   |
| Bit 2 | EXTRA_BYTE (Insert Extra Byte)           |
| Bit 3 | MISSING_BYTE (Omit a Byte)               |
| Bit 4 | BIT_FLIPS (Random Bit Flips)             |
| Bit 5 | RESERVED        |
| Bit 6 | RESERVED      |
| Bit 7 | RESERVED              |

## GPIO 3-Bit Command Protocol (GCP-3)
Implemented a custom 3-bit protocol using GPIO pins to transmit control commands from Jetson to Arduino.

| Bits  | Command Name | Description                       |
|-------|--------------|-----------------------------------|
| 000   | NO_OP        | No operation                     |
| 010   | MODE_16B     | Set 16-bit mode                  |
| 011   | RD_ACK       | Jetson confirms read complete     |
| 100   | WR_ACK       | Jetson confirms write complete    |
| 101   | READ_RQ      | Jetson requests read              |
| 110   | WRITE_RQ     | Jetson requests write             |
| 111   | EN_COM       | Jetson enables communication      |

## Operations Supported
- String Transmission with Simulated Errors
- Black & White Image Transmission with data integrity validation using sha-256 and md5 checksum methods
- Color Image Transmission with data integrity validation using sha-256 and md5 checksum methods

## Contributing Teammates
- [Anirudh Navalgund](https://github.com/Anirudh-Navalgund)
- [E Sujaya](https://github.com/Sujaya-E)
- [Royston Vedamuthu](https://github.com/RoystonV)
