#include "PuzzleMain.h"
#include <Arduino.h>

/*
 * using power button from adafruit
 * using pin to power off device programatically
 */
const int POWER_OFF_PIN = 3;

void setup() {
  pinMode(POWER_OFF_PIN, OUTPUT);  // sets the digital pin as output

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  
  // TODO enable for logging
  // while (!Serial) {
  //   yield();
  //   delay(10);
  // }            // wait till serial port is opened

  delay(100);  // RP2040 delay is not a bad idea

  PuzzleMain puzzleMain;
  digitalWrite(POWER_OFF_PIN, HIGH);
}

void loop() {
  // no loop action
}
