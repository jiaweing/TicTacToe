#include "ui.h"
#include "board.h"
#include "game_logic.h"
#include "pvai_game.h"

int pvaiGame(int difficulty)
{
	char board[9] = {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}; // computer squares are x, player squares are o, empty squares are b
	int turn = 0;
	char player = X_SYMBOL;

	drawBoard(board);

	for (turn = 0; turn < 9 && win(board) == EMPTY_SYMBOL; ++turn)
	{
		if (player == X_SYMBOL)
		{
			computerMove(X_SYMBOL, board, difficulty);
			player = O_SYMBOL;
		}
		else
		{
			playerMove(O_SYMBOL, board);
			player = X_SYMBOL;
		}

		drawBoard(board); // redraw board after changes
	}

	char winner = win(board);

	drawBoard(board);

	if (winner == X_SYMBOL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The computer (X) is the winner!", window);
		return SUCCESS;
	}
	else if (winner == O_SYMBOL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You (O) are the winner!", window);
		return SUCCESS;
	}
	else
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The game ended in a draw...", window);
		return SUCCESS;
	}

	return -1;
}

void computerMove(char symbol, char board[9], int difficulty)
{
	if (difficulty == EASY_DIFFICULTY || difficulty == MEDIUM_DIFFICULTY || difficulty == HARD_DIFFICULTY)
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
	else if (difficulty == IMPOSSIBLE_DIFFICULTY)
	{
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
}