#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

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

#define SERVER_HOSTNAME "127.0.0.1"
#define SERVER_PORT 27428

int pvpOnlineGame(const char *hostname, int hostportno);
int connect_to_server(const char *hostname, int hostportno);
int get_move(int sockfd, const char player, char board[9]);
void get_update(int sockfd, char board[9]);
int recv_server_msg(int sockfd, char *msg);
int recv_server_int(int sockfd);
int write_server_int(int sockfd, int msg);
void error(const char *msg);