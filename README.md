
# 📘 **README.md — Water Overflow Alert System (Arduino Nano 33 BLE Rev2 + Edge Impulse)**

```markdown
# 💧 Water Overflow Alert System  
**Arduino Nano 33 BLE Rev2 + HC-SR04 Ultrasonic Sensor + Edge Impulse (TinyML)**

This project detects **when a glass or small tank is FULL** using an ultrasonic distance sensor (HC-SR04) and a TinyML model trained in **Edge Impulse**.  
When the water reaches the FULL level (detected using ML), a **buzzer turns ON**.

This README represents the **final Overflow Detection System**, based entirely on **your dataset**.

---

# 🚀 Project Summary

- Ultrasonic sensor continuously measures distance to water surface.
- Distances are fed into a TinyML model running **on-device** on the Arduino Nano 33 BLE Rev2.
- The model classifies:
  - **glass_full** → Water level HIGH (3–6 cm)
  - **glass_not_full** → Water level LOW (8–12 cm)
- Buzzer turns ON only when **glass_full** is predicted.

Entire system works **offline**, no WiFi needed.

---

# 📂 Repository Structure

```

WaterOverflowAI/
│
├── README.md
├── HW_NOTES.md
├── .gitignore
│
├── code/
│   └── water_overflow_alert_nano33ble.ino
│
├── model/
│   └── edge_impulse_nano33ble_library.zip
│
├── dataset/
│   ├── full/*.csv
│   └── not_full/*.csv
│
└── images/
├── wiring_nano33ble.png
├── hardware_setup.png
├── sensor_placement.png
└── system_block_diagram.png

```

---

# 🧪 Dataset Explanation (VERY IMPORTANT)

Your dataset contains distance readings from HC-SR04:

| Condition | Distance | Label |
|----------|----------|--------|
| **FULL** | **3–6 cm** | `glass_full` |
| **NOT FULL** | **8–12 cm** | `glass_not_full` |

This makes the model extremely accurate because the distances **do not overlap**.

➡️ Dataset is perfectly suited for **Overflow Detection**, not underflow.

---

# 🎯 Working Principle

1. HC-SR04 measures water distance.
2. The Arduino collects multiple distance samples.
3. These samples go into the **Edge Impulse ML model**.
4. If model output = **glass_full**, the buzzer turns ON.
5. Otherwise, buzzer stays OFF.

---

# 🧱 Hardware Setup

### Components
- Arduino Nano 33 BLE Rev2 (3.3V logic)
- HC-SR04 ultrasonic sensor
- Active buzzer (5V or 3.3V)
- NPN transistor (for 5V buzzer)
- Voltage divider (10kΩ + 4.7kΩ)
- Breadboard + jumper wires
- Water glass/cup

---

# 🔌 Wiring Diagram (Nano 33 BLE Rev2)

### Ultrasonic Sensor
| HC-SR04 Pin | Arduino Pin |
|-------------|-------------|
| VCC | 5V |
| GND | GND |
| TRIG | D9 |
| ECHO | **D10 (through voltage divider)** |

### Voltage Divider (protects 3.3V Arduino)
```

ECHO(5V) --- 10kΩ ---●--- 4.7kΩ --- GND
|
D10

```

### Buzzer (5V type with transistor)
```

D8 → 1kΩ → Base of NPN
Emitter → GND
Collector → Buzzer –
Buzzer + → 5V

````

Images for GitHub:
- wiring diagram → `/images/wiring_nano33ble.png`
- setup photo → `/images/hardware_setup.png`

---

# 📐 Sensor Placement

- HC-SR04 is fixed above the glass, pointing downward.
- When water rises, distance decreases.
- Dataset defines:
  - **3–6 cm = FULL**
  - **8–12 cm = NOT FULL**

See `/images/sensor_placement.png`.

---

# 🧠 Machine Learning Model (Edge Impulse)

### Impulse Design Used
- **Input**: Ultrasonic distance readings  
- **DSP block**: Raw data (no filters)  
- **Classifier**: Dense Neural Network (quantized int8)  
- **Labels**: `glass_full`, `glass_not_full`

### Deployment
1. Edge Impulse → **Deployment**  
2. Select **Arduino Library**  
3. Build → download ZIP  
4. Add ZIP to Arduino IDE  
5. Include header:  
   ```cpp
   #include <your_project_inferencing.h>
````

---

# 🎵 Buzzer Logic

```text
IF prediction = glass_full AND confidence > 0.8
    buzzer = ON
ELSE
    buzzer = OFF
```

---

# 🌍 Real-World Applications

### ✔ Household water overflow alert

Stops water wastage by alerting when overhead tanks are full.

### ✔ Automated filling systems

Useful for beverage machines, laboratory liquids, etc.

### ✔ Smart home appliances

Prevents boiling water or milk overflowing.

### ✔ Industrial container monitoring

Detects liquid levels with a non-contact sensor.

### ✔ Education & TinyML demonstration

Shows how AI can run on microcontrollers for real-world sensing.

---

# ⚠️ Limitations

### 1️⃣ Sensor accuracy depends on surface condition

Foam, bubbles, or tilted sensor may cause wrong readings.

### 2️⃣ Fixed height calibration

Model works correctly **only** when:

* Sensor is at same height
* Same distance ranges (3–6 full, 8–12 not full)

Changing the container requires retraining.

### 3️⃣ Requires clean line-of-sight

Ultrasonic sensor fails if objects block the path.

### 4️⃣ Cannot detect water color, type, or movement

Only distance is measured.

### 5️⃣ Buzzer-only alert

No IoT notifications unless added manually.

---

# 🔧 How to Run the Project

1. Upload code from `/code/water_overflow_alert_nano33ble.ino`
2. Add the Edge Impulse ZIP library to Arduino IDE
3. Power Nano 33 BLE Rev2
4. Open Serial Monitor
5. Fill the glass with water

✔ When water reaches 3–6 cm distance → BUZZER ON
✔ When water is 8–12 cm → BUZZER OFF

---

# 📸 Images Used

Place these in `/images/`:

* wiring_nano33ble.png
* hardware_setup.png
* sensor_placement.png
* system_block_diagram.png

---

# 📝 License

MIT License (optional)

---


---

