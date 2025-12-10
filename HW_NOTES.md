# Hardware Notes — Arduino UNO R3 (5V)

## HC-SR04 wiring (UNO)
- VCC -> 5V
- GND -> GND
- TRIG -> D9
- ECHO -> D10

## Buzzer wiring
If buzzer draws low current and works at 3.3–5V:
- D8 -> Buzzer + (or use 5V if buzzer is active and uses more current)
- Buzzer - -> GND

If the buzzer is 5V and draws more current:
- Use an NPN transistor:
  - Buzzer + -> 5V
  - Buzzer - -> Collector
  - Emitter -> GND
  - D8 -> 1kΩ -> Base
- Tie common ground between Arduino and buzzer supply.

## Power & grounding
- USB supply is usually sufficient for UNO + HC-SR04 + small buzzer.
- Use external power if additional components are added.

## Safety
- Avoid powering HC-SR04 at 3.3V unless you have tested the module. Most reliable on 5V with UNO.
