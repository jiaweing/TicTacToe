void clearScreen(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
}

void drawEllipse(SDL_Renderer *renderer, int x, int y, int rx, int ry)
{
	double angle, x1, y1, x2, y2;

	for (int i = 0; i < 360; i++)
	{
		angle = i * M_PI / 180;
		x1 = rx * cos(angle);
		y1 = ry * sin(angle);
		x2 = x1 * cos(angle) - y1 * sin(angle) + x;
		y2 = x1 * sin(angle) + y1 * cos(angle) + y;
		SDL_RenderDrawPoint(renderer, x2, y2);
	}
}

void drawBoard(SDL_Renderer *renderer, const char board[9])
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	SDL_Rect board_rect = {SCREEN_WIDTH / 2 - BOARD_SIZE / 2, SCREEN_HEIGHT / 2 - BOARD_SIZE / 2, BOARD_SIZE, BOARD_SIZE};
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &board_rect);

	for (int i = 0; i < 9; ++i)
	{
		int row = i % 3;
		int col = i / 3;
		SDL_Rect tileRect = {board_rect.x + row * TILE_SIZE, board_rect.y + col * TILE_SIZE, TILE_SIZE, TILE_SIZE};
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &tileRect);

		if (board[i] == X_SYMBOL)
		{
			SDL_RenderDrawLine(renderer, tileRect.x + LINE_WIDTH, tileRect.y + LINE_WIDTH, tileRect.x + TILE_SIZE - LINE_WIDTH, tileRect.y + TILE_SIZE - LINE_WIDTH);
			SDL_RenderDrawLine(renderer, tileRect.x + TILE_SIZE - LINE_WIDTH, tileRect.y + LINE_WIDTH, tileRect.x + LINE_WIDTH, tileRect.y + TILE_SIZE - LINE_WIDTH);
		}
		else if (board[i] == O_SYMBOL)
		{
			drawEllipse(renderer, tileRect.x + TILE_SIZE / 2, tileRect.y + TILE_SIZE / 2, TILE_SIZE / 2 - LINE_WIDTH, TILE_SIZE / 2 - LINE_WIDTH);
		}
	}

	SDL_RenderPresent(renderer);
}