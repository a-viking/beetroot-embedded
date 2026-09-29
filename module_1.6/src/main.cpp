#include <Arduino.h>

#define R_PIN 17

#define ADC_BITS 12
#define SAMPLE_DELAY_MS 100

void setup() {
  Serial.begin(115200);
  analogReadResolution(ADC_BITS);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  int raw = analogRead(R_PIN);
  int mVread = analogReadMilliVolts(R_PIN);
  int calculated = ((float)raw / 4095) * 3100;
  float e = abs(((calculated - mVread) * 100) / (float)calculated);

  Serial.println("======================");
  Serial.print("raw \t\t");
  Serial.println(raw);

  Serial.print("calculated \t");
  Serial.println(calculated);

  Serial.print("mVread \t\t");
  Serial.println(mVread);

  Serial.print("rel error \t");
  Serial.println(e);
  Serial.println("======================");

  delay(SAMPLE_DELAY_MS);
}