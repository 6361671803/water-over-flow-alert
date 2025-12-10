/****************************************************
 * Water Overflow Alert - Arduino UNO + Edge Impulse
 * Sensor : HC-SR04
 * Buzzer : Active (AX buzzer)
 * FULL   : 3–6 cm  → glass_full  → buzzer ON
 * NOTFULL: 8–12 cm → glass_not_full → buzzer OFF
 ****************************************************/

// ⚠️ CHANGE THIS to your real Edge Impulse header name!
// Example: #include <water_overflow_alert_inferencing.h>
#include <your_project_inferencing.h>

const int TRIG_PIN   = 9;   // HC-SR04 TRIG
const int ECHO_PIN   = 10;  // HC-SR04 ECHO
const int BUZZER_PIN = 8;   // Active buzzer +

long echoDuration;
float distanceCm;

// EI input buffer
static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

// ---------- Read distance from HC-SR04 (cm) ----------
float readDistanceCm() {
  // Send trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo (timeout 30 ms)
  echoDuration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (echoDuration == 0) {
    return -1.0f;  // no echo
  }

  // Convert microseconds to cm
  float d = (echoDuration * 0.0343f) / 2.0f;
  return d;
}

// --------------------- SETUP ------------------------
void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("=== Water Overflow Alert (TinyML) ===");
  Serial.print("Frame size: ");
  Serial.println(EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
  Serial.print("Interval (ms): ");
  Serial.println(EI_CLASSIFIER_INTERVAL_MS);
}

// ---------------------- LOOP -----------------------
void loop() {

  // 1) Collect one frame of distance samples
  Serial.println("\nCollecting samples...");
  for (size_t i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i++) {
    float d = readDistanceCm();
    if (d < 0) d = 0.0f;        // if no reading, use 0

    features[i] = d;

    Serial.print("Sample ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(d);
    Serial.println(" cm");

    delay(EI_CLASSIFIER_INTERVAL_MS);   // must match EI frequency (e.g., 200 ms)
  }

  // 2) Wrap buffer into Edge Impulse signal
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

  // 4) Read prediction scores
  float score_full = 0.0f;
  float score_not_full = 0.0f;

  Serial.println("---- Classification ----");
  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    const char *label = result.classification[ix].label;
    float value = result.classification[ix].value;

    Serial.print(label);
    Serial.print(": ");
    Serial.println(value, 3);

    // ⚠️ Make sure these match your labels EXACTLY in Edge Impulse
    if (strcmp(label, "glass_full") == 0) {
      score_full = value;
    }
    else if (strcmp(label, "glass_not_full") == 0) {
      score_not_full = value;
    }
  }

  // 5) Decide FULL / NOT FULL and control buzzer
  const float FULL_THRESHOLD = 0.80f;   // 80% confidence

  if (score_full > FULL_THRESHOLD) {
    Serial.println("RESULT: GLASS FULL → BUZZER ON");
    digitalWrite(BUZZER_PIN, HIGH);   // active buzzer beeps
  } else {
    Serial.println("RESULT: NOT FULL → BUZZER OFF");
    digitalWrite(BUZZER_PIN, LOW);
  }

  Serial.println("------------------------");
  delay(500);   // short pause before next cycle
}
