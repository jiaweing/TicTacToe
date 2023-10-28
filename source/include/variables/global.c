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
#define IMPERFECT_GAME 3

SDL_Renderer *_renderer;
TTF_Font *_font;
#define MAX_SCORE 1000
#define MIN_SCORE -1000