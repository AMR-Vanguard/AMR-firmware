# AMR-firmware

This repository contains the firmware developed for our **Autonomous Mobile Robot (AMR)** project. The firmware is written and compiled for the **ATmega32U4** microcontroller using **Microchip Studio**.

---

## 🧠 Microcontroller: ATmega32U4

The **ATmega32U4** is an 8-bit AVR microcontroller from Microchip, well-suited for embedded systems, especially mobile robotics. It features:

- **Architecture**: AVR 8-bit RISC
- **Operating Voltage**: 2.7V – 5.5V
- **Flash Memory**: 32 KB (4 KB used for bootloader)
- **SRAM**: 2.5 KB  
- **EEPROM**: 1 KB  
- **Clock Speed**: Up to 16 MHz  
- **I/O Pins**: 26  
- **ADC**: 10-bit, 12 channels  
- **USB Support**: Native USB 2.0 (ideal for programming and serial communication)

We chose this microcontroller for its rich feature set and integrated USB functionality, which simplifies both programming and debugging.

---

## 💻 Development Environment

- **IDE**: [Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio) (formerly Atmel Studio)
- **Language**: C (AVR-GCC compiler)
- **Debugger/Programmer**: USB bootloader (via LUFA)

---

## 🚀 Features 

- Motor control using PWM
- Control Motors according to serial commands via USBmmm

---

## 🛠️ How to Build & Flash

1. Open the solution in **Microchip Studio**.
2. Connect the AMR board via USB.
3. Ensure ATmega32U4 is in bootloader mode (if needed).
4. Press **F5** to build and flash.
5. Use **Serial Terminal** or **Data Visualizer** for debugging.

---

