#include "ui.h"
#include "board.h"
#include "pvp_offline_game.h"

// Function to handle the player vs player offline Tic Tac Toe game
int pvpOfflineGame()
{
	// Initialize the game board with empty squares
	char board[9] = {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}; // computer squares are x, player squares are o, empty squares are b
	int turn = 0;												   // Variable to track the current turn
	int move = 0;												   // Variable to store the current player's move
	char player = X_SYMBOL;										   // Variable to represent the current player (X or O)

	// Draw the initial game screen
	drawOfflineGameScreen(board, player);

	// Loop through the turns until there is a winner or no more turns left
	for (turn = 0; turn < 9 && win(board) == EMPTY_SYMBOL; ++turn)
	{
		// Get the current player's move
		move = playerMove(player, board);
		// Update the game board with the player's move
		board[move] = player;
		// Switch to the other player for the next turn
		player = player == X_SYMBOL ? O_SYMBOL : X_SYMBOL;

		// Draw the updated game screen
		drawOfflineGameScreen(board, player);
	}

	// Check for the winner once the game is over
	char winner = win(board);

	// Display the result of the game to the user
	if (winner == X_SYMBOL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Player 1 (X) is the winner!", window);
	}
	else if (winner == O_SYMBOL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Player 2 (O) is the winner!", window);
	}
	else
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The game ended in a draw...", window);
	}

	// Clear the screen and return success
	clearScreen();
	return SUCCESS;
}

// Function to draw the offline game screen, including the current player's turn indication
void drawOfflineGameScreen(char board[9], char player)
{
	// Clear the screen
	clearScreen();
	// Draw the game board
	drawBoard(board);
	// Render text indicating the current player
	renderText(
		"Current Player",
		pcsenior18_f,
		BOARD_STATUS_PADDING,
		BOARD_STATUS_PADDING,
		white);
	// Render the X or O representing the current player
	renderText(
		player == X_SYMBOL ? "X" : "O",
		pcsenior24_f,
		BOARD_STATUS_PADDING,
		BOARD_STATUS_PADDING + 30,
		player == X_SYMBOL ? red : blue);
	// Update the SDL renderer
	SDL_RenderPresent(renderer);
}
