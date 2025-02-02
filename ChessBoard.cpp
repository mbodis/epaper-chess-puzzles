#include "ChessBoard.h"

/*
 * - initialize the board with empty squares
 */
ChessBoard::ChessBoard() {
  for (int row = 0; row < 8; ++row)
    for (int col = 0; col < 8; ++col)
      board[row][col] = '.';
}

/*
 * fenNotation: 1k1r4/pp3pp1/2p1p3/4b3/P3n1P1/8/KPP2PN1/3rBR1R
 */
void ChessBoard::parseFEN(char* fenNotation) {
  int row = 0, col = 0;
  for (int i = 0; i < strlen(fenNotation); i++) {
    char c = fenNotation[i];
    if (isdigit(c)) {
      col += c - '0';  // empty squares
    } else if (c == '/') {
      row++;  // move to the next row
      col = 0;
    } else {
      board[row][col] = c;  // place the piece
      col++;
    }
  }
}

void ChessBoard::printBoard() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      Serial.print(board[row][col]);
      Serial.print(" ");
    }
    Serial.println();
  }
}
