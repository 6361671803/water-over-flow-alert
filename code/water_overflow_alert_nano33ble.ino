// water_overflow_alert_nano33ble.ino
// Target: Arduino Nano 33 BLE Rev2 (3.3V logic)
// IMPORTANT: Protect HC-SR04 ECHO if HC-SR04 is powered at 5V (voltage divider or level shifter).

#include <your_project_inferencing.h> // <-- REPLACE this with the exact header from your Edge Impulse ZIP

const int TRIG_PIN = 9;   // HC-SR04 TRIG
const int ECHO_PIN = 10;  // HC-SR04 ECHO (via voltage divider)
const int BUZZER_PIN = 8; // Buzzer driver pin (to transistor or direct if 3.3V ok)

static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return -1.0f;
  float d = (duration * 0.0343f) / 2.0f;
  return d;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("Water Overflow Alert - Nano 33 BLE Rev2");
}

void loop() {
  // collect a frame
  for (size_t i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i++) {
    float d = readDistanceCm();
    if (d < 0) d = 0.0f;
    features[i] = d;
    Serial.print("sample "); Serial.print(i); Serial.print(": "); Serial.print(d); Serial.println(" cm");
    delay(EI_CLASSIFIER_INTERVAL_MS);
  }

  signal_t signal;
  int err = numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
  if (err != 0) {
    Serial.print("signal_from_buffer failed: "); Serial.println(err);
    return;
  }

  ei_impulse_result_t result;
  EI_IMPULSE_ERROR r = run_classifier(&signal, &result, false);
  if (r != EI_IMPULSE_OK) {
    Serial.print("run_classifier failed: "); Serial.println(r);
    return;
  }

  float score_full = 0.0f;
  float score_not_full = 0.0f;
  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    const char *label = result.classification[ix].label;
    float value = result.classification[ix].value;
    Serial.print(label); Serial.print(": "); Serial.println(value, 3);
    if (strcmp(label, "glass_full") == 0) score_full = value;
    if (strcmp(label, "glass_not_full") == 0) score_not_full = value;
  }

  const float FULL_THRESHOLD = 0.80f;
  if (score_full > FULL_THRESHOLD) {
    Serial.println("GLASS FULL — BUZZER ON");
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    Serial.println("NOT FULL — BUZZER OFF");
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(500);
}
