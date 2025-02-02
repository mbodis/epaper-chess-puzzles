#ifndef UTILS_H
#define UTILS_H

// sd card includes
#include <SPI.h>
#include "SdFat.h"
#define SD_CS_PIN 23

class Utils {
private:
public:
  static SdFat initSdCard();
};

#endif  // UTILS_H