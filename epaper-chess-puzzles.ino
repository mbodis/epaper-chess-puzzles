#include "PuzzleMain.h"
#include <Arduino.h>

/*
 * using power button from adafruit
 * using pin to power off device programatically
 */
const int POWER_OFF_PIN = 3;

void setup() {
  pinMode(POWER_OFF_PIN, OUTPUT);  // sets the digital pin as output

  Serial.begin(115200);
  delay(100);

  PuzzleMain puzzleMain;
  digitalWrite(POWER_OFF_PIN, HIGH);
}

void loop() {
  // no loop action
}
