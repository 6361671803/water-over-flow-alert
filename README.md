# 💧 Water Overflow Alert System — Arduino Nano 33 BLE Rev2 + Edge Impulse (TinyML)

**Target board:** Arduino Nano 33 BLE Rev2 (3.3V logic)

This repository runs a TinyML model (Edge Impulse) on Arduino Nano 33 BLE Rev2 to detect `glass_full` vs `glass_not_full` using an HC-SR04 ultrasonic sensor. When the model predicts `glass_full`, an active buzzer alerts.

---

## Quick features
- Offline TinyML inference on Nano 33 BLE Rev2  
- HC-SR04 ultrasonic distance sensing  
- Active buzzer alert  
- Included dataset files for reproducibility

---

## Important hardware notes
- **Nano 33 BLE Rev2 is 3.3V logic and NOT 5V tolerant.**  
- If HC-SR04 is powered at **5V (recommended)**, **use a voltage divider or level shifter on the ECHO pin** to protect the Nano.  
- If the buzzer requires 5V or draws > 20–30 mA, drive it through an NPN transistor or N-MOSFET and share common ground.

---

## Wiring summary
- HC-SR04 VCC → 5V  
- HC-SR04 GND → GND  
- HC-SR04 TRIG → D9  
- HC-SR04 ECHO → (voltage divider) → D10  
- Buzzer + → 5V (if 5V buzzer) or D8 (if 3.3V & low current)  
- Buzzer − → transistor collector / GND  
- Arduino D8 → 1k → transistor base/gate (if using transistor)

---

## Files in this repo
- `/code/water_overflow_alert_nano33ble.ino` — Arduino sketch (replace include header with the one in your Edge Impulse ZIP)  
- `/model/edge_impulse_nano33ble_library.zip` — Edge Impulse Arduino library (download from Edge Impulse Deployment)  
- `/dataset/` — CSV training & test samples  
- `/images/` — wiring diagram + photos + model accuracy screenshot  
- `/HW_NOTES.md` — hardware safety notes (level shifting & buzzer driver)

---

## How to get the Edge Impulse library (required)
1. Go to your Edge Impulse project → **Deployment** → **Arduino Library**.  
2. Choose **Quantized (int8)** and click **Build**.  
3. Download the ZIP and upload it into `/model/edge_impulse_nano33ble_library.zip` in this repo, or import the ZIP into Arduino IDE: `Sketch → Include Library → Add .ZIP Library`.

---

## How to compile & upload
1. Install board support (Arduino Boards Manager) for **Nano 33 BLE Rev2**.  
2. Sketch → Include Library → Add .ZIP Library → select the Edge Impulse ZIP.  
3. Tools → Board → **Arduino Nano 33 BLE Rev2**. Select port.  
4. Open `/code/water_overflow_alert_nano33ble.ino`, replace the `#include` line with the exact header from the ZIP if needed, and upload.

---

#### Hardware Setup Summary

- HC-SR04 ultrasonic sensor mounted above the glass, facing downward.
- Distance 3–6 cm = FULL (buzzer ON), 8–12 cm = NOT FULL (buzzer OFF).
- TRIG → D9
- ECHO → voltage divider → D10
- Buzzer driven from D8 (direct if 3.3V buzzer, via transistor if 5V).
- Arduino Nano 33 BLE Rev2 powered via USB.
- Common ground shared between sensor, Arduino, and buzzer.

