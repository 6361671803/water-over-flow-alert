/***************************************************
 * Water Overflow Alert - TinyML + Arduino Uno R3
 * Sensor: HC-SR04
 * Model: Edge Impulse (glass_full vs glass_not_full)
 * FULL distance (trained): ~3–6 cm
 ***************************************************/

// ⚠️ Replace this include with the one from your EI example sketch!
#include <your_project_inferencing.h> 
// Example: #include <water_overflow_glass_inferencing.h>

// Pin definitions (Adjust only if you wired differently)
const int TRIG_PIN   = 9;   // HC-SR04 TRIG
const int ECHO_PIN   = 10;  // HC-SR04 ECHO
const int BUZZER_PIN = 8;   // Buzzer + (– goes to GND)

// Globals
long duration_us;
float distance_cm;
static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

// Measure distance in cm using HC-SR04
float readDistanceCm() {
  // Trigger low
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Trigger high for 10µs
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo (timeout: 30ms)
  duration_us = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration_us == 0) {
    // No echo received (too far or error)
    return -1.0f;
  }

  // Convert microseconds to distance in cm
  float d = (duration_us * 0.0343f) / 2.0f;
  return d;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("=== Water Overflow Alert (Edge Impulse + Arduino UNO) ===");
  Serial.print("Frame size: ");
  Serial.println(EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
  Serial.print("Interval (ms): ");
  Serial.println(EI_CLASSIFIER_INTERVAL_MS);
}

void loop() {
  // 1) Collect one full frame of samples
  Serial.println("\nCollecting samples...");
  for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix++) {
    float d = readDistanceCm();
    if (d < 0) {
      // If no reading, just store 0
      d = 0.0f;
    }

    features[ix] = d;

    Serial.print("Sample ");
    Serial.print(ix);
    Serial.print(": ");
    Serial.print(d);
    Serial.println(" cm");

    // Match the frequency used in Edge Impulse (usually 5 Hz → 200 ms)
    delay(EI_CLASSIFIER_INTERVAL_MS);
  }

  // 2) Build Edge Impulse signal from buffer
  signal_t signal;
  int err = numpy::signal_from_buffer(
      features,
      EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE,
      &signal);
  if (err != 0) {
    Serial.print("signal_from_buffer failed: ");
    Serial.println(err);
    return;
  }

  // 3) Run classifier
  ei_impulse_result_t result;
  EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);
  if (res != EI_IMPULSE_OK) {
    Serial.print("run_classifier failed: ");
    Serial.println(res);
    return;
  }

  // 4) Read classification results
  float score_full = 0.0f;
  float score_not_full = 0.0f;

  Serial.println("---- Classification result ----");
  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    const char *label = result.classification[ix].label;
    float value = result.classification[ix].value;

    Serial.print(label);
    Serial.print(": ");
    Serial.println(value, 3);

    // ⚠️ Make sure these label strings match exactly
    if (strcmp(label, "glass_full") == 0) {
      score_full = value;
    } else if (strcmp(label, "glass_not_full") == 0) {
      score_not_full = value;
    }
  }

  // 5) Decide if FULL → control buzzer
  const float FULL_THRESHOLD = 0.80f; // 80% confidence

  if (score_full > FULL_THRESHOLD) {
    Serial.println("GLASS FULL → BUZZER ON");
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    Serial.println("NOT FULL → BUZZER OFF");
    digitalWrite(BUZZER_PIN, LOW);
  }

  Serial.println("-------------------------------");
  delay(500); // Short pause before next cycle
}
