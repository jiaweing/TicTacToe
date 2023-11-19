// TODO: choose who go first, random first position, choose colour of icon, undo button, responsive, timer, preview move with half opacity w/ blinks or animation, multiplayer

int minimax(char board[9], char player, int depth, int alpha, int beta)
{
	int bestScore = (player == X_SYMBOL) ? MIN_SCORE : MAX_SCORE;
	char winner = win(board);
	if (winner == X_SYMBOL) 
	{
		return MAX_SCORE;
	}
	else if (winner == O_SYMBOL)
	{
		return MIN_SCORE;
	}
	else
	{
		for (int i = 0; i < 9; ++i)
		{
			if (board[i] == EMPTY_SYMBOL)
			{
                board[i] = player;

				if (player == X_SYMBOL) // max
				{
					int score = minimax(board, O_SYMBOL, depth + 1, alpha, beta);
					if (bestScore < score)
					{
						bestScore = score - depth * 10;

						// Check if this branch's best move is worse than the best
						// option of a previously search branch. If it is, skip it
						alpha = alpha > bestScore ? alpha : bestScore;
						if (beta <= alpha) 
						{ 
                            board[i] = EMPTY_SYMBOL;
							break;
						}
					}
				} 
				else // min
				{
					int score = minimax(board, X_SYMBOL, depth + 1, alpha, beta);
					if (bestScore > score)
					{
						bestScore = score + depth * 10;

						// Check if this branch's best move is worse than the best
						// option of a previously search branch. If it is, skip it
						beta = beta < bestScore ? beta : bestScore;
						if (beta <= alpha) 
						{ 
                            board[i] = EMPTY_SYMBOL;
							break;
						}
					}
				}

				board[i] = EMPTY_SYMBOL;
			}		
		}

		return bestScore;
	}
}