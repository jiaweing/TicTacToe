char win(const char board[9])
{
	// determines if a player has won, returns 0 otherwise.
	unsigned int wins[8][3] = {{2, 4, 6}, {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}}; // win states
	for (int i = 0; i < 8; ++i)
	{
		if (board[wins[i][0]] != EMPTY_SYMBOL &&
			board[wins[i][0]] == board[wins[i][1]] &&
			board[wins[i][0]] == board[wins[i][2]])
			return board[wins[i][0]];
	}
	return EMPTY_SYMBOL;
}