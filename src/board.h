#include <ctype.h>

#define EMPTY_SYMBOL 'b'
#define X_SYMBOL 'x'
#define O_SYMBOL 'o'

#define BOARD_SIZE 300
#define TILE_SIZE 100
#define LINE_WIDTH 10

void playerMove(char symbol, char board[9]);
char win(const char board[9]);
void drawBoard(const char board[9]);
void drawCurrentPlayer(char currentPlayer);