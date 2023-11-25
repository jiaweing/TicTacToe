#include "socket.h"

int prepare_socket()
{
	int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket for server.");
		return ERROR;
	}

	return sockfd;
}

int connect_to_server(const int sockfd, const char *hostname, const int hostportno)
{
	struct hostent *server;
	struct sockaddr_in serv_addr;

	server = gethostbyname(hostname);
	if (server == NULL)
	{
		error("ERROR no such host.");
		return ERROR;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	memmove(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
	serv_addr.sin_port = htons(hostportno);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		error("ERROR connecting to server");
		return ERROR;
	}

	printf("[DEBUG] Connected to server.\n");
	return sockfd;
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