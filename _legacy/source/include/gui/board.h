#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void drawBoard(const char board[9]);
void renderText(const char *text, int x, int y, SDL_Color color);
void drawCurrentPlayer(char currentPlayer);
void clearScreen();

#endif // GAME_FUNCTIONS_H
