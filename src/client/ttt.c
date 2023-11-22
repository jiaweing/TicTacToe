// TODO: validation for input
// TODO: spruce up GUI
// TODO: choose who go first, random first position, choose colour of icon, undo button, responsive, timer, preview move with half opacity w/ blinks or animation, multiplayer, audio, quit button, score keeping, win animation, loading move, scoring

#include "ui.h"
#include "main_menu.h"
#include "pvp_offline_game.h"
#include "pvp_online_game.h"
#include "pvai_game.h"

int initialiseSDL();

int main(int argc, char *argv[])
{
	if (initialiseSDL() == ERROR)
	{
		printf("There has been an error with initialising SDL.\n");
		exit(0);
	}

	while (1) 
	{
		int gameType = mainMenu();
		if (gameType == PVP_GAME) 
		{
			char ip[15] = SERVER_HOSTNAME;
			int port = SERVER_PORT;
			if (askForHostIP(ip) == BACK)
			{
				continue;
			}
			if (askForHostPort(&port) == BACK)
			{
				continue;
			}
			if (pvpOnlineGame(ip, port) == ERROR)
			{
				renderText(
					"Couldn't connect to server", 
					pcsenior24_f, 
					BOARD_STATUS_PADDING, 
					SCREEN_HEIGHT - BOARD_STATUS_PADDING - 48, 
					white
				);
			}
		}
		else if (gameType == PVAI_GAME)
		{
			int response = difficultyMenu();
			if (response == BACK)
			{
				continue;
			}
			else
			{
				int difficulty = response;
				pvaiGame(difficulty);
			}
		}
		else if (gameType == EXIT)
		{
			TTF_Quit();
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
			exit(1);

		}
		else 
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "GAME TYPE", "Selected invalid option!", window);
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

	return SUCCESS;
}