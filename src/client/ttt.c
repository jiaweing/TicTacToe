// TODO: validation for input
// TODO: undo button, timer, preview move with half opacity w/ blinks or animation, audio, win animation, windows compatibility, window name icon

#include "ui.h"
#include "main_menu.h"
#include "pvp_offline_game.h"
#include "pvp_online_game.h"
#include "pvai_game.h"

int initialiseSDL();
int main(int argv, char **args)
{
	if (initialiseSDL() == ERROR) // Check if SDL initialization is unsuccessful
	{
		printf("There has been an error with initialising SDL.\n");
		exit(0);
	}

	while (1)
	{
		// Enter into an infinite loop for the main menu
		int gameType = mainMenu();  // Display and get the user's choice from the main menu
		if (gameType == PVP_GAME)
		{
			// If the user chooses Player vs. Player mode
			int response = pvpMenu();  // Display and get the user's choice from the PvP menu
			if (response == BACK)
			{
				// If the user chooses to go back to the main menu
				continue;  // Skip the rest of the loop and go back to the main menu
			}
			else if (response == ONLINE_GAME) // If the user chooses online PvP game
			{
				char ip[15] = SERVER_HOSTNAME; // Declare a variable for containing the server IP address
				int port = SERVER_PORT; // Declare a variable for containing the server port number

				if (askForHostIP(ip) == BACK)
				{
					// If the user chooses to go back during IP input
					continue;
				}
				if (askForHostPort(&port) == BACK)
				{
					// If the user chooses to go back during port input
					continue;
				}
				if (pvpOnlineGame(ip, port) == ERROR)
				{
					// If connecting to the server for online PvP fails
					renderText(
						"Couldn't connect to server",
						pcsenior24_f,
						BOARD_STATUS_PADDING,
						SCREEN_HEIGHT - BOARD_STATUS_PADDING - 50,
						red);
					continue;  // Display an error message and go back to the main menu
				}
			}
			else if (response == OFFLINE_GAME)
			{
				// If the user chooses offline PvP game
				pvpOfflineGame();  // Start the offline PvP game
			}
		}
		else if (gameType == PVAI_GAME)
		{
			// If the user chooses Player vs. AI mode
			int response = difficultyMenu();  // Display and get the user's choice from the difficulty menu
			if (response == BACK)
			{
				// If the user chooses to go back to the main menu
				continue;  // Skip the rest of the loop and go back to the main menu
			}
			else
			{
				// If the user selects a difficulty level
				int difficulty = response;
				pvaiGame(difficulty);  // Start the Player vs. AI game with the chosen difficulty
			}
		}
		else if (gameType == EXIT)
		{
			// If the user chooses to exit the game
			clearSDL();  // Clear SDL resources
			exit(1);  // Exit the program
		}
		else
		{
			// If an invalid game type is selected
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "GAME TYPE", "Selected invalid option!", window);  // Display an error message
		}
	}
}


int initialiseSDL()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return ERROR;
	}
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	// Create window
	window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (window == NULL)
	{
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		return ERROR;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return ERROR;
	}

	// Initialize SDL_ttf
	if (TTF_Init() != 0)
	{
		printf("TTF_Init Error: %s\n", TTF_GetError());
		SDL_Quit();
		return ERROR;
	}

	// Initialize SDL_ttf
	if (initialiseFonts() == ERROR)
	{
		printf("TTF_Init Error: %s\n", TTF_GetError());
		SDL_Quit();
		return ERROR;
	}

	clearScreen();

	return SUCCESS;
}