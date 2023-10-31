#define SCREEN_WIDTH 1100
#define SCREEN_HEIGHT 700
#define BOARD_SIZE 300
#define TILE_SIZE 100
#define LINE_WIDTH 10

#define EMPTY_SYMBOL 'b'
#define X_SYMBOL 'X'
#define O_SYMBOL 'O'

#define MAIN_MENU 0
#define TWO_PLAYER_GAME 1
#define MINIMAX_GAME 2
#define AI_GAME 3

SDL_Renderer *_renderer;
TTF_Font *_font;
#define MAX_SCORE 1000
#define MIN_SCORE -1000

// Definitions for naivebayes.c
// Constants to represent board positions
#define X 0
#define O 1
#define EMPTY 2 
// Number of board positions (9 in Tic Tac Toe)
#define NUM_POSITIONS 9
// Number of classes (Positive if X wins, Negative if Draw or O wins)
#define NUM_CLASSES 2
// Define the maximum number of data rows
#define MAX_ROWS 958
// Define the number of characters for 'positive\n' & 'negative\n'
#define MAX_CHARACTERS 9