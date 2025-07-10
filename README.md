# 🚗 Differential Drive Robot: Step-by-Step Calculations and Control System

This document explains the math and logic behind controlling a **two-wheeled differential drive robot** using PWM-driven stepper motors. It includes detailed kinematic derivations, frequency conversion for motor drivers, and acceleration profiles to transition between motion phases smoothly.

Designed to work with a **Jetson Nano** as a controller that sends velocity commands, and an **ATmega-based board** that interprets them and drives two motors.

---

## 📘 Table of Contents

1. [System Overview](#system-overview)  
2. [Differential Drive Kinematics](#1-differential-drive-kinematics)  
3. [Frequency Calculations](#2-frequency-calculation-from-velocity)  
4. [Acceleration and Velocity Profile](#3-velocity-profile--acceleration-phases)  
5. [Command Interface](#4-usb-command-interface)  

---

## System Overview

In a differential drive robot:

- Two wheels are independently driven.
- The robot moves forward/backward by setting both wheel speeds equal.
- The robot rotates by making the wheel speeds unequal (e.g., left slower or reversed).
- The center linear velocity (`V`) and angular velocity (`ω`) are used to determine wheel speeds.

The robot receives:

- `V` (m/s): center linear velocity  
- `ω` (rad/s): angular velocity about the center  

These values are sent from **Jetson Nano** to the microcontroller, which computes:

- Left and right wheel velocities (`v_L`, `v_R`)  
- Corresponding **PWM step frequencies** (`f_L`, `f_R`)  
- Applies **acceleration profiles** for smooth motion  

---

## 1. Differential Drive Kinematics

### 🔹 Purpose

Convert center velocity `V` and angular velocity `ω` into left and right wheel velocities.

### 🔹 Variables

- `V`: Linear velocity of the robot’s center (m/s)  
- `ω`: Angular velocity of the robot (rad/s)  
- `s`: Distance between the wheels (m)  

### 🔹 Equations

v_L = V - (s / 2) * ω
v_R = V + (s / 2) * ω


These determine how each wheel must move to achieve the desired linear and rotational motion.

---

## 2. Frequency Calculation from Velocity

### 🔹 Purpose

Convert the wheel’s **linear velocity** into a **PWM frequency** to control a stepper motor.

### 🔹 System Specs

- Stepper resolution: 12,800 steps/revolution  
- Gear ratio: 1:10 (motor spins 10 times for one wheel rotation)  
- Effective steps per wheel revolution:  
  `steps_per_rev = 12800 * 10 = 128000`  

- Wheel radius: 6 cm = 0.06 m  
- Wheel circumference:  
  `C = 2πr = 2 * π * 0.06 ≈ 0.37699 meters`

### 🔹 Steps Per Meter

To travel 1 meter:

steps_per_meter = 128000 / 0.37699 ≈ 339581.68


### 🔹 Final Formula

To convert a wheel velocity `v` (in m/s) to a frequency `f` (in steps/sec):

f = v * 339581.68


This frequency is sent to the stepper motor via PWM. Direction is managed separately by a GPIO pin.

---

## 3. Velocity Profile & Acceleration Phases

### 🔹 Purpose

Prevent instant jumps in velocity by gradually **accelerating or decelerating** each wheel until it reaches the target speed.

This smoothens motion and avoids jerky behavior, especially important when starting, stopping, or turning.

---

### 🔹 Variables

- `acc_c`: Linear acceleration at the center (m/s²)  
- `acc_omega`: Angular acceleration (rad/s²)  
- `s`: Distance between wheels (m)  
- `v_L`, `v_R`: Current left/right wheel velocities  
- `v_L_target`, `v_R_target`: Desired left/right wheel velocities  

---

### 🔹 Motion Phases

We use three phases to control the motion:

---

#### 🟢 Phase 1: Start Phase (robot starts from rest)

**When:** `V_prev = 0`  
**Then:**
acc_R = acc_c + (s / 2) * acc_omega
acc_L = acc_c - (s / 2) * acc_omega


> Both linear and angular acceleration affect the right/left wheels.

---

#### 🟡 Phase 2: Moving Phase (robot is already moving)

**When:** `V_prev ≠ 0`  
**Then:**

acc_R = (s / 2) * acc_omega
acc_L = - (s / 2) * acc_omega


> Similar to start phase, but in reverse. Applies deceleration.

---

### 🔄 Update Rule (Per Control Cycle)

At every time step `dt`:

v_next = v_current ± acc * dt


Repeat this update until:

v_current ≈ v_target (within a tolerance)


Convert the new velocity into PWM frequency using:

f = v * 339581.68

Update direction pin based on sign of `v`.

---

## 4. USB Command Interface

The system is controlled via USB from a Jetson Nano or any PC.

---

### 📥 Input Format

set <V> <ω>


Where:
- `V`: desired center velocity (m/s)  
- `ω`: desired angular velocity (rad/s)  

Examples:
set 0.3 0.0 → forward
set 0.3 0.2 → curve right
set 0.0 0.0 → stop
set -0.3 0.0 → reverse


---

### 📤 Internal Steps After Receiving Command

1. Parse `V` and `ω`  
2. Compute `v_L` and `v_R`  
3. Compute acceleration based on phase  
4. Update `v_L` and `v_R` over time  
5. Convert `v_L`, `v_R` to `f_L`, `f_R`  
6. Output PWM and direction to motor drivers  

---

## ✅ Summary

| Component            | Description                            |
|----------------------|----------------------------------------|
| `v_L`, `v_R`         | Left and Right wheel velocities        |
| `f_L`, `f_R`         | Frequencies (steps/sec) for motors     |
| `acc_c`, `acc_omega` | User-defined accelerations             |
| `s`                  | Distance between wheels                |
| Jetson Nano          | Sends `V` and `ω`                      |
| ATmega + PWM         | Applies acceleration & controls motors |

---

## 📝 License

MIT License — free to use, modify, and share with attribution.

---

**Author:**  
Bathiya Dissanayake  
University of Moratuwa — Department of Electronics and Telecommunication Engineering

