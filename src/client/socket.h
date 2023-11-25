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

#define ERROR -1
#define SUCCESS 1

int prepare_socket();
int connect_to_server(const int sockfd, const char *hostname, const int hostportno);
int get_move(int sockfd, const char player, char board[9]);
void get_update(int sockfd, char board[9]);
int recv_server_msg(int sockfd, char *msg);
int recv_server_int(int sockfd);
int write_server_int(int sockfd, int msg);
void error(const char *msg);