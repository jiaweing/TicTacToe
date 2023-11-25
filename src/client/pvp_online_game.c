#include "ui.h"
#include "board.h"
#include "socket.h"
#include "server_responses.h"
#include "pvp_online_game.h"

int pvpOnlineGame(const char *hostname, int hostportno)
{
	clearScreen();
	renderAnchoredText(
		"Attempting to connect to server...",
		pcsenior24_f,
		SCREEN_WIDTH / 2,
		SCREEN_HEIGHT / 2,
		white);
	SDL_RenderPresent(renderer);

	int sockfd = prepare_socket();
	if (sockfd == ERROR)
	{
		clearScreen();
		close(sockfd);
		return ERROR;
	}
	if (connect_to_server(sockfd, hostname, hostportno) == ERROR)
	{
		clearScreen();
		close(sockfd);
		return ERROR;
	}
	int id = recv_server_int(sockfd);
	if (id < 0)
	{
		clearScreen();
		close(sockfd);
		return ERROR;
	}
	
	char msg[4];
	int playerFound = 0;

	char board[9] = {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}; // player 1 squares are x, player 2 squares are o, empty squares are b
	const char player = id % 2 ? X_SYMBOL : O_SYMBOL;

	SDL_Event event;

	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				close(sockfd);
				exit(0);
				break;
			}
		}

		if (recv_server_msg(sockfd, msg) == ERROR)
		{
			clearScreen();
			close(sockfd);
			return ERROR;
		}

		if (!playerFound)
		{
			if (!strcmp(msg, HOLD))
			{
				printf("Waiting for a second player...\n");
				clearScreen();
				renderAnchoredText(
					"Looking for players...",
					pcsenior24_f,
					SCREEN_WIDTH / 2,
					SCREEN_HEIGHT / 2,
					white);
				SDL_RenderPresent(renderer);
			}
			else if (!strcmp(msg, START))
			{
				playerFound = 1;
				clearScreen();
				drawBoard(board);
				renderText(
					"Game on!",
					pcsenior24_f,
					BOARD_STATUS_PADDING,
					BOARD_STATUS_PADDING,
					white);
				SDL_RenderPresent(renderer);
			}
			else
			{
				error("Unknown message.");
				clearScreen();
				close(sockfd);
				return ERROR;
			}
		}
		else
		{
			if (!strcmp(msg, PLAYER_TURN))
			{
				get_move(sockfd, player, board);
				clearScreen();
				drawBoard(board);
				renderText(
					"Your turn!",
					pcsenior24_f,
					BOARD_STATUS_PADDING,
					BOARD_STATUS_PADDING,
					white);
				SDL_RenderPresent(renderer);
			}
			else if (!strcmp(msg, UPDATE_BOARD))
			{
				get_update(sockfd, board);
				clearScreen();
				drawBoard(board);
				renderText(
					"Your turn!",
					pcsenior24_f,
					BOARD_STATUS_PADDING,
					BOARD_STATUS_PADDING,
					white);
				SDL_RenderPresent(renderer);
			}
			else if (!strcmp(msg, WAIT))
			{
				clearScreen();
				drawBoard(board);
				renderText(
					"Waiting for opponent...",
					pcsenior24_f,
					BOARD_STATUS_PADDING,
					BOARD_STATUS_PADDING,
					white);
				SDL_RenderPresent(renderer);
			}
			else if (!strcmp(msg, GAME_WIN))
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You won the match!", window);
				break;
			}
			else if (!strcmp(msg, GAME_LOSE))
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You lost the match.", window);
				break;
			}
			else if (!strcmp(msg, GAME_DRAW))
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The match ended in a draw...", window);
				break;
			}
			else if (!strcmp(msg, GAME_INTERRUPTED))
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Your opponent rage quit...", window);
				break;
			}
			else
			{
				error("Unknown message.");
				clearScreen();
				close(sockfd);
				return ERROR;
			}
		}

		usleep(100);
	}

	close(sockfd);
	clearScreen();
	return SUCCESS;
}

int get_move(int sockfd, const char player, char board[9])
{
	int move = playerMove(player, board);
	board[move] = player;
	write_server_int(sockfd, move);
	return move;
}

void get_update(int sockfd, char board[9])
{
	int player_id = recv_server_int(sockfd);
	int move = recv_server_int(sockfd);
	board[move] = player_id ? X_SYMBOL : O_SYMBOL;
}