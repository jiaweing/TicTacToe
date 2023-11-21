#include "board.h"
#include "naivebayes.h"
#include "game_logic.h"

// Function to predict the next move
int predictNextMove(char board[], double priors[], double likelihoods[NUM_POSITIONS][MAX_ROWS][NUM_CLASSES])
{
	// Calculate the posterior probability for each possible move
	int temp_board[NUM_POSITIONS];

	for (int pos = 0; pos < NUM_POSITIONS; pos++)
	{
		if (board[pos] == EMPTY_SYMBOL)
		{
			temp_board[pos] = EMPTY;
		}
		else if (board[pos] == X_SYMBOL)
		{
			temp_board[pos] = X;
		}
		else if (board[pos] == O_SYMBOL)
		{
			temp_board[pos] = O;
		}
	}

	double posteriors[NUM_POSITIONS] = {0.0};

	for (int pos = 0; pos < NUM_POSITIONS; pos++)
	{
		if (temp_board[pos] == EMPTY)
		{
			for (int cls = 0; cls < NUM_CLASSES; cls++)
			{
				posteriors[pos] += priors[cls];
				posteriors[pos] *= likelihoods[pos][temp_board[pos]][cls];
			}
		}
	}

	// Find the move with the highest posterior probability
	int best_move = 0;
	double max_posterior = posteriors[0];

	for (int pos = 1; pos < NUM_POSITIONS; pos++)
	{
		if (temp_board[pos] == EMPTY && posteriors[pos] > max_posterior)
		{
			max_posterior = posteriors[pos];
			best_move = pos;
		}
	}

	return best_move;
}

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

// Test split_dataset function. Should return training.data and testing.data with respective instances and outcomes according to split ratio.
/* void test_split_data(int training_data[MAX_ROWS][NUM_POSITIONS], int training_labels[MAX_ROWS],
						int testing_data[MAX_ROWS][NUM_POSITIONS], int testing_labels[MAX_ROWS]) {

	FILE *file = fopen("training.data", "w");
	for (int i = 0; i < 766; i++) {
		char temp[NUM_POSITIONS];
		for (int j = 0; j < NUM_POSITIONS; j++) {
			if (training_data[i][j] == X)
			{
				temp[j] = 'x';
			}
			else if (training_data[i][j] == O)
			{
				temp[j] = 'o';
			}
			else {
				temp[j] = 'b';
			}
		}
		fprintf(file,"%c,%c,%c,%c,%c,%c,%c,%c,%c,%d\n",
			temp[0], temp[1], temp[2],
			temp[3], temp[4], temp[5],
			temp[6], temp[7], temp[8],
			training_labels[i]);
	}
	fclose(file);

	FILE *file2 = fopen("testing.data", "w");
	for (int i = 0; i < 192; i++) {
		char temp[NUM_POSITIONS];
		for (int j = 0; j < NUM_POSITIONS; j++) {
			if (testing_data[i][j] == X)
			{
				temp[j] = 'x';
			}
			else if (testing_data[i][j] == O)
			{
				temp[j] = 'o';
			}
			else {
				temp[j] = 'b';
			}
		}
		fprintf(file2,"%c,%c,%c,%c,%c,%c,%c,%c,%c,%d\n",
			temp[0], temp[1], temp[2],
			temp[3], temp[4], temp[5],
			temp[6], temp[7], temp[8],
			training_labels[i]);
	}
	fclose(file2);
} */