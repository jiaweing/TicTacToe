#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

#define ERROR -1
#define SUCCESS 1

#define SCREEN_WIDTH 950
#define SCREEN_HEIGHT 700

#define PCSENIOR_FONT "assets/fonts/pcsenior.ttf"

void renderText(const char *text, int x, int y, SDL_Color color);
void clearScreen();

SDL_Window *window;
SDL_Renderer *renderer;