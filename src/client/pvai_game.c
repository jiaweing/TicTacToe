#include "ui.h"
#include "board.h"
#include "game_logic.h"
#include "pvai_game.h"

int pvaiGame(int difficulty)
{
	char board[9] = {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}; // computer squares are x, player squares are o, empty squares are b
	int turn = 0;
	int move = 0;
	char player = X_SYMBOL;

	drawAIGameScreen(board, player);

	int data[MAX_ROWS][NUM_POSITIONS];
	int labels[MAX_ROWS];
	int training_data[MAX_TRAINING_ROWS][NUM_POSITIONS];
	int training_labels[MAX_TRAINING_ROWS];
	int testing_data[MAX_TESTING_ROWS][NUM_POSITIONS];
	int testing_labels[MAX_TESTING_ROWS];
	int split_ratio = 80;
	double priors[NUM_CLASSES];
	double likelihoods[NUM_POSITIONS][EMPTY + 1][NUM_CLASSES];

	extract_data(data, labels);
	split_data(data, labels, training_data, training_labels, testing_data, testing_labels, split_ratio);

	// NaiveBayes Train Model and Cross Validation
	validate(training_data, training_labels, testing_data, testing_labels, likelihoods, priors);

	for (turn = 0; turn < 9 && win(board) == EMPTY_SYMBOL; ++turn)
	{
		if (player == X_SYMBOL)
		{
			move = computerMove(X_SYMBOL, board, difficulty, priors, likelihoods);
			board[move] = X_SYMBOL;
			player = O_SYMBOL;
		}
		else
		{
			move = playerMove(O_SYMBOL, board);
			board[move] = O_SYMBOL;
			player = X_SYMBOL;
		}

		drawAIGameScreen(board, player);
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

	clearScreen();
	return SUCCESS;
}

int computerMove(char symbol, char board[9], int difficulty, double priors[NUM_CLASSES], double likelihoods[NUM_POSITIONS][EMPTY + 1][NUM_CLASSES])
{
	if (difficulty == EASY_DIFFICULTY || difficulty == MEDIUM_DIFFICULTY)
	{
		int next_move = predictNextMove(difficulty, board, priors, likelihoods);
		return next_move;
	}
	else if (difficulty == HARD_DIFFICULTY || difficulty == IMPOSSIBLE_DIFFICULTY)
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
		return move;
	}
	else
	{
		return -1;
	}
}

void drawAIGameScreen(char board[9], char player)
{
	clearScreen();
	drawBoard(board);
	renderText(
		"Current Player",
		pcsenior18_f,
		BOARD_STATUS_PADDING,
		BOARD_STATUS_PADDING,
		white);
	renderText(
		player == X_SYMBOL ? "X (Computer)" : "O (You)",
		pcsenior24_f,
		BOARD_STATUS_PADDING,
		BOARD_STATUS_PADDING + 30,
		player == X_SYMBOL ? red : blue);
	SDL_RenderPresent(renderer);
}