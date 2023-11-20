#include "ui.h"
#include "board.h"
#include "pvp_offline_game.h"

int pvpOfflineGame()
{
	char board[9] = {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}; // computer squares are x, player squares are o, empty squares are b
	int turn = 0;
	int move = 0;
	char player = X_SYMBOL;

	drawBoard(board);

	for (turn = 0; turn < 9 && win(board) == EMPTY_SYMBOL; ++turn) // loop until turns finish or there is no win
	{
		move = playerMove(player, board);
		board[move] = player;
		player = player == X_SYMBOL ? O_SYMBOL : X_SYMBOL;

		drawBoard(board); // redraw board after changes
	}

	char winner = win(board); // check winner once got winner or out of turns

	drawBoard(board); // redraw board after changes

	if (winner == X_SYMBOL)
	{																											  // check winner type
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Player 1 (X) is the winner!", window); // alert user on winner
		return SUCCESS;
	}
	else if (winner == O_SYMBOL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Player 2 (O) is the winner!", window);
		return SUCCESS;
	}
	else
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The game ended in a draw...", window);
		return SUCCESS;
	}

	return -1;
}