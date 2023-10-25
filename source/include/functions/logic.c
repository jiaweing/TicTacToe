void playerMove(char symbol, char board[9])
{
	SDL_Event event;
	int done = 0;

	drawCurrentPlayer(symbol);

	// Poll for event
	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					if (x >= SCREEN_WIDTH / 2 - BOARD_SIZE / 2 && x < SCREEN_WIDTH / 2 + BOARD_SIZE / 2 &&
						y >= SCREEN_HEIGHT / 2 - BOARD_SIZE / 2 && y < SCREEN_HEIGHT / 2 + BOARD_SIZE / 2)
					{
						int row = (y - (SCREEN_HEIGHT / 2 - BOARD_SIZE / 2)) / TILE_SIZE;
						int col = (x - (SCREEN_WIDTH / 2 - BOARD_SIZE / 2)) / TILE_SIZE;
						int pos = (row * 3) + col;
						if (board[pos] == EMPTY_SYMBOL)
						{
							board[pos] = symbol;
							done = 1;
						}
					}
				}
				break;
			case SDL_QUIT:
				exit(0);
				break;
			}
		}
	}
}

void computerMove(char board[9])
{
	drawCurrentPlayer(X_SYMBOL);

	int move = -1;
	int score = -2;
	int i;
	for (i = 0; i < 9; ++i)
	{
		if (board[i] == EMPTY_SYMBOL)
		{
			board[i] = X_SYMBOL;
			int tempScore = -minimax(board, O_SYMBOL);
			board[i] = EMPTY_SYMBOL;
			if (tempScore > score)
			{
				score = tempScore;
				move = i;
			}
		}
	}

	// returns a score based on minimax tree at a given node.
	board[move] = X_SYMBOL;
}