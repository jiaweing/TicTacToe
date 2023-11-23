#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#ifdef __APPLE__
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <ws2ipdef.h>
#endif

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#include "board.h"

#define DEFAULT_PORT 27428

int player_count;
pthread_mutex_t mutexcount;

int setup_listener(int portno);
void get_clients(int lis_sockfd, int *cli_sockfd);
void *run_game(void *thread_data);
int get_player_move(int cli_sockfd);
void send_update(int *cli_sockfd, int move, int player_id);
void send_player_count(int cli_sockfd);
int recv_clients_int(int cli_sockfd);
void write_clients_int(int *cli_sockfd, int msg);
void write_clients_msg(int *cli_sockfd, char *msg);
void write_client_int(int cli_sockfd, int msg);
void write_client_msg(int cli_sockfd, char *msg);
void server_draw_board(char board[9]);
void server_error(const char *msg);