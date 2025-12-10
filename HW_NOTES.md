# 🔧 Hardware Setup — Water Overflow Alert System  
**Arduino UNO R3 + HC-SR04 Ultrasonic Sensor + Active Buzzer**

This document explains **all hardware connections**, **power notes**, **sensor placement**, and **operational considerations** for the Water Overflow Detection project using TinyML and Edge Impulse.

---

# 🧩 1. Components Required

### Core Components
- **Arduino UNO R3 (5V logic)**
- **HC-SR04 Ultrasonic Distance Sensor**
- **Active Buzzer (5V recommended)**  
- **Breadboard & Jumper Wires**

### Optional Components (recommended for clean operation)
- **NPN Transistor (2N2222)** → for driving loud buzzers  
- **1kΩ Resistor** → for transistor base  
- **USB Cable for UNO**  
- **Water Glass / Cup / Container**  

---

# ⚡ 2. Electrical Characteristics

### Arduino UNO R3
- Operating Voltage: **5V**
- Digital I/O max current: **20–30 mA**
- Logic level: **5V tolerant on all pins**

### HC-SR04
- Operating Voltage: **5V**
- TRIG logic: 5V  
- ECHO logic: returns 5V  
- NO level shifting required for Arduino UNO  

### Active Buzzer
- If low-current (10–20 mA) → direct pin drive  
- If high-current → MUST use NPN transistor  

---

# 🛠 3. Wiring Connections

## **HC-SR04 → Arduino UNO**

| HC-SR04 Pin | Arduino UNO Pin |
|-------------|------------------|
| VCC         | 5V               |
| GND         | GND              |
| TRIG        | **D9**           |
| ECHO        | **D10**          |

- No voltage divider needed (UNO uses 5V logic).

---

## **Buzzer → Arduino UNO**

### 🔹 **Option A – Direct Wiring (small buzzer)**  
| Buzzer Pin | Arduino UNO |
|------------|-------------|
| +          | **D8**      |
| –          | GND         |

Use this only if your buzzer is **low current**.

### 🔹 **Option B – Transistor Wiring (recommended)**
Arduino D8 → 1kΩ → Base (2N2222)
Emitter → GND
Collector → Buzzer –
Buzzer + → 5V

Advantages:
- Protects Arduino pin  
- Allows loud buzzers  
- More reliable triggering  

---

# 📏 4. Sensor Placement (VERY IMPORTANT)

The ultrasonic sensor must be placed:

- **Facing downward toward water surface**  
- **At a fixed vertical height above the glass**

Your dataset defines:

| Water Status | Distance (cm) |
|--------------|----------------|
| **Full**     | **3–6 cm**     |
| **Not Full** | **8–12 cm**    |

Therefore:

### ✔ Sensor must be mounted **above the glass** so that:
- When water **touches the full line**, distance = **3–6 cm**
- When water **is below half**, distance = **8–12 cm**

If you change the sensor height or glass shape →  
**Your dataset will no longer match → Model accuracy drops.**

---

# 🔄 5. Power Setup

### UNO Power Options:
- USB power from laptop (recommended)
- 5V adapter (phone charger)

HC-SR04 + buzzer + UNO USB power = works fine.

**Important:**  
All components must share **common ground**.

---

# 🧠 6. Hardware Flow Summary
HC-SR04 → Distance Samples → Arduino UNO → Edge Impulse Model → Decision:
| |
| └──▶ glass_full → Buzzer ON
└────▶ glass_not_full → Buzzer OFF


---

# 🧪 7. Testing & Validation

Keep glass filled gradually and check:

- Buzzer should **remain OFF** until distance <= **6 cm**
- When water reaches 3–6 cm → buzzer **turns ON**
- Move hand near sensor → model will classify incorrectly (normal)

Use Serial Monitor @ **115200 baud** for:
- Live distance readings  
- ML prediction scores  

---

# ⚠️ 8. Limitations

- Ultrasonic sensors struggle with splashes or foam.  
- Must keep sensor clean and stable.  
- Changing the angle or height breaks dataset accuracy.  
- Not suitable for very small glasses (<6 cm depth).  
- Must avoid vibrating surfaces (affects readings).

---

# 🌍 9. Real-World Uses

- Home tank overflow prevention  
- School/college TinyML demonstration  
- Non-contact water level sensing  
- Stop motor before overflow in tanks  
- Smart kitchen systems  

---

