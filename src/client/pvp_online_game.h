#include <stdio.h>

#define SERVER_HOSTNAME "127.0.0.1"
#define SERVER_PORT 27428

int pvpOnlineGame(const char *hostname, int hostportno);
int get_move(int sockfd, const char player, char board[9]);
void get_update(int sockfd, char board[9]);