#include "socket.h"

// Function to create a socket
int prepare_socket()
{
	int sockfd;

	// Create a socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket for server.");  // Print an error message
		return ERROR;
	}

	return sockfd;  // Return the created socket file descriptor
}

// Function to connect to the server
int connect_to_server(const int sockfd, const char *hostname, const int hostportno)
{
	struct hostent *server;
	struct sockaddr_in serv_addr;

	// Get host information using the provided hostname
	server = gethostbyname(hostname);
	if (server == NULL)
	{
		error("ERROR no such host.");  // Print an error message
		return ERROR;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));  // Clear the serv_addr structure

	serv_addr.sin_family = AF_INET;  // Set the address family to IPv4
	memmove(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);  // Copy the host address
	serv_addr.sin_port = htons(hostportno);  // Set the port number

	// Connect to the server
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		error("ERROR connecting to server");  // Print an error message
		return ERROR;
	}

	printf("[DEBUG] Connected to server.\n");  // Print a debug message
	return sockfd;  // Return the connected socket file descriptor
}

// Function to receive a message from the server
int recv_server_msg(int sockfd, char *msg)
{
	memset(msg, 0, 4);  // Clear the message buffer
	int n = read(sockfd, msg, 3);  // Read a message from the server

	if (n < 0 || n != 3)
	{
		error("ERROR reading message from server socket.");  // Print an error message
		return ERROR;
	}

	printf("[DEBUG] Received message: %s\n", msg);  // Print a debug message
	return SUCCESS;  // Return success
}

// Function to receive an integer from the server
int recv_server_int(int sockfd)
{
	int msg = 0;
	int n = read(sockfd, &msg, sizeof(int));  // Read an integer from the server

	if (n < 0 || n != sizeof(int))
	{
		error("ERROR reading int from server socket.");  // Print an error message
		return ERROR;
	}

	printf("[DEBUG] Received int: %i\n", msg);  // Print a debug message
	return msg;  // Return the received integer
}

// Function to write an integer to the server
int write_server_int(int sockfd, int msg)
{
	int n = write(sockfd, &msg, sizeof(int));  // Write an integer to the server

	if (n < 0)
	{
		error("ERROR writing int to server socket.");  // Print an error message
		return ERROR;
	}

	printf("[DEBUG] Wrote int to server: %i\n", msg);  // Print a debug message
	return SUCCESS;  // Return success
}

// Function to handle errors
void error(const char *msg)
{
	perror(msg);  // Print an error message
	printf("Either the server shut down or the other player disconnected.\nGame over.\n");  // Print additional information
}
