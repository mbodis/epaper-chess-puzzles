#ifndef CHESSPUZZLE_H
#define CHESSPUZZLE_H

/* display */
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>

#include <Arduino.h>
#include "ChessBoard.h"

class ChessPuzzle {

public:
  // lichess puzzle id e.g. 00sHx
  char* id = new char[10];

  // e.g. q3k1nr/1pp1nQpp/3p4/1P2p3/4P3/B1PP1b2/B5PP/5K2 b k - 0 17
  char* fenNotation = new char[100];

  // e.g. e8d7 a2e6 d7d8 f7f8
  char* solution = new char[100];

  // boolean of next move
  bool whiteIsOnTheMove = false;

  // e.g. kqKQ
  // k - black king side
  // q - black queen side
  // K - white king side
  // Q - white queen side
  char* castlingAvailability = new char[10];

  // e.g. e3, or - when not possible
  char* enPassant = new char[20];

  ChessBoard chessBoard;

  ChessPuzzle();
  void init();
  void print();
  void drawChessboard();
  void drawSolution();
};

#endif  // CHESSPUZZLE_H