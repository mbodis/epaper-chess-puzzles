#include "Utils.h"

SdFat Utils::initSdCard() {
  SdFat SD;
  SdSpiConfig config(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(16), &SPI1);
  // Retry mechanism for SD card initialization
  while (!SD.begin(config)) {
    Serial.println("[Utils] ERR: initialization failed! Retrying...");
    delay(1000);  // Wait for a second before retrying
  }
  return SD;
}
