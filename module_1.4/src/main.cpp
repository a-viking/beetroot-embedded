#include <Arduino.h>

#define BOARD_BTN 0
#define BTN_PIN 18
#define LED_BLUE 15
#define LED_RED 16

char mode = 1;

void mode1();
void mode2();

void setup() {
    Serial.begin(115200);
    pinMode(BTN_PIN, INPUT_PULLUP);
    pinMode(BOARD_BTN, INPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    Serial.println("Loading...");
    while (!Serial && millis() < 3000);
}

void loop() {
    int extBtnState = digitalRead(BTN_PIN);
    int boardBtnState = digitalRead(BOARD_BTN);

    if (extBtnState == LOW && mode != 1) {
        mode = 1;
    } else if (boardBtnState == LOW && mode != 2) {
        mode = 2;
    }

    if (mode == 1) {
        mode1();
    } else if (mode == 2) {
        mode2();
    }
}

void mode1() {
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_RED, HIGH);
    delay(200);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_RED, LOW);
    delay(200);
}

void mode2() {
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_RED, LOW);
    delay(1000);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_RED, HIGH);
    delay(1000);
}
