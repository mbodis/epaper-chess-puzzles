#ifndef PUZZLEMAIN_H
#define PUZZLEMAIN_H

#include <Arduino.h>
#include "Utils.h"
#include "ChessPuzzle.h"
#include "CsvParser.h"

/*
 * if solution file is present
 *   draw just solution - partly render
 * 
 * if no solution is present
 *    parse config
 *    select random puzlle
 *    draw new puzzle
 *    create solution into the file
 */
class PuzzleMain {
public:
  char *SOLUTION_FILE = (char *)"solution.txt";

  PuzzleMain();

  void createFileWithSolution();
  void deleteFileWithSolution();
  bool hasSolutionFile();
  void readSolution();

  ChessPuzzle puzzle;
  SdFat SD;
};

#endif  // PUZZLEMAIN_H