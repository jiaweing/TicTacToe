#include "server.h"
#include "server_responses.h"

int player_count = 0;  // Global variable to keep track of the number of players

int main(int argc, char *argv[])
{
    int port;

    if (argc < 2)
    {
        port = DEFAULT_PORT;  // Use default port if not provided in command line arguments
    }
    else
    {
        port = atoi(argv[1]);  // Convert the provided port from string to integer
    }

    int lis_sockfd = setup_listener(port);  // Set up the listener socket
    pthread_mutex_init(&mutexcount, NULL);  // Initialize mutex for player_count

    printf("Server running on port %d...\n", port);

    while (1)
    {
        if (player_count <= 252)  // Limit the number of players
        {
            int *cli_sockfd = (int *)malloc(2 * sizeof(int));  // Allocate memory for client socket descriptors
            memset(cli_sockfd, 0, 2 * sizeof(int));  // Initialize memory to 0

            get_clients(lis_sockfd, cli_sockfd);  // Accept and handle client connections

            #ifdef DEBUG
            printf("[DEBUG] Starting new game thread...\n");
            #endif

            pthread_t thread;
            int result = pthread_create(&thread, NULL, run_game, (void *)cli_sockfd);  // Create a new thread for the game
            if (result)
            {
                printf("ERROR Thread creation failed with return code %d\n", result);
                exit(-1);  // Exit the program if thread creation fails
            }

            #ifdef DEBUG
            printf("[DEBUG] New game thread started.\n");
            #endif
        }
    }

    close(lis_sockfd);  // Close the listener socket

    pthread_mutex_destroy(&mutexcount);  // Destroy the mutex
    pthread_exit(NULL);  // Exit the main thread
}

int setup_listener(int portno)
{
    int sockfd;
    struct sockaddr_in serv_addr;

    #ifdef _WIN32
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);  // Initialize Winsock on Windows
    if (result != 0)
    {
        printf("WSAStartup failed with error: %d\n", result);
        server_error("ERROR WSAStartup failed.");
    }
    #endif

    sockfd = socket(AF_INET, SOCK_STREAM, 0);  // Create a socket
    if (sockfd < 0)
    {
        server_error("ERROR opening listener socket.");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));  // Initialize memory for server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);  // Set up the server address

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  // Bind the socket to the server address
    {
        server_error("ERROR binding listener socket.");
    }

    #ifdef DEBUG
    printf("[DEBUG] Listener set.\n");
    #endif

    return sockfd;  // Return the listener socket descriptor
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
        listen(lis_sockfd, 253 - player_count);  // Listen for incoming connections with a backlog based on available player slots

        memset(&cli_addr, 0, sizeof(cli_addr));  // Initialize memory for client address

        clilen = sizeof(cli_addr);

        cli_sockfd[num_conn] = accept(lis_sockfd, (struct sockaddr *)&cli_addr, &clilen);  // Accept a connection from a client

        if (cli_sockfd[num_conn] < 0)
        {
            server_error("ERROR accepting a connection from a client.");
        }

        #ifdef DEBUG
        printf("[DEBUG] Accepted connection from client %d\n", num_conn);
        #endif

        write(cli_sockfd[num_conn], &num_conn, sizeof(int));  // Send the client its ID

        #ifdef DEBUG
        printf("[DEBUG] Sent client %d its ID.\n", num_conn);
        #endif

        pthread_mutex_lock(&mutexcount);  // Lock the mutex to safely modify player_count
        player_count++;  // Increment the number of players
        printf("Number of players is now %d.\n", player_count);
        pthread_mutex_unlock(&mutexcount);  // Unlock the mutex

        if (num_conn == 0)
        {
            write_client_msg(cli_sockfd[0], HOLD);  // Tell the first client to hold

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

    write_clients_msg(cli_sockfd, START);  // Notify clients that the game is starting

    #ifdef DEBUG
    printf("[DEBUG] Sent start message.\n");
    #endif

    server_draw_board(board);  // Display the initial game board

    for (turn = 0; turn < 9 && win(board) == EMPTY_SYMBOL; ++turn) // loop until turns finish or there is no win
    {
        if (player != prev_player)
        {
            write_client_msg(cli_sockfd[(player + 1) % 2], WAIT);  // Notify the other player to wait
        }

        int valid = 0;
        int move = 0;

        while (!valid)
        {
            move = get_player_move(cli_sockfd[player]);  // Get the player's move
            if (move == -1)
            {
                printf("Player disconnected.\n");
                turn = -1;
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
                write_client_msg(cli_sockfd[player], INVALID_MOVE);  // Notify the player of an invalid move
            }
        }

        if (move == -1)
        {
            break;
        }
        else if (move == 9)
        {
            prev_player = player;
            send_player_count(cli_sockfd[player]);  // Send player count to the current player
        }
        else
        {
            board[move] = player ? X_SYMBOL : O_SYMBOL;  // Update the game board with the player's move
            send_update(cli_sockfd, move, player);  // Send an update to both players
            server_draw_board(board);  // Display the updated game board

            prev_player = player;
            player = (prev_player + 1) % 2;  // Switch players
        }
    }

    if (turn == -1)
    {
        write_clients_msg(cli_sockfd, GAME_INTERRUPTED);  // Notify clients of game interruption
    }
    else
    {
        char winner = win(board); // check winner once got winner or out of turns   
        if (winner == X_SYMBOL || winner == O_SYMBOL)
        {
            write_client_msg(cli_sockfd[prev_player], GAME_WIN);  // Notify the winning player
            write_client_msg(cli_sockfd[(prev_player + 1) % 2], GAME_LOSE);  // Notify the losing player
        }
        else
        {
            write_clients_msg(cli_sockfd, GAME_DRAW);  // Notify both players of a draw
        }
    }

    printf("Game over.\n");

    close(cli_sockfd[0]);  // Close client sockets
    close(cli_sockfd[1]);

    pthread_mutex_lock(&mutexcount);
    player_count--;
    printf("Number of players is now %d.\n", player_count);
    player_count--;
    printf("Number of players is now %d.\n", player_count);
    pthread_mutex_unlock(&mutexcount);

    free(cli_sockfd);  // Free allocated memory for client socket descriptors

    pthread_exit(NULL);  // Exit the game thread
}

int get_player_move(int cli_sockfd)
{
    #ifdef DEBUG
    printf("[DEBUG] Getting player move...\n");
    #endif

    write_client_msg(cli_sockfd, PLAYER_TURN);  // Notify the player that it's their turn

    return recv_clients_int(cli_sockfd);  // Receive the player's move from the client
}

void send_update(int *cli_sockfd, int move, int player_id)
{
    #ifdef DEBUG
    printf("[DEBUG] Sending update...\n");
    #endif

    write_clients_msg(cli_sockfd, UPDATE_BOARD);  // Notify both players that the board is being updated
    write_clients_int(cli_sockfd, player_id);  // Send the player ID
    write_clients_int(cli_sockfd, move);  // Send the move

    #ifdef DEBUG
    printf("[DEBUG] Update sent.\n");
    #endif
}

void send_player_count(int cli_sockfd)
{
    write_client_msg(cli_sockfd, PLAYER_COUNT);  // Notify the player of the current player count
    write_client_int(cli_sockfd, player_count);  // Send the player count
    #ifdef DEBUG
    printf("[DEBUG] Player Count Sent.\n");
    #endif
}

int recv_clients_int(int cli_sockfd)
{
    int msg = 0;
    int n = read(cli_sockfd, &msg, sizeof(int));  // Read an integer from the client

    if (n < 0 || n != sizeof(int))
    {
        return -1;  // Return -1 if there was an error reading or if the expected size was not received
    }

    #ifdef DEBUG
    printf("[DEBUG] Received int: %d\n", msg);
    #endif

    return msg;  // Return the received integer
}

void write_clients_int(int *cli_sockfd, int msg)
{
    write_client_int(cli_sockfd[0], msg);  // Write an integer to the first client
    write_client_int(cli_sockfd[1], msg);  // Write an integer to the second client
}

void write_clients_msg(int *cli_sockfd, char *msg)
{
    write_client_msg(cli_sockfd[0], msg);  // Write a message to the first client
    write_client_msg(cli_sockfd[1], msg);  // Write a message to the second client
}

void write_client_int(int cli_sockfd, int msg)
{
    int n = write(cli_sockfd, &msg, sizeof(int));  // Write an integer to the client socket
    if (n < 0)
    {
        server_error("ERROR writing int to client socket");  // Handle error if writing fails
    }
}

void write_client_msg(int cli_sockfd, char *msg)
{
    int n = write(cli_sockfd, msg, strlen(msg));  // Write a message to the client socket
    if (n < 0)
    {
        server_error("ERROR writing msg to client socket");  // Handle error if writing fails
    }
}

void server_draw_board(char board[9])
{
    printf(" %c | %c | %c \n", board[0], board[1], board[2]);  // Display the Tic Tac Toe board
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[3], board[4], board[5]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[6], board[7], board[8]);
}

void server_error(const char *msg)
{
    perror(msg);  // Print an error message
    pthread_exit(NULL);  // Exit the thread
}

