
## Chess Puzzle Box with E-Paper Display
This project is a compact, battery-efficient chess puzzle box that uses an e-paper display and a push-button power switch for minimal power consumption. Puzzles are sourced from the Lichess puzzle database.

### Usage:

* First click: Shows a chess puzzle.
* Second click: Reveals the solution.

Perfect for practicing chess tactics on the go, with long battery life thanks to the low-power design.


<img src="./prototype.png" width="500" alt="image of chess box"/>

## hardware setup

---
- 3.7 LiPo battery 
- 4.2 epaper wave share: https://www.waveshare.com/4.2inch-e-paper-module.htm
- adafruit logger RP2040: https://learn.adafruit.com/adafruit-feather-rp2040-adalogger/overview
- adafruit push-button power switch: https://www.adafruit.com/product/1400
- jump wires

## setup:

---
1. install arduino IDE https://www.arduino.cc/
2. install RP2040 board https://learn.adafruit.com/adafruit-feather-rp2040-adalogger/arduino (version 4.4.1)
3. install library for sd card: https://learn.adafruit.com/adafruit-feather-rp2040-adalogger/sd-card-2   
   - (note: 32 GB not working, 8GB working correctly)
4. setup epaper library (two options):

   **Option 1: Use included library (recommended)**
   - library supports **BOTH** 4.2" e-paper V1 and V2 displays
   - create symlink: `ln -s /path/to/repo/lib ~/Documents/Arduino/libraries/EPD4in2`
   - GPIO config is already set in `lib/DEV_Config.h` for Adalogger RP2040
   - to use V2 display, uncomment `#define USE_EPAPER_V2` in `ChessPuzzle.h`

   **Option 2: Download manually**
   - library supports **ONLY** 4.2" e-paper V1 displays
   - download from: https://www.waveshare.com/wiki/E-Paper_ESP8266_Driver_Board
   - copy into `~/Documents/Arduino/libraries/EPD4in2`
   - update `DEV_Config.h` to match board:
     ```
     // GPIO config Adalogger RP2040 with 4.2 waveshare epaper
     #define EPD_SCK_PIN  14
     #define EPD_MOSI_PIN 15
     #define EPD_CS_PIN   9
     #define EPD_RST_PIN  11
     #define EPD_DC_PIN   27
     #define EPD_BUSY_PIN 2
     ```
5. wire by attached scheme
6. generate puzzles and config by running `generate-puzzles.sh` (this will create puzzles and config in sd-card folder)
   - copy content of sd-card folder into memory card (root folder)
7. select proper board and port in Arduino IDE
8. upload and enjoy

## SD card:

---
  - should contain `config.txt` file with two numbers. First is the number of lines. Second number of files.
  - should contain `counter.txt` file with one number. For beginnig it should be set to zero. Keep position in puzzles.
  - list of puzzles accordingly to config files puzzle-1.csv, puzzle-2.csv ... 
  - `solution.txt` is created automatically by the device after a puzzle is shown. It holds the current puzzle (in CSV format compatible with `puzzle-*.csv`) so that the next power-on can render the solution view. It is removed automatically after the solution is shown.

## how it works (two-boot flow):

---
The push-button power switch fully cuts power between clicks, so the device boots from scratch on every press:

1. **Boot 1 — puzzle**: no `solution.txt` on the SD card. The device reads the next puzzle from `puzzle-*.csv`, renders the chessboard, then writes the current puzzle data (id, FEN, solution, counter) into `solution.txt` and powers off.
2. **Boot 2 — solution**: `solution.txt` exists. The device reloads the puzzle data, re-renders the chessboard from the FEN as the "previous" frame, overlays the solution text, performs a partial refresh, deletes `solution.txt` and powers off.

## schema:

---
<img src="./scheme-chesspuzzles.png" width="500" alt="image of schema"/>

