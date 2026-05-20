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
  char *PUZZLE_COUNTER = (char *)"counter.txt";

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

  /*
   * puzzle we are going to display
   * one number stored in counter.txt (max lines * files)
   */
  long nextPuzzle = 0;

  void parseLine(ChessPuzzle *, char *);

public:
  CsvParser();
  bool getPuzzle(ChessPuzzle *);
  void createFilePuzzleCounter();
  void deleteFilePuzzleCounter();
};

#endif  // CSVPARSER_H