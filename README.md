# 📘 Differential Drive Kinematic Derivation

## Inputs

- Center linear velocity: `V` (m/s)
- Angular velocity: `ω` (rad/s)
- Distance between wheels: `s` (m)

## Output: Individual Wheel Velocities

To achieve the motion, we compute:

\[
v_L = V - \frac{s}{2} \cdot \omega
\]
\[
v_R = V + \frac{s}{2} \cdot \omega
\]

- If `ω > 0`: turns left (inner = left wheel)
- If `ω < 0`: turns right
# 📘 Frequency Calculation from Velocity

## Parameters

- Motor resolution: 12,800 steps/rev
- Gear ratio: 1:10 → steps/wheel = 12800 × 10 = **128000**
- Wheel radius = 6 cm = 0.06 m

---

## Step Calculations

### 1. Wheel Circumference:

\[
C = 2 \pi r = 2 \pi \cdot 0.06 \approx 0.37699 \, \text{m}
\]

### 2. Steps per meter:

\[
\text{steps/m} = \frac{128000}{0.37699} \approx 339581.68
\]

### 3. Final Frequency Formula:

\[
f = v \cdot 339581.68
\]

Where:
- `f` = step frequency for motor driver
- `v` = wheel velocity in m/s
# 📘 Acceleration and Velocity Profile Logic

We define **3 motion phases** with different accelerations for left and right wheels.

Let:
- `acc_c` = center linear acceleration (m/s²)
- `acc_omega` = angular acceleration (rad/s²)
- `s` = distance between wheels (m)

---

## 🔹 Phase 1: Starting from Rest (`V_prev == 0`)

\[
acc_R = acc_c + \frac{s}{2} \cdot acc_\omega
\]
\[
acc_L = acc_c - \frac{s}{2} \cdot acc_\omega
\]

---

## 🔹 Phase 2: While Moving (`V_prev ≠ 0`)

\[
acc_R = \frac{s}{2} \cdot acc_\omega
\]
\[
acc_L = -\frac{s}{2} \cdot acc_\omega
\]

---

## 🔹 Phase 3: Stopping (`V_target == 0`)

\[
acc_R = acc_c + \frac{s}{2} \cdot acc_\omega
\]
\[
acc_L = acc_c - \frac{s}{2} \cdot acc_\omega
\]

---

## 🔄 Step-by-Step Velocity Update

Each iteration:

\[
v(t + dt) = v(t) \pm acc \cdot dt
\]

Repeat until:

\[
v(t) = v_{\text{target}}
\]
