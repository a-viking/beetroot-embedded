#include <Arduino.h>

#define LED_BLUE 15
#define LED_RED 17

void setup() {
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
}

void loop() {
  delay(500);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, HIGH);
  delay(500);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, LOW);
}