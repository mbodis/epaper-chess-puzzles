#include "ChessPuzzle.h"

ChessPuzzle::ChessPuzzle() {
}

void ChessPuzzle::init() {
  this->chessBoard.parseFEN(this->fenNotation);
}

void ChessPuzzle::print() {
  Serial.println("[ChessPuzzle] --selected puzzle--");
  Serial.printf("[ChessPuzzle] id: %s \n", this->id);
  Serial.printf("[ChessPuzzle] FEN notation: %s \n", this->fenNotation);
  Serial.printf("[ChessPuzzle] solution: %s \n", this->solution);
  Serial.printf("[ChessPuzzle] %s is on the move \n", this->whiteIsOnTheMove ? "white" : "black");
  Serial.printf("[ChessPuzzle] en passant: %s \n", this->enPassant);
  Serial.printf("[ChessPuzzle] castling availability: %s \n", this->castlingAvailability);
  this->chessBoard.printBoard();
}

/*
 * Paint_DrawBitMap(chessboard);
 *  400 width
 *  300 heigh
 *  square 32x32
 *  8 * 32 = 256
 *  (300 - 256) / 2 = 22 -> top, left, bottom, padding of chessboard
 */
void ChessPuzzle::drawChessboard() {
  DEV_Module_Init();

  // clear screen
  EPD_4IN2_Init_Fast();
  EPD_4IN2_Clear();
  DEV_Delay_ms(500);

  // Create a new image cache
  UBYTE *BlackImage;
  UWORD Imagesize = ((EPD_4IN2_WIDTH % 8 == 0) ? (EPD_4IN2_WIDTH / 8) : (EPD_4IN2_WIDTH / 8 + 1)) * EPD_4IN2_HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    Serial.println("[ChessPuzzle] ERR: Failed to apply for black memory...");
    while (1)
      ;
  }
  // new image
  Paint_NewImage(BlackImage, EPD_4IN2_WIDTH, EPD_4IN2_HEIGHT, 0, WHITE);
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);

  // print chessboard
  int marginX = 22;
  int marginY = 22;
  int x = 0;
  int y = 0;
  boolean isWhite = true;
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {

      x = marginX + col * 32;
      y = marginY + row * 32;

      if ((row + col) % 2 == 0) {
        Paint_DrawImage(white, x, y, 32, 32);
        isWhite = true;
      } else {
        Paint_DrawImage(black, x, y, 32, 32);
        isWhite = false;
      }

      if (this->chessBoard.board[row][col] == 'R') {
        Paint_DrawImage(isWhite ? white_rook_w : white_rook_b, x, y, 32, 32);
      } else if (this->chessBoard.board[row][col] == 'N') {
        Paint_DrawImage(isWhite ? white_knight_w : white_knight_b, x, y, 32, 32);
      } else if (this->chessBoard.board[row][col] == 'B') {
        Paint_DrawImage(isWhite ? white_bishop_w : white_bishop_b, x, y, 32, 32);
      } else if (this->chessBoard.board[row][col] == 'K') {
        Paint_DrawImage(isWhite ? white_king_w : white_king_b, x, y, 32, 32);
      } else if (this->chessBoard.board[row][col] == 'Q') {
        Paint_DrawImage(isWhite ? white_queen_w : white_queen_b, x, y, 32, 32);
      } else if (this->chessBoard.board[row][col] == 'P') {
        Paint_DrawImage(isWhite ? white_pawn_w : white_pawn_b, x, y, 32, 32);
      } else if (this->chessBoard.board[row][col] == 'r') {
        Paint_DrawImage(isWhite ? black_rook_w : black_rook_b, x, y, 32, 32);
      } else if (this->chessBoard.board[row][col] == 'n') {
        Paint_DrawImage(isWhite ? black_knight_w : black_knight_b, x, y, 32, 32);
      } else if (this->chessBoard.board[row][col] == 'b') {
        Paint_DrawImage(isWhite ? black_bishop_w : black_bishop_b, x, y, 32, 32);
      } else if (this->chessBoard.board[row][col] == 'k') {
        Paint_DrawImage(isWhite ? black_king_w : black_king_b, x, y, 32, 32);
      } else if (this->chessBoard.board[row][col] == 'q') {
        Paint_DrawImage(isWhite ? black_queen_w : black_queen_b, x, y, 32, 32);
      } else if (this->chessBoard.board[row][col] == 'p') {
        Paint_DrawImage(isWhite ? black_pawn_w : black_pawn_b, x, y, 32, 32);
      }
    }
  }

  // print chessboard notation
  Paint_DrawString_EN(0, 32 + 0 * 32, "8", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(0, 32 + 1 * 32, "7", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(0, 32 + 2 * 32, "6", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(0, 32 + 3 * 32, "5", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(0, 32 + 4 * 32, "4", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(0, 32 + 5 * 32, "3", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(0, 32 + 6 * 32, "2", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(0, 32 + 7 * 32, "1", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(26 + 0 * 32, 280, "A", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(26 + 1 * 32, 280, "B", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(26 + 2 * 32, 280, "C", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(26 + 3 * 32, 280, "D", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(26 + 4 * 32, 280, "E", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(26 + 5 * 32, 280, "F", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(26 + 6 * 32, 280, "G", &Font16, WHITE, BLACK);
  Paint_DrawString_EN(26 + 7 * 32, 280, "H", &Font16, WHITE, BLACK);

  // print chessboard details
  Paint_DrawString_EN(290, 20, "puzzle: ", &Font12, WHITE, BLACK);
  Paint_DrawString_EN(340, 20, this->id, &Font12, WHITE, BLACK);

  Paint_DrawString_EN(290, 30, "castling: ", &Font12, WHITE, BLACK);
  Paint_DrawString_EN(360, 30, this->castlingAvailability, &Font12, WHITE, BLACK);

  Paint_DrawString_EN(290, 40, "en passant: ", &Font12, WHITE, BLACK);
  Paint_DrawString_EN(370, 40, this->enPassant, &Font12, WHITE, BLACK);

  Paint_DrawString_EN(290, 60, "next move:", &Font12, WHITE, BLACK);
  Paint_DrawString_EN(290, 70, this->whiteIsOnTheMove ? "White" : "Black", &Font16, WHITE, BLACK);

  // final print
  EPD_4IN2_Display(BlackImage);

  // clean
  DEV_Delay_ms(500);
  EPD_4IN2_Sleep();
  free(BlackImage);
  BlackImage = NULL;
}

void ChessPuzzle::drawSolution() {
  DEV_Module_Init();
  // need to init partial otherwise image will be inverted
  EPD_4IN2_Init_Partial();
  DEV_Delay_ms(500);

  //Create a new image cache
  UBYTE *BlackImage;
  UWORD Imagesize = ((EPD_4IN2_WIDTH % 8 == 0) ? (EPD_4IN2_WIDTH / 8) : (EPD_4IN2_WIDTH / 8 + 1)) * EPD_4IN2_HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    Serial.println("[ChessPuzzle] ERR: Failed to apply for black memory...");
    while (1)
      ;
  }
  // new image
  Paint_NewImage(BlackImage, EPD_4IN2_WIDTH, EPD_4IN2_HEIGHT, 0, WHITE);
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);

  // print solution
  Paint_DrawString_EN(290, 90, "solution:", &Font12, WHITE, BLACK);
  int yy = 100;

  char *partlySolution = strtok(this->solution, " ");
  while (partlySolution != nullptr) {
    Paint_DrawString_EN(290, yy, partlySolution, &Font16, WHITE, BLACK);
    partlySolution = strtok(nullptr, " ");
    yy += 15;
  }

  // partly print only solution
  EPD_4IN2_PartialDisplay(285, 90, 380, 250, BlackImage);

  // clean
  DEV_Delay_ms(500);
  EPD_4IN2_Sleep();
  free(BlackImage);
  BlackImage = NULL;
}
