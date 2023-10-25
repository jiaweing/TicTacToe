// Function to handle the PvAI game
void playPvAI(char board[9], SDL_Renderer *renderer, SDL_Window *window)
{
	int turn = 0;
	char player = X_SYMBOL;

	for (turn = 0; turn < 9 && win(board) == EMPTY_SYMBOL; ++turn)
	{
		if (player == X_SYMBOL)
		{
			computerMove(board);
			player = O_SYMBOL;
		}
		else
		{
			playerMove(O_SYMBOL, board);
			player = X_SYMBOL;
		}

		drawBoard(board);
	}

	char winner = win(board);

	if (winner == X_SYMBOL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The computer (X) is the winner!", window);
	}
	else if (winner == O_SYMBOL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You (O) are the winner!", window);
	}
	else
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The game ended in a draw...", window);
	}

	// Reset the game
	player = X_SYMBOL;
	for (int i = 0; i < 9; ++i)
	{
		board[i] = 'b';
	}
}

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

	int move = 0;
	int score = MIN_SCORE;
	for (int i = 0; i < 9; ++i)
	{
		if (board[i] == EMPTY_SYMBOL)
		{
			board[i] = X_SYMBOL;
			int tempScore = minimax(board, O_SYMBOL, 0, MIN_SCORE, MAX_SCORE);
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