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

char ChessBoard::getPieceAt(char* square) {
  // square format: "e2" (file a-h, rank 1-8)
  // board[0][0] = A8, board[7][7] = H1
  char file = square[0]; // a-h
  char rank = square[1]; // 1-8
  int col = file - 'a'; // 0-7
  int row = 8 - (rank - '0'); // 7-0
  if (row >= 0 && row < 8 && col >= 0 && col < 8) {
    return board[row][col];
  }
  return '.'; // empty or invalid
}

void ChessBoard::makeMove(char* uciMove) {
  // UCI format: e2e4, g1f3, e7e8q (promotion)
  char from[3] = {uciMove[0], uciMove[1], '\0'};
  char to[3] = {uciMove[2], uciMove[3], '\0'};

  // Convert to board indices
  int fromCol = from[0] - 'a';
  int fromRow = 8 - (from[1] - '0');
  int toCol = to[0] - 'a';
  int toRow = 8 - (to[1] - '0');

  if (fromRow >= 0 && fromRow < 8 && fromCol >= 0 && fromCol < 8 &&
      toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8) {
    // Move piece
    char piece = board[fromRow][fromCol];
    board[fromRow][fromCol] = '.';
    board[toRow][toCol] = piece;

    // Handle promotion (e.g., e7e8q)
    if (strlen(uciMove) == 5) {
      char promoPiece = uciMove[4];
      // Convert to uppercase for white, lowercase for black
      if (isupper(piece)) {
        board[toRow][toCol] = toupper(promoPiece);
      } else {
        board[toRow][toCol] = tolower(promoPiece);
      }
    }
  }
}
