void drawBoard(const char board[9])
{
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	SDL_RenderClear(_renderer);

	SDL_Rect board_rect = {SCREEN_WIDTH / 2 - BOARD_SIZE / 2, SCREEN_HEIGHT / 2 - BOARD_SIZE / 2, BOARD_SIZE, BOARD_SIZE};
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(_renderer, &board_rect);

	for (int i = 0; i < 9; ++i)
	{
		int row = i % 3;
		int col = i / 3;
		SDL_Rect tileRect = {board_rect.x + row * TILE_SIZE, board_rect.y + col * TILE_SIZE, TILE_SIZE, TILE_SIZE};
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(_renderer, &tileRect);

	 if (board[i] == X_SYMBOL)
		{
			// Set the color for X
			SDL_Color color = {255, 0, 0, 255};  // Red color
			renderText("X", tileRect.x + TILE_SIZE / 2, tileRect.y + TILE_SIZE / 2, color);
		}
		else if (board[i] == O_SYMBOL)
		{
			// Set the color for O
			SDL_Color color = {0, 155, 255, 255};  // Blue color
			renderText("O", tileRect.x + TILE_SIZE / 2, tileRect.y + TILE_SIZE / 2, color);
		}
	}

	drawCurrentPlayer(' ');
	SDL_RenderPresent(_renderer);
}

void renderText(const char *text, int x, int y, SDL_Color color)
{
	SDL_Surface *textSurface = TTF_RenderText_Solid(_font, text, color);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);

	SDL_Rect textRect;
	textRect.x = x - textSurface->w / 2;
	textRect.y = y - textSurface->h / 2;
	textRect.w = textSurface->w;
	textRect.h = textSurface->h;

	SDL_RenderCopy(_renderer, textTexture, NULL, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}


void drawCurrentPlayer(char currentPlayer)
{
	SDL_Rect textRect = {10, 10, 0, 0};
	SDL_Color textColor = {0, 0, 0, 255};

	char text[20];
	sprintf(text, "Current Player: %c", currentPlayer);

	SDL_Surface *textSurface = TTF_RenderText_Solid(_font, text, textColor);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);

	textRect.w = textSurface->w;
	textRect.h = textSurface->h;

	SDL_RenderCopy(_renderer, textTexture, NULL, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	SDL_RenderPresent(_renderer);
}

void clearScreen()
{
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	SDL_RenderClear(_renderer);
}