// something's not working here
int minimax(char board[9], char player)
{
	// How is the position like for player (their turn) on board?
	char winner = win(board);
	if (winner != EMPTY_SYMBOL)
		return winner == X_SYMBOL ? 1 : -1;

	int move = -1;
	int score = -2; // Losing moves are preferred to no move
	for (int i = 0; i < 9; ++i)
	{ // For all moves
		if (board[i] == EMPTY_SYMBOL)
		{					   // If legal
			board[i] = player; // Try the move
			int thisScore = -minimax(board, player == X_SYMBOL ? O_SYMBOL : X_SYMBOL);
			if (thisScore > score)
			{
				score = thisScore;
				move = i;
			}						 // Pick the one that's worst for the opponent
			board[i] = EMPTY_SYMBOL; // Reset board after try
		}
	}

	if (move == -1)
		return 0;
	return score;
}