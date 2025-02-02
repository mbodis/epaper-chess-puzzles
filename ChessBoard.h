#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <Arduino.h>

class ChessBoard {
public:
  /*
   * board representation:
   * - board[0][0] - top left - A8
   * - board[0][7] - bottom left - A1
   */
  char board[8][8];

  ChessBoard();
  void parseFEN(char* fenNotation);
  void printBoard();
};

#endif  // CHESSBOARD_H