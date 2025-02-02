#include "PuzzleMain.h"

PuzzleMain::PuzzleMain() {
  Serial.println("[PuzzleMain] -- start --");

  SD = Utils::initSdCard();

  if (this->hasSolutionFile()) {
    this->readSolution();
    this->puzzle.drawSolution();
    this->deleteFileWithSolution();
  } else {
    CsvParser parser;
    if (parser.getRandomPuzzle(&this->puzzle)) {
      this->puzzle.init();
      this->puzzle.print();
      this->puzzle.drawChessboard();
      this->createFileWithSolution();
    }
  }

  Serial.println("[PuzzleMain] -- end --");
}

bool PuzzleMain::hasSolutionFile() {
  bool exists = SD.exists(this->SOLUTION_FILE);
  Serial.printf("[PuzzleMain] %s \n", exists ? "draw solution" : "draw new puzzle");
  return exists;
}

void PuzzleMain::readSolution() {
  Serial.println("[PuzzleMain] readSolution");
  // TODO need to re init SD card ?!?
  SdFat SD = Utils::initSdCard();

  File32 myFile = SD.open(this->SOLUTION_FILE);
  char line[300];
  if (myFile) {
    while (myFile.available()) {
      int n = myFile.fgets(line, sizeof(line));
      if (n <= 0) {
        Serial.println("[PuzzleMain] ERR: fgets failed");
      }
      if (line[n - 1] != '\n' && n == (sizeof(line) - 1)) {
        Serial.println("[PuzzleMain] ERR: line too long");
      }
      this->puzzle.solution = line;
      break;
    }
    // close the file:
    myFile.close();
  }
}

void PuzzleMain::deleteFileWithSolution() {
  bool deleted = false;
  if (SD.exists(this->SOLUTION_FILE)) {
    deleted = SD.remove(this->SOLUTION_FILE);
  }
  Serial.printf("[PuzzleMain] solution file %s \n", deleted ? "deleted" : "not deleted");
}

void PuzzleMain::createFileWithSolution() {
  File32 myFile = SD.open(this->SOLUTION_FILE, FILE_WRITE);
  myFile.print(this->puzzle.solution);
  bool success = myFile.close();
  Serial.printf("[PuzzleMain] solution: %s \n", this->puzzle.solution);
  Serial.printf("[PuzzleMain] createFileWithSolution %s \n", success ? "success" : "failed");
}
