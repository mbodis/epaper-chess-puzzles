#include "ChessPuzzle.h"

ChessPuzzle::ChessPuzzle() {
}

void ChessPuzzle::init() {
  this->chessBoard.parseFEN(this->fenNotation);
}

void ChessPuzzle::printToLog() {
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
#ifdef USE_EPAPER_V2
  EPD_4IN2_V2_Init_Fast(0);
  EPD_4IN2_V2_Clear();
#else
  EPD_4IN2_Init_Fast();
  EPD_4IN2_Clear();
#endif
  DEV_Delay_ms(500);

  // Create a new image cache
  UBYTE *BlackImage;
  UWORD Imagesize = ((EPAPER_WIDTH % 8 == 0) ? (EPAPER_WIDTH / 8) : (EPAPER_WIDTH / 8 + 1)) * EPAPER_HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    Serial.println("[ChessPuzzle] ERR: Failed to apply for black memory...");
    while (1)
      ;
  }

  this->renderChessboardImage(BlackImage);

  // final print
#ifdef USE_EPAPER_V2
  EPD_4IN2_V2_Display(BlackImage);
#else
  EPD_4IN2_Display(BlackImage);
#endif

  // clean
  DEV_Delay_ms(500);
#ifdef USE_EPAPER_V2
  EPD_4IN2_V2_Sleep();
#else
  EPD_4IN2_Sleep();
#endif
  free(BlackImage);
  BlackImage = NULL;
}

// Renders the current puzzle (welcome / congratulations / chessboard +
// metadata) into the given full-screen image buffer. Used by both
// drawChessboard() and drawSolution() so the solution view can include
// the original chessboard as background for V2 partial refresh.
void ChessPuzzle::renderChessboardImage(UBYTE *BlackImage) {
  // new image
  Paint_NewImage(BlackImage, EPAPER_WIDTH, EPAPER_HEIGHT, 0, WHITE);
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);

  this->isMessage = this->puzzleCounter == 0 || this->puzzleCounter % 1000 == 0;

  if (this->isMessage){
    // welcome screen
    if (this->puzzleCounter == 0){
      Paint_DrawString_EN(150, 100, "WELCOME", &Font20, WHITE, BLACK); 
      Paint_DrawString_EN(190, 120, "to", &Font12, WHITE, BLACK); 
      Paint_DrawString_EN(110, 140, "Chess Puzzle Box", &Font16, WHITE, BLACK); 
      Paint_DrawString_EN(350, 270, "v0.1", &Font12, WHITE, BLACK); 
    
    // congratulation X * 1000 puzzle screen
    }else{
      Paint_DrawString_EN(90, 100, "CONGRATULATIONS", &Font20, WHITE, BLACK);
      Paint_DrawString_EN(140, 130, "you solved", &Font16, WHITE, BLACK); 
      Paint_DrawNum(100, 160, this->puzzleCounter, &Font20, BLACK, WHITE); 
      Paint_DrawString_EN(200, 160, "puzzles", &Font20, WHITE, BLACK); 
    }
  }else{
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
    // puzzle ID in top-right corner: #abc123 (commented out)
    // char puzzleIdWithHash[50];
    // snprintf(puzzleIdWithHash, sizeof(puzzleIdWithHash), "#%s", this->id);
    // Paint_DrawString_EN(330, 22, puzzleIdWithHash, &Font12, WHITE, BLACK);

    // triangle arrow before who is on move (outline for WHITE, filled for BLACK)
    int arrowX = 298;
    int arrowY = 30;
    if (this->whiteIsOnTheMove) {
      // WHITE: outline only (3 lines)
      Paint_DrawLine(arrowX, arrowY, arrowX - 5, arrowY - 5, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(arrowX, arrowY, arrowX - 5, arrowY + 5, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(arrowX - 5, arrowY - 5, arrowX - 5, arrowY + 5, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    } else {
      // BLACK: filled (very dense horizontal lines)
      Paint_DrawLine(arrowX, arrowY, arrowX - 5, arrowY - 5, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(arrowX, arrowY, arrowX - 5, arrowY + 5, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(arrowX - 5, arrowY - 5, arrowX - 5, arrowY + 5, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(arrowX - 4, arrowY - 4, arrowX - 4, arrowY + 4, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(arrowX - 3, arrowY - 3, arrowX - 3, arrowY + 3, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(arrowX - 2, arrowY - 2, arrowX - 2, arrowY + 2, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(arrowX - 1, arrowY - 1, arrowX - 1, arrowY + 1, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    }

    Paint_DrawString_EN(305, 22, this->whiteIsOnTheMove ? "WHITE" : "BLACK", &Font20, WHITE, BLACK);

    // horizontal line separator below who is on move
    // Paint_DrawLine(290, 50, 380, 50, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    // compact format: labels on first line, values on second line
    Paint_DrawString_EN(290, 50, "CASTLE", &Font12, WHITE, BLACK);
    Paint_DrawString_EN(360, 50, "EP", &Font12, WHITE, BLACK);

    Paint_DrawString_EN(290, 64, this->castlingAvailability, &Font12, WHITE, BLACK);
    Paint_DrawString_EN(360, 64, this->enPassant, &Font12, WHITE, BLACK);

    // horizontal line separator below CASTLE/EP
    Paint_DrawLine(290, 80, 380, 80, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  }
}

void ChessPuzzle::drawSolution() {
  DEV_Module_Init();

  UWORD Imagesize = ((EPAPER_WIDTH % 8 == 0) ? (EPAPER_WIDTH / 8) : (EPAPER_WIDTH / 8 + 1)) * EPAPER_HEIGHT;

#ifdef USE_EPAPER_V2
  // V2 diff-based partial refresh needs TWO full-screen buffers:
  //   - OldImage: chessboard as it is currently on the panel (rendered
  //     from the FEN that was persisted to SD by drawChessboard()).
  //   - NewImage: chessboard + solution text overlay.
  // For every pixel where OldImage == NewImage the V2 partial waveform
  // produces no pulse, so the chessboard area on the panel is left
  // untouched and only the solution rect changes.
  UBYTE *OldImage = (UBYTE *)malloc(Imagesize);
  UBYTE *NewImage = (UBYTE *)malloc(Imagesize);
  if (OldImage == NULL || NewImage == NULL) {
    Serial.println("[ChessPuzzle] ERR: Failed to apply for black memory...");
    while (1)
      ;
  }

  // Render the chessboard into the "old" buffer.
  this->renderChessboardImage(OldImage);
  // Start the "new" buffer as an identical copy.
  memcpy(NewImage, OldImage, Imagesize);

  // Overlay the solution text only on the new buffer.
  Paint_SelectImage(NewImage);
  Paint_DrawString_EN(290, 85, "SOLUTION:", &Font12, WHITE, BLACK);
  int yy = 100;
  char *partlySolution = strtok(this->solution, " ");
  while (partlySolution != nullptr) {
    Paint_DrawString_EN(290, yy, partlySolution, &Font16, WHITE, BLACK);
    partlySolution = strtok(nullptr, " ");
    yy += 15;
  }

  EPD_4IN2_V2_Init_Partial();
  DEV_Delay_ms(500);
  EPD_4IN2_V2_PartialDisplayDiff(OldImage, NewImage);

  DEV_Delay_ms(500);
  EPD_4IN2_V2_Sleep();

  free(OldImage);
  free(NewImage);
  OldImage = NULL;
  NewImage = NULL;
#else
  // V1 path: uses a single buffer; V1 driver keeps its own old-data RAM cache.
  EPD_4IN2_Init_Partial();
  DEV_Delay_ms(500);

  UBYTE *BlackImage;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    Serial.println("[ChessPuzzle] ERR: Failed to apply for black memory...");
    while (1)
      ;
  }
  Paint_NewImage(BlackImage, EPAPER_WIDTH, EPAPER_HEIGHT, 0, WHITE);
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);

  Paint_DrawString_EN(290, 85, "SOLUTION:", &Font12, WHITE, BLACK);
  int yy = 100;
  char *partlySolution = strtok(this->solution, " ");
  while (partlySolution != nullptr) {
    Paint_DrawString_EN(290, yy, partlySolution, &Font16, WHITE, BLACK);
    partlySolution = strtok(nullptr, " ");
    yy += 15;
  }

  int startX = 280;
  int startY = 85;
  int endX = 380;
  int endY = 295;
  EPD_4IN2_PartialDisplay(startX, startY, endX, endY, BlackImage);

  DEV_Delay_ms(500);
  EPD_4IN2_Sleep();
  free(BlackImage);
  BlackImage = NULL;
#endif
}
