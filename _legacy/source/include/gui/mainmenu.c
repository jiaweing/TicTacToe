void drawMenu(SDL_Rect buttonRects[5])
{
    // Clear the screen
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);

    // Create a font (you should replace this with your own font)
    TTF_Font *font = TTF_OpenFont("fonts/pcsenior.ttf", 39);
    TTF_Font *btnfont = TTF_OpenFont("fonts/pcsenior.ttf", 25);
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
    textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);
    textRect.x = SCREEN_WIDTH / 2 - textSurface->w / 2;
    textRect.y = 40;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(_renderer, textTexture, NULL, &textRect);

    // Draw the menu options (PVP, PVAI, Exit)
    char *menuOptions[] = {"PvP", "Perfect AI", "PvAI","PvNBAI","Exit"};
    SDL_Color buttonColors[] = {{0, 100, 200}, {0, 200, 100}, {200, 100, 0}, {200, 200, 0},{0, 100, 200}};
    // SDL_Rect buttonRects[4];
    for (int i = 0; i < 5; i++)
    {
        // Define button colors and positions
        SDL_SetRenderDrawColor(_renderer, buttonColors[i].r, buttonColors[i].g, buttonColors[i].b, 255);
        buttonRects[i].x = SCREEN_WIDTH / 2-150; //-150 because the button width is 300
        buttonRects[i].y = 115 + i * 95;
        buttonRects[i].w = 300;
        buttonRects[i].h = 80;
        // SDL_SetRenderDrawColor(_renderer, 255, 255, 255,255); // Border color (black)
        SDL_RenderFillRect(_renderer, &buttonRects[i]);

        textSurface = TTF_RenderText_Solid(btnfont, menuOptions[i], textColor);
        textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);

        // Center the text within the buttonRect
        textRect.x = buttonRects[i].x + (buttonRects[i].w - textSurface->w) / 2;
        textRect.y = buttonRects[i].y + (buttonRects[i].h - textSurface->h) / 2;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;

        SDL_RenderCopy(_renderer, textTexture, NULL, &textRect);
    }

    // Clean up resources
    TTF_CloseFont(font);
    TTF_CloseFont(btnfont);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}