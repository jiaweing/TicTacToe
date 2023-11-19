#include "ui.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void renderText(const char *text, int x, int y, SDL_Color color)
{
	TTF_Font *font = TTF_OpenFont(PCSENIOR_FONT, 24);
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Rect textRect;
	textRect.x = x - textSurface->w / 2;
	textRect.y = y - textSurface->h / 2;
	textRect.w = textSurface->w;
	textRect.h = textSurface->h;

	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

void clearScreen()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
}