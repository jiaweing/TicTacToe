#include "ui.h"
#include "main_menu.h"
#include "game_logic.h"

int mainMenu()
{
    SDL_Event event;
    SDL_Rect buttonRects[3];

    drawMainMenu(buttonRects);
    SDL_RenderPresent(renderer);

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;

                if (y >= buttonRects[0].y && y < buttonRects[0].y + buttonRects[0].h && x >= buttonRects[0].x && x < buttonRects[0].x + buttonRects[0].w)
                {
                    if (x >= SCREEN_WIDTH / 2 - 50 && x < SCREEN_WIDTH / 2 + 50)
                    {
                        return PVP_GAME;
                    }
                }
                else if (y >= buttonRects[1].y && y < buttonRects[1].y + buttonRects[1].h && x >= buttonRects[1].x && x < buttonRects[1].x + buttonRects[1].w)
                {
                    if (x >= SCREEN_WIDTH / 2 - 50 && x < SCREEN_WIDTH / 2 + 50 * 2)
                    {
                        return PVAI_GAME;
                    }
                }
                else if (y >= buttonRects[2].y && y < buttonRects[2].y + buttonRects[2].h && x >= buttonRects[2].x && x < buttonRects[2].x + buttonRects[2].w)
                {
                    if (x >= SCREEN_WIDTH / 2 - 50 && x < SCREEN_WIDTH / 2 + 50 * 2)
                    {
                        return EXIT;
                    }
                }
            }
            else if (event.type == SDL_QUIT)
            {
				exit(0);
				break;
            }
        }
    }

    return ERROR;
}

int difficultyMenu()
{
    SDL_Event event;
    SDL_Rect buttonRects[5];

    drawDifficultyMenu(buttonRects);
    SDL_RenderPresent(renderer);

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;

                if (y >= buttonRects[0].y && y < buttonRects[0].y + buttonRects[0].h && x >= buttonRects[0].x && x < buttonRects[0].x + buttonRects[0].w)
                {
                    if (x >= SCREEN_WIDTH / 2 - 50 && x < SCREEN_WIDTH / 2 + 50)
                    {
                        return EASY_DIFFICULTY;
                    }
                }
                else if (y >= buttonRects[1].y && y < buttonRects[1].y + buttonRects[1].h && x >= buttonRects[1].x && x < buttonRects[1].x + buttonRects[1].w)
                {
                    if (x >= SCREEN_WIDTH / 2 - 50 && x < SCREEN_WIDTH / 2 + 50 * 2)
                    {
                        return MEDIUM_DIFFICULTY;
                    }
                }
                else if (y >= buttonRects[2].y && y < buttonRects[2].y + buttonRects[2].h && x >= buttonRects[2].x && x < buttonRects[2].x + buttonRects[2].w)
                {
                    if (x >= SCREEN_WIDTH / 2 - 50 && x < SCREEN_WIDTH / 2 + 50 * 2)
                    {
                        return HARD_DIFFICULTY;
                    }
                }
                else if (y >= buttonRects[3].y && y < buttonRects[3].y + buttonRects[3].h && x >= buttonRects[3].x && x < buttonRects[3].x + buttonRects[3].w)
                {
                    if (x >= SCREEN_WIDTH / 2 - 50 && x < SCREEN_WIDTH / 2 + 50 * 2)
                    {
                        return IMPOSSIBLE_DIFFICULTY;
                    }
                }
                else if (y >= buttonRects[4].y && y < buttonRects[4].y + buttonRects[4].h && x >= buttonRects[4].x && x < buttonRects[4].x + buttonRects[4].w)
                {
                    if (x >= SCREEN_WIDTH / 2 - 50 && x < SCREEN_WIDTH / 2 + 50 * 2)
                    {
                        return BACK;
                    }
                }
            }
            else if (event.type == SDL_QUIT)
            {
				exit(0);
				break;
            }
        }
    }

    return ERROR;
}

int drawMainMenu(SDL_Rect buttonRects[3])
{
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Create a font (you should replace this with your own font)
    TTF_Font *font = TTF_OpenFont(PCSENIOR_FONT, 39);
    TTF_Font *btnfont = TTF_OpenFont(PCSENIOR_FONT, 18);
    if (font == NULL)
    {
        fprintf(stderr, "TTF_OpenFont Errors: %s\n", TTF_GetError());
        return ERROR;
    }

    SDL_Color textColor = {255, 255, 255};
    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Rect textRect;

    // Draw the title
    textSurface = TTF_RenderText_Solid(font, "Tic Tac Toe", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect.x = SCREEN_WIDTH / 2 - textSurface->w / 2;
    textRect.y = 40;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Draw the menu options (PVP, PVAI, Exit)
    char *menuOptions[] = {"Player v Player", "Player v AI", "Exit"};
    SDL_Color buttonColors[] = {{0, 100, 200}, {0, 200, 100}, {42, 52, 146}};
    // SDL_Rect buttonRects[4];
    for (int i = 0; i < 3; i++)
    {
        // Define button colors and positions
        SDL_SetRenderDrawColor(renderer, buttonColors[i].r, buttonColors[i].g, buttonColors[i].b, 255);
        buttonRects[i].x = SCREEN_WIDTH / 2 - 150; //-150 because the button width is 300
        buttonRects[i].y = 115 + i * 95;
        buttonRects[i].w = 300;
        buttonRects[i].h = 80;
        // SDL_SetRenderDrawColor(_renderer, 255, 255, 255,255); // Border color (black)
        SDL_RenderFillRect(renderer, &buttonRects[i]);

        textSurface = TTF_RenderText_Solid(btnfont, menuOptions[i], textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        // Center the text within the buttonRect
        textRect.x = buttonRects[i].x + (buttonRects[i].w - textSurface->w) / 2;
        textRect.y = buttonRects[i].y + (buttonRects[i].h - textSurface->h) / 2;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }

    // Clean up resources
    TTF_CloseFont(font);
    TTF_CloseFont(btnfont);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    return SUCCESS;
}

int drawDifficultyMenu(SDL_Rect buttonRects[5])
{
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Create a font (you should replace this with your own font)
    TTF_Font *font = TTF_OpenFont(PCSENIOR_FONT, 39);
    TTF_Font *btnfont = TTF_OpenFont(PCSENIOR_FONT, 18);
    if (font == NULL)
    {
        fprintf(stderr, "TTF_OpenFont Errors: %s\n", TTF_GetError());
        return ERROR;
    }

    SDL_Color textColor = {255, 255, 255};
    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Rect textRect;

    // Draw the title
    textSurface = TTF_RenderText_Solid(font, "Tic Tac Toe", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect.x = SCREEN_WIDTH / 2 - textSurface->w / 2;
    textRect.y = 40;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Draw the menu options (PVP, PVAI, Exit)
    char *menuOptions[] = {"Easy", "Medium", "Hard", "Impossible", "Back"};
    SDL_Color buttonColors[] = {{79, 175, 68}, {181, 173, 16}, {255, 149, 38}, {239, 68, 35}, {42, 52, 146}};
    // SDL_Rect buttonRects[4];
    for (int i = 0; i < 5; i++)
    {
        // Define button colors and positions
        SDL_SetRenderDrawColor(renderer, buttonColors[i].r, buttonColors[i].g, buttonColors[i].b, 255);
        buttonRects[i].x = SCREEN_WIDTH / 2 - 150; //-150 because the button width is 300
        buttonRects[i].y = 115 + i * 95;
        buttonRects[i].w = 300;
        buttonRects[i].h = 80;
        // SDL_SetRenderDrawColor(_renderer, 255, 255, 255,255); // Border color (black)
        SDL_RenderFillRect(renderer, &buttonRects[i]);

        textSurface = TTF_RenderText_Solid(btnfont, menuOptions[i], textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        // Center the text within the buttonRect
        textRect.x = buttonRects[i].x + (buttonRects[i].w - textSurface->w) / 2;
        textRect.y = buttonRects[i].y + (buttonRects[i].h - textSurface->h) / 2;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }

    // Clean up resources
    TTF_CloseFont(font);
    TTF_CloseFont(btnfont);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    return SUCCESS;
}