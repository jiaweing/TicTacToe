void drawMenu(SDL_Renderer *renderer,SDL_Rect buttonRects[4])
{
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Create a font (you should replace this with your own font)
    TTF_Font *font = TTF_OpenFont("fonts/pcsenior.ttf", 42);
    TTF_Font *btnfont = TTF_OpenFont("fonts/pcsenior.ttf", 36);
    if (font == NULL)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Color textColor = {255, 255, 255};
    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Rect textRect;

    // Draw the title
    textSurface = TTF_RenderText_Solid(font, "Tic Tac Toe", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect.x = SCREEN_WIDTH / 2 - textSurface->w / 2;
    textRect.y = 80;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Draw the menu options (PVP, PVAI, Exit)
    char *menuOptions[] = {"PvP", "Perfect AI", "PvAI", "Exit"};
    SDL_Color buttonColors[] = {{0, 100, 200}, {0, 200, 100}, {200, 100, 0}, {200, 200, 0}};
    // SDL_Rect buttonRects[4];
    for (int i = 0; i < 4; i++)
    {
        // Define button colors and positions
        SDL_SetRenderDrawColor(renderer, buttonColors[i].r, buttonColors[i].g, buttonColors[i].b, 255);
        buttonRects[i].x = SCREEN_WIDTH / 2 - 200;
        buttonRects[i].y = 160 + i * 130;
        buttonRects[i].w = 400;
        buttonRects[i].h = 100;
        // SDL_SetRenderDrawColor(renderer, 255, 255, 255,255); // Border color (black)
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
}