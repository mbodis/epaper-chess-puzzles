#include "PuzzleMain.h"
#include "CsvParser.h"
#include <string.h>
#include <stdlib.h>

PuzzleMain::PuzzleMain() {
  Serial.println("[PuzzleMain] -- start --");

  SD = Utils::initSdCard();

  if (this->hasSolutionFile()) {
    this->readSolution();
    this->puzzle.init();
    this->puzzle.printToLog();
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
  Serial.printf("[PuzzleMain] %s \n", exists ? "draw solution" : "draw new puzzle");
  return exists;
}

void PuzzleMain::readSolution() {
  Serial.println("[PuzzleMain] readSolution");
  // TODO need to re init SD card ?!?
  SdFat SD = Utils::initSdCard();

  File32 myFile = SD.open(this->SOLUTION_FILE);
  if (!myFile) {
    Serial.println("[PuzzleMain] ERR: cannot open solution file");
    return;
  }

  if (!myFile.available()) {
    Serial.println("[PuzzleMain] ERR: solution file empty");
    myFile.close();
    return;
  }

  int n = myFile.fgets(this->solutionLine, sizeof(this->solutionLine));
  myFile.close();
  if (n <= 0) {
    Serial.println("[PuzzleMain] ERR: fgets failed");
    return;
  }
  if (this->solutionLine[n - 1] != '\n' && n == (int)(sizeof(this->solutionLine) - 1)) {
    Serial.println("[PuzzleMain] ERR: line too long");
  }
  // Strip trailing newline
  if (n > 0 && this->solutionLine[n - 1] == '\n') {
    this->solutionLine[n - 1] = '\0';
  }

  // Snapshot the line first because CsvParser::parseLine mutates it via
  // strtok and we still need to extract the trailing puzzleCounter field.
  char counterCopy[sizeof(this->solutionLine)];
  strncpy(counterCopy, this->solutionLine, sizeof(counterCopy));
  counterCopy[sizeof(counterCopy) - 1] = '\0';

  CsvParser::parseLine(&this->puzzle, this->solutionLine);

  // The saved line has the format: id,FEN,solution,puzzleCounter
  // Extract the 4th comma-separated field.
  char *tok = strtok(counterCopy, ",");
  int idx = 0;
  this->puzzle.puzzleCounter = 1;  // safe default (non-zero, not multiple of 1000)
  while (tok != nullptr) {
    if (idx == 3) {
      this->puzzle.puzzleCounter = atol(tok);
      break;
    }
    tok = strtok(nullptr, ",");
    idx++;
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
  // Persist full puzzle state in CSV-compatible format so that on the next
  // power-on (when only the solution file is present) drawSolution() can
  // rebuild the chessboard from FEN.
  //
  // Line format (compatible with CsvParser::parseLine):
  //   id,<placement> <w|b> <castling> <enpassant> 0 1,solution,puzzleCounter
  //
  // The "0 1" are FEN halfmove clock + fullmove number placeholders -
  // CsvParser::parseLine ignores those two fields.
  File32 myFile = SD.open(this->SOLUTION_FILE, FILE_WRITE);
  myFile.print(this->puzzle.id);
  myFile.print(",");
  myFile.print(this->puzzle.fenNotation);
  myFile.print(" ");
  myFile.print(this->puzzle.whiteIsOnTheMove ? "w" : "b");
  myFile.print(" ");
  myFile.print(this->puzzle.castlingAvailability);
  myFile.print(" ");
  myFile.print(this->puzzle.enPassant);
  myFile.print(" 0 1,");
  myFile.print(this->puzzle.solution);
  myFile.print(",");
  myFile.print(this->puzzle.puzzleCounter);
  myFile.print("\n");
  bool success = myFile.close();
  Serial.printf("[PuzzleMain] solution: %s \n", this->puzzle.solution);
  Serial.printf("[PuzzleMain] createFileWithSolution %s \n", success ? "success" : "failed");
}
