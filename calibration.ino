#define NUM_SENSORS 5
int sensorPins[NUM_SENSORS] = {A1, A2, A3, A4, A5};
int minValues[NUM_SENSORS];
int maxValues[NUM_SENSORS];
int threshold[NUM_SENSORS];

void setup() {
  Serial.begin(9600);
  delay(1000); // Allow system to stabilize
  calibrateSensors(); // Run calibration once
}

void loop() {
  // Example: print calibrated readings for verification
  for (int i = 0; i < NUM_SENSORS; i++) {
    int val = analogRead(sensorPins[i]);
    Serial.print(val);
    Serial.print("\t");
  }
  Serial.println();
  delay(200);
}

/* ---------------- Calibration Routine ---------------- */

void calibrateSensors() {
  Serial.println("Starting calibration...");
  delay(1000);

  // Initialize min and max values
  for (int i = 0; i < NUM_SENSORS; i++) {
    minValues[i] = 1023;
    maxValues[i] = 0;
  }

  // Move robot manually over black and white surfaces
  // for around 5–10 seconds during this calibration period
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {  // 5 seconds calibration
    for (int i = 0; i < NUM_SENSORS; i++) {
      int val = analogRead(sensorPins[i]);
      if (val < minValues[i]) minValues[i] = val;
      if (val > maxValues[i]) maxValues[i] = val;
    }
  }

  // Compute thresholds
  for (int i = 0; i < NUM_SENSORS; i++) {
    threshold[i] = (minValues[i] + maxValues[i]) / 2;
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(": min=");
    Serial.print(minValues[i]);
    Serial.print(", max=");
    Serial.print(maxValues[i]);
    Serial.print(", threshold=");
    Serial.println(threshold[i]);
  }

  Serial.println("Calibration complete!");
}
