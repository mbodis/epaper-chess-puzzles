
STEPS epaper chess puzzles:
1) install arduino IDE https://www.arduino.cc/
2) install board https://learn.adafruit.com/adafruit-feather-rp2040-adalogger/arduino (version 4.4.1)
3) install library for sd card: https://learn.adafruit.com/adafruit-feather-rp2040-adalogger/sd-card-2   
  - note: 32 GB not working, 8GB working correctly
4) download epaper library: https://www.waveshare.com/wiki/E-Paper_ESP8266_Driver_Board
  - copy into ~/Arduino/libraries
  - update DEV_Config.h
    // GPIO config Adalogger RP2040 with 4.2 waveshare epaper
    #define EPD_SCK_PIN  14
    #define EPD_MOSI_PIN 15
    #define EPD_CS_PIN   9
    #define EPD_RST_PIN  11
    #define EPD_DC_PIN   27
    #define EPD_BUSY_PIN 2
5) connect by attached scheme (pic #1)
6) select proper board and port in Arduino IDE
7) upload and enjoy

SD card:
there 


TODO schema
TODO CSV not described





