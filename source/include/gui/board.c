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
			SDL_RenderDrawLine(_renderer, tileRect.x + LINE_WIDTH, tileRect.y + LINE_WIDTH, tileRect.x + TILE_SIZE - LINE_WIDTH, tileRect.y + TILE_SIZE - LINE_WIDTH);
			SDL_RenderDrawLine(_renderer, tileRect.x + TILE_SIZE - LINE_WIDTH, tileRect.y + LINE_WIDTH, tileRect.x + LINE_WIDTH, tileRect.y + TILE_SIZE - LINE_WIDTH);
		}
		else if (board[i] == O_SYMBOL)
		{
			drawEllipse(tileRect.x + TILE_SIZE / 2, tileRect.y + TILE_SIZE / 2, TILE_SIZE / 2 - LINE_WIDTH, TILE_SIZE / 2 - LINE_WIDTH);
		}
	}

	drawCurrentPlayer(' ');
	SDL_RenderPresent(_renderer);
}

void drawCurrentPlayer(char currentPlayer)
{
	printf("Current Player: %c\n", currentPlayer);
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

void drawEllipse(int x, int y, int rx, int ry)
{
	double angle, x1, y1, x2, y2;

	for (int i = 0; i < 360; i++)
	{
		angle = i * M_PI / 180;
		x1 = rx * cos(angle);
		y1 = ry * sin(angle);
		x2 = x1 * cos(angle) - y1 * sin(angle) + x;
		y2 = x1 * sin(angle) + y1 * cos(angle) + y;
		SDL_RenderDrawPoint(_renderer, x2, y2);
	}
}

void clearScreen()
{
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	SDL_RenderClear(_renderer);
}