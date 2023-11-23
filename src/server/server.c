#include "server.h"
#include "server_responses.h"

int player_count = 0;

int main(int argc, char *argv[])
{
	int port;

	if (argc < 2)
	{
		port = DEFAULT_PORT;
	}
	else
	{
		port = atoi(argv[1]);
	}

	int lis_sockfd = setup_listener(port);
	pthread_mutex_init(&mutexcount, NULL);

	printf("Server running on port %d...\n", port);

	while (1)
	{
		if (player_count <= 252)
		{
			int *cli_sockfd = (int *)malloc(2 * sizeof(int));
			memset(cli_sockfd, 0, 2 * sizeof(int));

			get_clients(lis_sockfd, cli_sockfd);

#ifdef DEBUG
			printf("[DEBUG] Starting new game thread...\n");
#endif

			pthread_t thread;
			int result = pthread_create(&thread, NULL, run_game, (void *)cli_sockfd);
			if (result)
			{
				printf("ERROR Thread creation failed with return code %d\n", result);
				exit(-1);
			}

#ifdef DEBUG
			printf("[DEBUG] New game thread started.\n");
#endif
		}
	}

	close(lis_sockfd);

	pthread_mutex_destroy(&mutexcount);
	pthread_exit(NULL);
}

int setup_listener(int portno)
{
	int sockfd;
	struct sockaddr_in serv_addr;

#ifdef _WIN32
	WSADATA wsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("WSAStartup failed with error: %d\n", result);
		server_error("ERROR WSAStartup failed.");
	}
#endif

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		server_error("ERROR opening listener socket.");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		server_error("ERROR binding listener socket.");
	}

#ifdef DEBUG
	printf("[DEBUG] Listener set.\n");
#endif

	return sockfd;
}

void get_clients(int lis_sockfd, int *cli_sockfd)
{
	socklen_t clilen;
	struct sockaddr_in cli_addr;

#ifdef DEBUG
	printf("[DEBUG] Listening for clients...\n");
#endif

	int num_conn = 0;
	while (num_conn < 2)
	{
		listen(lis_sockfd, 253 - player_count);

		memset(&cli_addr, 0, sizeof(cli_addr));

		clilen = sizeof(cli_addr);

		cli_sockfd[num_conn] = accept(lis_sockfd, (struct sockaddr *)&cli_addr, &clilen);

		if (cli_sockfd[num_conn] < 0)
		{
			server_error("ERROR accepting a connection from a client.");
		}

#ifdef DEBUG
		printf("[DEBUG] Accepted connection from client %d\n", num_conn);
#endif

		write(cli_sockfd[num_conn], &num_conn, sizeof(int));

#ifdef DEBUG
		printf("[DEBUG] Sent client %d it's ID.\n", num_conn);
#endif

		pthread_mutex_lock(&mutexcount);
		player_count++;
		printf("Number of players is now %d.\n", player_count);
		pthread_mutex_unlock(&mutexcount);

		if (num_conn == 0)
		{
			write_client_msg(cli_sockfd[0], HOLD);

#ifdef DEBUG
			printf("[DEBUG] Told client 0 to hold.\n");
#endif
		}

		num_conn++;
	}
}

void *run_game(void *thread_data)
{
	int *cli_sockfd = (int *)thread_data;
	char board[9] = {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}; // computer squares are x, player squares are o, empty squares are b
	int turn = 0;
	int player = 0;
	int prev_player = 1;

	printf("Game on!\n");

	write_clients_msg(cli_sockfd, START);

#ifdef DEBUG
	printf("[DEBUG] Sent start message.\n");
#endif

	server_draw_board(board);

	for (turn = 0; turn < 9 && win(board) == EMPTY_SYMBOL; ++turn) // loop until turns finish or there is no win
	{
		if (player != prev_player)
		{
			write_client_msg(cli_sockfd[(player + 1) % 2], WAIT);
		}

		int valid = 0;
		int move = 0;

		while (!valid)
		{
			move = get_player_move(cli_sockfd[player]);
			if (move == -1)
			{
				break;
			}
			printf("Player %d played position %d\n", player, move);

			if (board[move] == EMPTY_SYMBOL)
			{
				valid = 1;
			}
			else
			{
				printf("Move was invalid. Let's try this again...\n");
				write_client_msg(cli_sockfd[player], INVALID_MOVE);
			}
		}

		if (move == -1)
		{
			printf("Player disconnected.\n");
			break;
		}
		else if (move == 9)
		{
			prev_player = player;
			send_player_count(cli_sockfd[player]);
		}
		else
		{
			board[move] = player ? X_SYMBOL : O_SYMBOL;
			send_update(cli_sockfd, move, player);
			server_draw_board(board);

			prev_player = player;
			player = (prev_player + 1) % 2;
		}
	}

	char winner = win(board); // check winner once got winner or out of turns

	if (winner == X_SYMBOL || winner == O_SYMBOL)
	{
		write_client_msg(cli_sockfd[prev_player], GAME_WIN);
		write_client_msg(cli_sockfd[(prev_player + 1) % 2], GAME_LOSE);
	}
	else
	{
		write_clients_msg(cli_sockfd, GAME_DRAW);
	}

	printf("Game over.\n");

	close(cli_sockfd[0]);
	close(cli_sockfd[1]);

	pthread_mutex_lock(&mutexcount);
	player_count--;
	printf("Number of players is now %d.\n", player_count);
	player_count--;
	printf("Number of players is now %d.\n", player_count);
	pthread_mutex_unlock(&mutexcount);

	free(cli_sockfd);

	pthread_exit(NULL);
}

int get_player_move(int cli_sockfd)
{
#ifdef DEBUG
	printf("[DEBUG] Getting player move...\n");
#endif

	write_client_msg(cli_sockfd, PLAYER_TURN);

	return recv_clients_int(cli_sockfd);
}

void send_update(int *cli_sockfd, int move, int player_id)
{
#ifdef DEBUG
	printf("[DEBUG] Sending update...\n");
#endif

	write_clients_msg(cli_sockfd, UPDATE_BOARD);
	write_clients_int(cli_sockfd, player_id);
	write_clients_int(cli_sockfd, move);

#ifdef DEBUG
	printf("[DEBUG] Update sent.\n");
#endif
}

void send_player_count(int cli_sockfd)
{
	write_client_msg(cli_sockfd, PLAYER_COUNT);
	write_client_int(cli_sockfd, player_count);

#ifdef DEBUG
	printf("[DEBUG] Player Count Sent.\n");
#endif
}

int recv_clients_int(int cli_sockfd)
{
	int msg = 0;
	int n = read(cli_sockfd, &msg, sizeof(int));

	if (n < 0 || n != sizeof(int))
	{
		return -1;
	}

	printf("[DEBUG] Received int: %d\n", msg);

	return msg;
}

void write_clients_int(int *cli_sockfd, int msg)
{
	write_client_int(cli_sockfd[0], msg);
	write_client_int(cli_sockfd[1], msg);
}

void write_clients_msg(int *cli_sockfd, char *msg)
{
	write_client_msg(cli_sockfd[0], msg);
	write_client_msg(cli_sockfd[1], msg);
}

void write_client_int(int cli_sockfd, int msg)
{
	int n = write(cli_sockfd, &msg, sizeof(int));
	if (n < 0)
	{
		server_error("ERROR writing int to client socket");
	}
}

void write_client_msg(int cli_sockfd, char *msg)
{
	int n = write(cli_sockfd, msg, strlen(msg));
	if (n < 0)
	{
		server_error("ERROR writing msg to client socket");
	}
}

void server_draw_board(char board[9])
{
	printf(" %c | %c | %c \n", board[0], board[1], board[2]);
	printf("-----------\n");
	printf(" %c | %c | %c \n", board[3], board[4], board[5]);
	printf("-----------\n");
	printf(" %c | %c | %c \n", board[6], board[7], board[8]);
}

void server_error(const char *msg)
{
	perror(msg);
	pthread_exit(NULL);
}