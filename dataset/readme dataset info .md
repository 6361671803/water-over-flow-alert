
---

# 🧪 Dataset Explanation (VERY IMPORTANT)

Your dataset contains distance readings from HC-SR04:

| Condition     | Distance  | Label              |
|---------------|-----------|--------------------|
| **FULL**      | **3–6 cm**| `glass_full`       |
| **NOT FULL**  | **8–12 cm**| `glass_not_full`  |

This clear separation (3–6 cm vs 8–12 cm) makes the model easy to classify and gives high accuracy.  
➡️ Dataset is perfectly suited for **Overflow Detection**, not underflow.

**DO NOT** change or rename the CSVs — they must match the files used to train the model.

---

# 🎯 Working Principle

1. HC-SR04 measures the distance to the water surface.  
2. Arduino collects a frame of distance samples (as required by the Edge Impulse impulse).  
3. The frame is fed to the Edge Impulse model (running on the UNO).  
4. If model output = **glass_full** with confidence > threshold, buzzer = ON.  
5. Otherwise, buzzer = OFF.

---

# 🧱 Hardware Setup

### Components
- **Arduino UNO R3** (5V logic)  
- **HC-SR04** ultrasonic sensor  
- **Active buzzer** (5V recommended) OR buzzer + NPN transistor (if high current)  
- **Breadboard + jumper wires**  
- **Glass / cup / small tank**  

---

# 🔌 Wiring Diagram (Arduino UNO R3)

### Ultrasonic Sensor
| HC-SR04 Pin | Arduino UNO Pin |
|-------------|------------------|
| VCC         | 5V               |
| GND         | GND              |
| TRIG        | D9               |
| ECHO        | D10              |

### Buzzer (direct or via transistor)
**Direct (low-current, small buzzer):**
| Buzzer + | Arduino UNO Pin |
|----------|------------------|
| +        | D8               |
| -        | GND              |

**With transistor (recommended for 5V high-current buzzers):**
