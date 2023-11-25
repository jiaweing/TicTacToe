#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -1
#define SUCCESS 1

#define SCREEN_WIDTH 950
#define SCREEN_HEIGHT 700

#define BOARD_STATUS_PADDING 10

#ifdef __APPLE__
#define PCSENIOR_FONT "assets/fonts/pcsenior.ttf"
#define ARCADE_FONT "assets/fonts/ARCADE.TTF"
#define INTER_FONT "assets/fonts/Inter-Regular.ttf"
#endif

#ifdef _WIN32
#define PCSENIOR_FONT "assets/fonts/pcsenior.ttf"
#define ARCADE_FONT "assets/fonts/ARCADE.TTF"
#define INTER_FONT "assets/fonts/Inter-Regular.ttf"
#endif

#define BACKDROP_IMAGE "assets/images/backdropfinal.png"

int initialiseFonts();
void drawTextInput(char *label, char *var);
void renderText(char *text, TTF_Font *font, int x, int y, SDL_Color color);
void renderAnchoredText(char *text, TTF_Font *font, int x, int y, SDL_Color color);
int setBackgroundImage(const char *imagePath);
void clearScreen();

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern TTF_Font *pcsenior24_f;
extern TTF_Font *pcsenior18_f;
extern TTF_Font *arcade24_f;

extern SDL_Color blue;
extern SDL_Color indigo;
extern SDL_Color red;
extern SDL_Color orange;
extern SDL_Color green;
extern SDL_Color black;
extern SDL_Color white;
extern SDL_Color cyan;
extern SDL_Color btnblue;
extern SDL_Color deepblue;
extern SDL_Color transparent;