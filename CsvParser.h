#ifndef CSVPARSER_H
#define CSVPARSER_H

#include "ChessPuzzle.h"
#include <Arduino.h>
#include "Utils.h"
using namespace std;
#include <string>

class CsvParser {
private:
  char *PUZZLE_CONFIG = (char *)"config.txt";

  string selectedPuzzleFile;
  int selectedPuzzleLine = 0;

  /*
   * we are always expecting that there will be exactly same puzzles per file
   * putting more puzzle can slow down the process
   * number of lines is stored in config.txt
   */
  long lines = 0;

  /*
   * number of files with puzzles
   * puzzle-1.csv, puzzle-2.csv ...
   * number of lines is stored in config.txt
   */
  long files = 0;

  void parseLine(ChessPuzzle *, char *);

public:
  CsvParser();
  bool getRandomPuzzle(ChessPuzzle *);
};

#endif  // CSVPARSER_H