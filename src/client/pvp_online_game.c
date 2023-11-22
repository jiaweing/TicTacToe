#include "ui.h"
#include "board.h"
#include "server_responses.h"
#include "pvp_online_game.h"

int pvpOnlineGame(const char *hostname, int hostportno)
{
	// back button
	int sockfd = connect_to_server(hostname, hostportno);
	if (sockfd == ERROR)
	{
		clearScreen();
		return ERROR;
	}
	int id = recv_server_int(sockfd);
	if (id < 0)
	{
		clearScreen();
		return ERROR;
	}
	char msg[4];

	int playerFound = 0;

	char board[9] = {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}; // computer squares are x, player squares are o, empty squares are b
	const char player = id % 2 ? X_SYMBOL : O_SYMBOL;

	SDL_Event event;

	while (1)
    {
		while (SDL_PollEvent(&event))
        {
			if (event.type == SDL_QUIT)
			{
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
					white
				);
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
					white
				);
				SDL_RenderPresent(renderer);
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
					white
				);
				SDL_RenderPresent(renderer);
			}
			else if (!strcmp(msg, INVALID_MOVE))
			{
				clearScreen();
				drawBoard(board);
				printf("That position has already been played. Try again.\n");
			}
			else if (!strcmp(msg, PLAYER_COUNT))
			{
				int num_players = recv_server_int(sockfd);
				printf("There are currently %d active players.\n", num_players);
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
					white
				);
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
					white
				);
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
			else
			{
				error("Unknown message.");
			}
		}

		usleep(100);
	}

	close(sockfd);
	clearScreen();
	return SUCCESS;
}

int connect_to_server(const char *hostname, int hostportno)
{
	struct hostent *server;
	struct sockaddr_in serv_addr;
	int sockfd;

	#ifdef _WIN32
	WSADATA wsaData;
	
	int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        return 1;
    }
	#endif

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket for server.");
		return ERROR;
	}

	server = gethostbyname(hostname);
	if (server == NULL)
	{
		fprintf(stderr, "ERROR, no such host\n");
		return ERROR;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	memmove(server->h_addr, &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(hostportno);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{
		error("ERROR connecting to server");
		return ERROR;
	}

	printf("[DEBUG] Connected to server.\n");
	return sockfd;
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

int recv_server_msg(int sockfd, char *msg)
{
	memset(msg, 0, 4);
	int n = read(sockfd, msg, 3);

	if (n < 0 || n != 3)
	{
		error("ERROR reading message from server socket.");
		return ERROR;
	}

	printf("[DEBUG] Received message: %s\n", msg);
	return SUCCESS;
}

int recv_server_int(int sockfd)
{
	int msg = 0;
	int n = read(sockfd, &msg, sizeof(int));

	if (n < 0 || n != sizeof(int))
	{
		error("ERROR reading int from server socket.");
		return ERROR;
	}

	printf("[DEBUG] Received int: %i\n", msg);
	return msg;
}

int write_server_int(int sockfd, int msg)
{
	int n = write(sockfd, &msg, sizeof(int));
	if (n < 0)
	{
		error("ERROR writing int to server socket.");
		return ERROR;
	}

	printf("DEBUG] Wrote int to server: %i\n", msg);
	return SUCCESS;
}

void error(const char *msg)
{
	perror(msg);
	printf("Either the server shut down or the other player disconnected.\nGame over.\n");
}