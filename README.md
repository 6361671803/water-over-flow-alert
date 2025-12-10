# 💧 Water Overflow Alert System — Arduino Nano 33 BLE Rev2 + Edge Impulse (TinyML)


**Target board:** Arduino Nano 33 BLE Rev2 (3.3V logic)


This repository contains a TinyML-based water level detection project that classifies *glass_full* and *glass_not_full* using an ultrasonic sensor (HC-SR04) and an Edge Impulse model running on an Arduino Nano 33 BLE Rev2. When the model predicts `glass_full`, the system turns a buzzer on to alert.


## Quick features
- Offline TinyML inference on Nano 33 BLE Rev2
- HC-SR04 ultrasonic distance sensing
- Active buzzer alert
- Datasets included for reproducibility


## Important hardware notes
- **Nano 33 BLE Rev2 is 3.3V logic and NOT 5V tolerant.**
- If HC-SR04 is powered at 5V (recommended), **use a voltage divider or level shifter on the ECHO pin** to protect the Nano.
- If the buzzer requires 5V or more current, **drive it through an NPN transistor or logic MOSFET** and share ground.


## Wiring (summary)
- HC-SR04 VCC → 5V (or 3.3V if you know yours is stable at 3.3V)
- HC-SR04 GND → GND
- HC-SR04 TRIG → D9
- HC-SR04 ECHO → (via voltage divider) → D10
- Buzzer + → 5V (if 5V buzzer) or D8 (if 3.3V and low current)
- Buzzer - → transistor collector / GND
- Arduino pin D8 → base/gate driver resistor → transistor base/gate

## How to build the Edge Impulse library (required)
1. Open your Edge Impulse project → **Deployment** → **Arduino Library**.
2. Choose **Quantized (int8)** and click **Build**.
3. Download the ZIP and save as `/model/edge_impulse_nano33ble_library.zip` in this repo or instruct users to import it in Arduino IDE.

## Test & Run
1. Install Nano 33 BLE board support in Arduino IDE.
2. Sketch → Include Library → Add .ZIP Library → select `edge_impulse_nano33ble_library.zip`.
3. Select Tools → Board → Arduino Nano 33 BLE Rev2 → upload the `/code/*.ino` sketch.
4. Open Serial Monitor @ 115200 to see sampling and inference logs.


---
