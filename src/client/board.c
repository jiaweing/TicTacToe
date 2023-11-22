#include "ui.h"
#include "board.h"

int playerMove(char symbol, char board[9])
{
	SDL_Event event;

	// Poll for event
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					if (x >= SCREEN_WIDTH / 2 - BOARD_SIZE / 2 && x < SCREEN_WIDTH / 2 + BOARD_SIZE / 2 &&
						y >= SCREEN_HEIGHT / 2 - BOARD_SIZE / 2 && y < SCREEN_HEIGHT / 2 + BOARD_SIZE / 2)
					{
						int row = (y - (SCREEN_HEIGHT / 2 - BOARD_SIZE / 2)) / TILE_SIZE;
						int col = (x - (SCREEN_WIDTH / 2 - BOARD_SIZE / 2)) / TILE_SIZE;
						int pos = (row * 3) + col;
						if (board[pos] == EMPTY_SYMBOL)
						{
							return pos;
						}
					}
				}
				break;
			case SDL_QUIT:
				exit(0);
				break;
			}
		}
	}

	return -1;
}

void drawBoard(const char board[9])
{
	if (setBackgroundImage("assets/images/backdropfinal.jpeg") == ERROR)
    {
        printf("Image not loading");
    }

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_Rect board_rect = {SCREEN_WIDTH / 2 - BOARD_SIZE / 2, SCREEN_HEIGHT / 2 - BOARD_SIZE / 2, BOARD_SIZE, BOARD_SIZE};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &board_rect);

	for (int i = 0; i < 9; ++i)
	{
		int row = i % 3;
		int col = i / 3;
		SDL_Rect tileRect = {board_rect.x + row * TILE_SIZE, board_rect.y + col * TILE_SIZE, TILE_SIZE, TILE_SIZE};
		TTF_Font *font = TTF_OpenFont(PCSENIOR_FONT, 35);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &tileRect);

		if (board[i] == X_SYMBOL)
		{
			// Set the color for X
			SDL_Color color = {255, 0, 0, 255}; // Red color
			renderAnchoredText("X", font, tileRect.x + TILE_SIZE / 2, tileRect.y + TILE_SIZE / 2, color);
		}
		else if (board[i] == O_SYMBOL)
		{
			// Set the color for O
			SDL_Color color = {0, 155, 255, 255}; // Blue color
			renderAnchoredText("O", font, tileRect.x + TILE_SIZE / 2, tileRect.y + TILE_SIZE / 2, color);
		}
	}
}

void drawCurrentPlayer(char currentPlayer)
{
	SDL_Rect textRect = {10, 10, 0, 0};
	SDL_Color textColor = {255, 255, 255, 255};

	char text[20];
	sprintf(text, "Current Player: %c", toupper(currentPlayer));

	TTF_Font *font = TTF_OpenFont(PCSENIOR_FONT, 24);
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	textRect.w = textSurface->w;
	textRect.h = textSurface->h;

	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	SDL_RenderPresent(renderer);
}

char win(const char board[9])
{
	// determines if a player has won, returns 0 otherwise.
	unsigned int wins[8][3] = {{2, 4, 6}, {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}}; // win states
	for (int i = 0; i < 8; ++i)
	{
		if (board[wins[i][0]] != EMPTY_SYMBOL &&
			board[wins[i][0]] == board[wins[i][1]] &&
			board[wins[i][0]] == board[wins[i][2]])
			return board[wins[i][0]];
	}
	return EMPTY_SYMBOL;
}