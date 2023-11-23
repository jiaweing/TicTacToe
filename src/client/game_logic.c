#include "board.h"
#include "naivebayes.h"
#include "game_logic.h"

// Function to predict the next move based on Naive Bayes
int predictNextMove(int difficulty_level, char char_board[NUM_POSITIONS], double priors[NUM_CLASSES], double likelihoods[NUM_POSITIONS][EMPTY+1][NUM_CLASSES]) {
    int int_board[NUM_POSITIONS];
    double pos = 0.0;
    double neg = 0.0;
    int empty_pos[NUM_POSITIONS];
    double max_pos = 0.0;
    double min_pos = 1.0;
    int best_move;
    int result;

    changeToIntBoard(char_board, int_board);
    int possible_moves = countEmptyPositions(int_board, empty_pos);

    if (possible_moves > 1) {
        if (difficulty_level == 1) {
            // Search for game instance with lowest posterior probability for a positive outcome
            for (int j = 0; j < possible_moves; ++j) {
                int_board[empty_pos[j]] = X;
                result = naiveBayesPredict(int_board, priors, likelihoods, &pos, &neg);
                if (result == 1) {
                    if (pos < min_pos) {
                        min_pos = pos;
                        best_move = empty_pos[j];
                    }
                }
                int_board[empty_pos[j]] = EMPTY;
            }

            /* Search for game instance with highest posterior probability for a negative outcome if there is no game instance 
               whose posterior probability for a positive outcome is higher than the negative outcome */
            if (min_pos == 1.0) {
                for (int j = 0; j < possible_moves; ++j) {
                    int_board[empty_pos[j]] = X;
                    result = naiveBayesPredict(int_board, priors, likelihoods, &pos, &neg);
                    if (result == 0) {
                        if (neg > max_pos) {
                            max_pos = pos;
                            best_move = empty_pos[j];
                        }
                    }
                    int_board[empty_pos[j]] = EMPTY;
                }
            }

            return best_move;
        }

        else if (difficulty_level == 2) {
            // Search for game instance with highest posterior probability for a positive outcome
            for (int j = 0; j < possible_moves; ++j) {
                int_board[empty_pos[j]] = X;
                result = naiveBayesPredict(int_board, priors, likelihoods, &pos, &neg);
                if (result == 1) {
                    if (pos > max_pos) {
                        max_pos = pos;
                        best_move = empty_pos[j];
                    }
                }
                int_board[empty_pos[j]] = EMPTY;
            }

            /* Search for game instance with lowest posterior probability for a negative outcome if there is no game instance 
               whose posterior probability for a positive outcome is higher than the negative outcome */
            if (max_pos == 0.0) {
                for (int j = 0; j < possible_moves; ++j) {
                    int_board[empty_pos[j]] = X;
                    result = naiveBayesPredict(int_board, priors, likelihoods, &pos, &neg);
                    if (result == 0) {
                        if (neg < min_pos) {
                            min_pos = pos;
                            best_move = empty_pos[j];
                        }
                    }
                    int_board[empty_pos[j]] = EMPTY;
                }
            }

            return best_move;
        } 
    }
    
    return empty_pos[0];

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
