#include "CsvParser.h"
#include <string>
using namespace std;

/*
 * - read config.txt
 *  - get number of file lines
 *  - get number of files
 * - set random filename
 * - set random line
 */
CsvParser::CsvParser() {

  // read lines, files from config.txt
  SdFat SD = Utils::initSdCard();

  File32 myFile = SD.open(this->PUZZLE_CONFIG);
  char line[400];
  if (myFile) {
    while (myFile.available()) {
      int n = myFile.fgets(line, sizeof(line));
      if (n <= 0) {
        Serial.println("[PuzzleMain] ERR: fgets failed");
      }
      if (line[n - 1] != '\n' && n == (sizeof(line) - 1)) {
        Serial.println("[PuzzleMain] ERR: line too long");
      }
      string str = line;
      int val = stoi(str);
      if (this->lines == 0) {
        this->lines = val;
        continue;
      }
      if (this->files == 0) {
        this->files = val;
        break;
      }
    }
    // close the file:
    myFile.close();
  }
  Serial.printf("[CsvParser] lines: %d \n", this->lines);
  Serial.printf("[CsvParser] files: %d \n", this->files);

  // get random file
  randomSeed(analogRead(A0));
  int randPuzzleFile = random(this->files) + 1;
  this->selectedPuzzleFile = "puzzle-" + std::to_string(randPuzzleFile) + ".csv";

  Serial.printf("[CsvParser] Reading CSV file: %s \n", this->selectedPuzzleFile.c_str());

  // get random line
  randomSeed(analogRead(A0));
  this->selectedPuzzleLine = random(this->lines) + 1;
  Serial.printf("[CsvParser] selected puzzle on line %d \n", this->selectedPuzzleLine);
}

/*
 * - read puzzle from csv
 */
bool CsvParser::getRandomPuzzle(ChessPuzzle* puzzle) {

  // init SD card
  SdFat SD = Utils::initSdCard();

  // read
  File32 myFile = SD.open(this->selectedPuzzleFile.c_str());
  int currentLine = 0;
  char line[300];
  if (myFile) {
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      currentLine++;
      int n = myFile.fgets(line, sizeof(line));
      if (currentLine != this->selectedPuzzleLine) {
        continue;
      }
      if (n <= 0) {
        Serial.println("[CsvParser] ERR: fgets failed");
      }
      if (line[n - 1] != '\n' && n == (sizeof(line) - 1)) {
        Serial.println("[CsvParser] ERR: line too long");
      }
      this->parseLine(puzzle, line);
      break;
    }
    // close the file:
    myFile.close();
    return true;
  } else {
    // if the file didn't open, print an error:
    Serial.printf("[CsvParser] ERR: error opening %s \n", this->selectedPuzzleFile.c_str());
    return false;
  }
}

/*
 * full line:
 * 00sHx,q3k1nr/1pp1nQpp/3p4/1P2p3/4P3/B1PP1b2/B5PP/5K2 b k - 0 17,e8d7 a2e6 d7d8 f7f8,1760,80,83,72,mate mateIn2 middlegame short,https://lichess.org/yyznGmXs/black#34,Italian_Game Italian_Game_Classical_Variation
 *
 * fen notation: https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 * example: rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1
 *
 * desc:
 *  1. Piece placement data
 *  2. Active color: "w" means that White is to move; "b" means that Black is to move
 *  3. Castling availability
 *  4. En passant target square
 *  5. Halfmove clock - not used
 *  6. Fullmove number - not used
 */
void CsvParser::parseLine(ChessPuzzle* puzzle, char* str) {
  str = strtok(str, ",");
  if (!str) return;
  puzzle->id = str;

  str = strtok(nullptr, ",");
  if (!str) return;
  char* fen = str;

  str = strtok(nullptr, ",");
  if (!str) return;
  puzzle->solution = str;

  fen = strtok(fen, " ");
  if (!fen) return;
  puzzle->fenNotation = fen;

  fen = strtok(nullptr, " ");
  if (!fen) return;
  puzzle->whiteIsOnTheMove = strcmp(fen, "w") == 0;

  fen = strtok(nullptr, " ");
  if (!fen) return;
  puzzle->castlingAvailability = fen;

  fen = strtok(nullptr, " ");
  if (!fen) return;
  puzzle->enPassant = fen;
}
