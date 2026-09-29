#include <Arduino.h>

#define VT_PIN 15
#define R_PIN 17

#define ADC_BITS 12
#define SAMPLE_DELAY_MS 100
#define LIGHT_THRESHOLD 2800
#define DARK_THRESHOLD 1400

void setup() {
  Serial.begin(115200);
  pinMode(VT_PIN, OUTPUT);
  analogReadResolution(ADC_BITS);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  int raw = analogRead(R_PIN);
  Serial.print("raw = ");
  Serial.println(raw);

  if (raw > LIGHT_THRESHOLD) {
    digitalWrite(VT_PIN, LOW);
  } else if (raw < DARK_THRESHOLD) {
    digitalWrite(VT_PIN, HIGH);
  }

  delay(SAMPLE_DELAY_MS);
}