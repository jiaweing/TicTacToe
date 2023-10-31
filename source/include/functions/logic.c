// Function to handle the PvAI game
void playPvAI(char board[9], SDL_Renderer *renderer, SDL_Window *window, int gameType)
{
	int turn = 0;
	char player = X_SYMBOL;

	for (turn = 0; turn < 9 && win(board) == EMPTY_SYMBOL; ++turn)
	{
		if (player == X_SYMBOL)
		{
			computerMove(board,gameType);
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

/* // Function to handle the ImperfectAI game
void playPvimperfectAI(char board[9], SDL_Renderer *renderer, SDL_Window *window, int gameType)
{
	int turn = 0;
	char player = X_SYMBOL;

	for (turn = 0; turn < 9 && win(board) == EMPTY_SYMBOL; ++turn)
	{
		if (player == X_SYMBOL)
		{
			computerMove(board,gameType);
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
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The computer (X) is the winner imperfect aglo!", window);
	}
	else if (winner == O_SYMBOL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You (O) are the winner! imperfect algo ", window);
	}
	else
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The game ended in a draw... imperfect algo", window);
	}

	// Reset the game
	player = X_SYMBOL;
	for (int i = 0; i < 9; ++i)
	{
		board[i] = 'b';
	}
} */

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

void computerMove(char board[9], int gametype)
{
	drawCurrentPlayer(X_SYMBOL);
	
	if (gametype == MINIMAX_GAME) {
		int move = 0;
		int bestScore = MIN_SCORE;
		for (int i = 0; i < 9; ++i)
		{
			if (board[i] == EMPTY_SYMBOL)
			{
				board[i] = X_SYMBOL;
				int score = minimax(board, O_SYMBOL, 0, MIN_SCORE, MAX_SCORE);
				board[i] = EMPTY_SYMBOL;
				if (score > bestScore)
				{
					bestScore = score;
					move = i;
				}
			}
		}
		// returns a score based on minimax tree at a given node.
		board[move] = X_SYMBOL;
	}

	else if (gametype == IMPERFECT_GAME)
	{
		int data[MAX_ROWS][NUM_POSITIONS];
		int labels[MAX_ROWS];
		int training_data[MAX_ROWS][NUM_POSITIONS];
		int training_labels[MAX_ROWS];
		int testing_data[MAX_ROWS][NUM_POSITIONS]; 
		int testing_labels[MAX_ROWS]; 
		int split_ratio = 80;
		double priors[NUM_CLASSES];
		double likelihoods[NUM_POSITIONS][MAX_ROWS][NUM_CLASSES];

		extract_data(data, labels);
		split_data(data, labels, training_data, training_labels, testing_data, testing_labels, split_ratio);
		calculatePriors(training_data, training_labels, priors);
		calculateLikelihoods(training_data, training_labels, likelihoods, priors);

		int next_move = predictNextMove(board, priors, likelihoods);

		board[next_move] = X_SYMBOL;
		
	}
		
}