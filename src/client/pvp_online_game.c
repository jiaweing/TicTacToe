#include "ui.h"
#include "board.h"
#include "socket.h"
#include "server_responses.h"
#include "pvp_online_game.h"

int pvpOnlineGame(const char *hostname, int hostportno)
{
#ifdef _WIN32
	clearScreen(); // Clear the screen for Windows
	return ERROR;  // Return an error code on Windows
#endif

	clearScreen(); // Clear the screen for non-Windows platforms

	renderAnchoredText(
		"Attempting to connect to server...",
		pcsenior24_f,
		SCREEN_WIDTH / 2,
		SCREEN_HEIGHT / 2,
		white);
	SDL_RenderPresent(renderer);

	int sockfd = prepare_socket(); // Prepare the socket for communication
	if (sockfd == ERROR)
	{
		clearScreen();
		close(sockfd);
		return ERROR;
	}
	if (connect_to_server(sockfd, hostname, hostportno) == ERROR) // connect to server using the socket, host ip address and port number
	{
		clearScreen();
		close(sockfd);
		return ERROR;
	}
	int id = recv_server_int(sockfd); // Receive the player ID from the server
	if (id < 0)
	{
		clearScreen();
		close(sockfd);
		return ERROR;
	}

	char msg[4];		 // buffer for messages from the server
	int playerFound = 0; // flag for whether an opponentis found on the server

	char board[9] = {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}; // Initialize the game board
	const char player = id % 2 ? X_SYMBOL : O_SYMBOL;			   // Determine the player symbol based on the player ID

	SDL_Event event;

	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				close(sockfd);
				clearSDL();
				exit(0);
				break;
			}
		}

		if (recv_server_msg(sockfd, msg) == ERROR) // if error recieving message, quit
		{
			clearScreen();
			close(sockfd);
			return ERROR;
		}

		if (!playerFound)
		{
			// Handling messages before opponent is found

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
			// Handling messages after opponent is found

			if (!strcmp(msg, PLAYER_TURN))
			{
				// If the received message indicates it's the player's turn
				get_move(sockfd, player, board); // Get the player's move
				clearScreen();					 // Clear the game screen
				drawBoard(board);				 // Draw the updated game board
				renderText(
					"Your turn!",
					pcsenior24_f,
					BOARD_STATUS_PADDING,
					BOARD_STATUS_PADDING,
					white);					 // Render a text indicating it's the player's turn
				SDL_RenderPresent(renderer); // Update the SDL renderer
			}
			else if (!strcmp(msg, UPDATE_BOARD))
			{
				// If the received message indicates an update to the game board
				get_update(sockfd, board); // Update the local game board based on the server's message
				clearScreen();			   // Clear the game screen
				drawBoard(board);		   // Draw the updated game board
				renderText(
					"Your turn!",
					pcsenior24_f,
					BOARD_STATUS_PADDING,
					BOARD_STATUS_PADDING,
					white);					 // Render a text indicating it's the player's turn
				SDL_RenderPresent(renderer); // Update the SDL renderer
			}
			else if (!strcmp(msg, WAIT))
			{
				// If the received message indicates the player should wait for the opponent
				clearScreen();	  // Clear the game screen
				drawBoard(board); // Draw the current game board
				renderText(
					"Waiting for opponent...",
					pcsenior24_f,
					BOARD_STATUS_PADDING,
					BOARD_STATUS_PADDING,
					white);					 // Render a text indicating the player is waiting
				SDL_RenderPresent(renderer); // Update the SDL renderer
			}
			else if (!strcmp(msg, GAME_WIN))
			{
				// If the received message indicates the player has won
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You won the match!", window); // Display a message box indicating the player has won
				break;																							 // Exit the loop, ending the game
			}
			else if (!strcmp(msg, GAME_LOSE))
			{
				// If the received message indicates the player has lost
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You lost the match.", window); // Display a message box indicating the player has lost
				break;																							  // Exit the loop, ending the game
			}
			else if (!strcmp(msg, GAME_DRAW))
			{
				// If the received message indicates the game has ended in a draw
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The match ended in a draw...", window); // Display a message box indicating a draw
				break;																									   // Exit the loop, ending the game
			}
			else if (!strcmp(msg, GAME_INTERRUPTED))
			{
				// If the received message indicates the opponent has quit the game
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Your opponent rage quit...", window); // Display a message box indicating the opponent has quit
				break;																									 // Exit the loop, ending the game
			}
			else
			{
				// If the received message is unknown
				error("Unknown message."); // Print an error message
				clearScreen();			   // Clear the game screen
				close(sockfd);			   // Close the socket
				return ERROR;			   // Return an error code
			}
		}

		usleep(100); // Introduce a small delay
	}

	close(sockfd);
	clearScreen();
	return SUCCESS;
}

int get_move(int sockfd, const char player, char board[9])
{
	int move = playerMove(player, board); // Get the player's move
	board[move] = player;				  // Update the board with the player's move
	write_server_int(sockfd, move);		  // Send the move to the server
	return move;
}

void get_update(int sockfd, char board[9])
{
	int player_id = recv_server_int(sockfd);	   // Receive player ID from the server
	int move = recv_server_int(sockfd);			   // Receive the move from the server
	board[move] = player_id ? X_SYMBOL : O_SYMBOL; // Update the board
}