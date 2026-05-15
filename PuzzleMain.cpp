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
    if (parser.getPuzzle(&this->puzzle)) {
      this->puzzle.init();
      this->puzzle.printToLog();
      this->puzzle.drawChessboard();
      if (!this->puzzle.isMessage){
        this->createFileWithSolution();
      }
    }
  }

  Serial.println("[PuzzleMain] -- end --");
}

bool PuzzleMain::hasSolutionFile() {
  bool exists = SD.exists(this->SOLUTION_FILE);
  Serial.print("[PuzzleMain] ");
  Serial.println(exists ? "draw solution" : "draw new puzzle");
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
  Serial.print("[PuzzleMain] solution file ");
  Serial.println(deleted ? "deleted" : "not deleted");
}

void PuzzleMain::createFileWithSolution() {
  File32 myFile = SD.open(this->SOLUTION_FILE, FILE_WRITE);
  myFile.print(this->puzzle.solution);
  bool success = myFile.close();
  Serial.print("[PuzzleMain] solution: ");
  Serial.println(this->puzzle.solution);
  Serial.print("[PuzzleMain] createFileWithSolution ");
  Serial.println(success ? "success" : "failed");
}
