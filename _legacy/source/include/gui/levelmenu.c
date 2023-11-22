// #define BUTTON_WIDTH 100  // Replace with your button width
// #define BUTTON_HEIGHT 50  // Replace with your button height

// void levelMenu(SDL_Renderer *_renderer){
//     SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);  // Set background color (white)
//     SDL_RenderClear(_renderer);
//     SDL_RenderPresent(_renderer);
//     int buttonClicked = -1;
//     SDL_Rect levelbtnRects[3] = {
//         {SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 3 - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT},
//         {SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT},
//         {SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT * 2 / 3 - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT}
//     };

//     TTF_Font *font = TTF_OpenFont("fonts/pcsenior.ttf", 39);
//     TTF_Font *btnFont = TTF_OpenFont("fonts/pcsenior.ttf", 25);
//     TTF_Font *timerFont = TTF_OpenFont("fonts/pcsenior.ttf",9);
//     if (font == NULL || btnFont == NULL) {
//         fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
//         TTF_Quit();
//         SDL_Quit();
//         return;
//     }

//     // Render the buttons
//     SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);  // Set color to black
//     for (int i = 0; i < 3; i++) {
//         SDL_RenderFillRect(_renderer, &levelbtnRects[i]);
//     }

//     // Wait for a button click
//     SDL_Event event;
//     while (buttonClicked == -1) {
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_MOUSEBUTTONDOWN) {
//                 int x = event.button.x;
//                 int y = event.button.y;
//                 for (int i = 0; i < 3; i++) {
//                     if (x >= levelbtnRects[i].x && x <= levelbtnRects[i].x + levelbtnRects[i].w &&
//                         y >= levelbtnRects[i].y && y <= levelbtnRects[i].y + levelbtnRects[i].h) {
//                         buttonClicked = i;
//                         break;
//                     }
//                 }
//             }
//         }
//     }
//     SDL_RenderPresent(_renderer);
// }










// void levelMenu(SDL_Renderer *_renderer) {
//     SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);  // Set background color (white)
//     SDL_RenderClear(_renderer);
//     int whiteScreen = 1;
//     int timeout = 10000;  // Timeout in milliseconds (10 seconds)
//     int startTime = SDL_GetTicks();  // start timer
//     int buttonClicked = -1;
//     SDL_Rect levelbtnRects[3];

//     TTF_Font *font = TTF_OpenFont("fonts/pcsenior.ttf", 39);
//     TTF_Font *btnFont = TTF_OpenFont("fonts/pcsenior.ttf", 25);
//     TTF_Font *timerFont = TTF_OpenFont("fonts/pcsenior.ttf",9);
//     if (font == NULL || btnFont == NULL) {
//         fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
//         TTF_Quit();
//         SDL_Quit();
//         return;
//     }
    
//     while (whiteScreen) {
        
//         SDL_Color leveltextColor = {0, 0, 0, 255};
//         SDL_Surface *leveltextSurface;
//         SDL_Texture *leveltextTexture;
//         SDL_Rect leveltextRect;
//         SDL_Event event;
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_QUIT) {
//                 whiteScreen = 0;
//             } else if (event.type == SDL_MOUSEBUTTONDOWN) {
//                 int x = event.button.x;
//                 int y = event.button.y;

//                 // Check for button click events
//                 for (int i = 0; i < 3; i++) {
//                     if (x >= levelbtnRects[i].x && x <= levelbtnRects[i].x + levelbtnRects[i].w &&
//                         y >= levelbtnRects[i].y && y <= levelbtnRects[i].y + levelbtnRects[i].h) {
//                         buttonClicked = i;  // Record which button was clicked
//                         printf("%d", i);
//                         break;
//                     }
//                 }
//             }
//         }

//         if (buttonClicked != -1) {
//             if (buttonClicked == 0) {
//                 printf("Easy button clicked!\n");
//             } else if (buttonClicked == 1) {
//                 printf("Medium button clicked!\n");
//             } else if (buttonClicked == 2) {
//                 printf("Hard button clicked!\n");
//             }
//             // Reset the buttonClicked value to prevent re-triggering the same button action
//             buttonClicked = -1;
//         }
//         SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);  // Set background color (white)
//         SDL_RenderClear(_renderer);

//         // Render "Select Difficulty" title
//         // SDL_Rect levelbtnRects[3];
//         // SDL_Color leveltextColor = {0, 0, 0, 255};
//         // SDL_Surface *leveltextSurface;
//         // SDL_Texture *leveltextTexture;
//         // SDL_Rect leveltextRect;

//         leveltextSurface = TTF_RenderText_Solid(font, "Select Difficulty", leveltextColor);
//         leveltextTexture = SDL_CreateTextureFromSurface(_renderer, leveltextSurface);
//         leveltextRect.x = SCREEN_WIDTH / 2 - leveltextSurface->w / 2;
//         leveltextRect.y = 100;
//         leveltextRect.w = leveltextSurface->w;
//         leveltextRect.h = leveltextSurface->h;
//         SDL_RenderCopy(_renderer, leveltextTexture, NULL, &leveltextRect);

//         char *levelmenuOptions[] = {"Easy", "Medium", "Hard"};
//         SDL_Color buttonColors[] = {{0, 200, 0}, {200, 200, 0}, {200, 0, 0}};

//         int startY = 200; // Starting Y position for buttons
//         int buttonSpacing = 95; // Vertical spacing between buttons

//         for (int i = 0; i < 3; i++) {
//             SDL_SetRenderDrawColor(_renderer, buttonColors[i].r, buttonColors[i].g, buttonColors[i].b, 255);
//             levelbtnRects[i].x = SCREEN_WIDTH / 2 - 150; // Center the text within the buttonRect
//             levelbtnRects[i].y = startY + i * buttonSpacing;
//             levelbtnRects[i].w = 300;
//             levelbtnRects[i].h = 80;

//             SDL_RenderFillRect(_renderer, &levelbtnRects[i]); // Render button background

//             leveltextSurface = TTF_RenderText_Solid(btnFont, levelmenuOptions[i], leveltextColor);
//             leveltextTexture = SDL_CreateTextureFromSurface(_renderer, leveltextSurface);

//             // Center the text within the buttonRect
//             leveltextRect.x = levelbtnRects[i].x + (levelbtnRects[i].w - leveltextSurface->w) / 2;
//             leveltextRect.y = levelbtnRects[i].y + (levelbtnRects[i].h - leveltextSurface->h) / 2;
//             leveltextRect.w = leveltextSurface->w;
//             leveltextRect.h = leveltextSurface->h;

//             SDL_RenderCopy(_renderer, leveltextTexture, NULL, &leveltextRect); // Render button text
//             SDL_FreeSurface(leveltextSurface);
//             SDL_DestroyTexture(leveltextTexture);
//         }

//         // portion to show the timer for the screen
//         int timeRemaining = (timeout - (SDL_GetTicks() - startTime)) / 1000; // Convert milliseconds to seconds
//         char timeText[20];
//         snprintf(timeText, sizeof(timeText), "Timing out in ... %d sec", timeRemaining);

//         SDL_Surface *timeSurface = TTF_RenderText_Solid(timerFont, timeText, leveltextColor);
//         SDL_Texture *timeTexture = SDL_CreateTextureFromSurface(_renderer, timeSurface);

//         SDL_Rect timeRect;
//         timeRect.x = 20; // X position of the countdown label
//         timeRect.y = SCREEN_HEIGHT-20; // Y position of the countdown label
//         timeRect.w = timeSurface->w;
//         timeRect.h = timeSurface->h;

//         SDL_RenderCopy(_renderer, timeTexture, NULL, &timeRect);
//         SDL_FreeSurface(timeSurface);
//         SDL_DestroyTexture(timeTexture);

//         SDL_RenderPresent(_renderer);

//         // SDL_Event event;
//         // while (SDL_PollEvent(&event)) {
//         //     if (event.type == SDL_QUIT) {
//         //         whiteScreen = 0;  // Exit the white screen loop if the window is closed
//         //     }
//         //     // Handle button click events here
//         // }
//         // if (SDL_GetTicks() - startTime > timeout) {
//         //     whiteScreen =0; // Exit loop if the timeout is reached
//         // }
//     }
// }
//DO NOT RUN THISCODE WITHOUT TIMEOUT IT WILL HANG

void levelMenu(SDL_Renderer *_renderer)
{
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255); // Set background color (white)
    SDL_RenderClear(_renderer);
    int whiteScreen = 1;
    int timeout = 10000;            // Timeout in milliseconds (10 seconds)
    int startTime = SDL_GetTicks(); // start timer
    int buttonClicked = -1;
    SDL_Rect levelbtnRects[3];

    TTF_Font *font = TTF_OpenFont("fonts/pcsenior.ttf", 39);
    TTF_Font *btnFont = TTF_OpenFont("fonts/pcsenior.ttf", 25);
    TTF_Font *timerFont = TTF_OpenFont("fonts/pcsenior.ttf", 9);
    if (font == NULL || btnFont == NULL)
    {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return;
    }

    while (whiteScreen)
    {

        SDL_Color leveltextColor = {0, 0, 0, 255};
        SDL_Surface *leveltextSurface;
        SDL_Texture *leveltextTexture;
        SDL_Rect leveltextRect;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                whiteScreen = 0;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;

                // Check for button click events
                for (int i = 0; i < 3; i++)
                {
                    if (x >= levelbtnRects[i].x && x <= levelbtnRects[i].x + levelbtnRects[i].w &&
                        y >= levelbtnRects[i].y && y <= levelbtnRects[i].y + levelbtnRects[i].h)
                    {
                        buttonClicked = i; // Record which button was clicked
                        printf("%d", i);
                        break;
                    }
                }
            }
        }

        if (buttonClicked != -1)
        {
            if (buttonClicked == 0)
            {
                printf("Easy button clicked!\n");
            }
            else if (buttonClicked == 1)
            {
                printf("Medium button clicked!\n");
            }
            else if (buttonClicked == 2)
            {
                printf("Hard button clicked!\n");
            }
            // Reset the buttonClicked value to prevent re-triggering the same button action
            buttonClicked = -1;
        }
        SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255); // Set background color (white)
        SDL_RenderClear(_renderer);

        // Render "Select Difficulty" title
        leveltextSurface = TTF_RenderText_Solid(font, "Select Difficulty", leveltextColor);
        leveltextTexture = SDL_CreateTextureFromSurface(_renderer, leveltextSurface);
        leveltextRect.x = SCREEN_WIDTH / 2 - leveltextSurface->w / 2;
        leveltextRect.y = 100;
        leveltextRect.w = leveltextSurface->w;
        leveltextRect.h = leveltextSurface->h;
        SDL_RenderCopy(_renderer, leveltextTexture, NULL, &leveltextRect);
        char *levelmenuOptions[] = {"Easy", "Medium", "Hard"};
        SDL_Color buttonColors[] = {{0, 200, 0}, {200, 200, 0}, {200, 0, 0}};

        int startY = 200;       // Starting Y position for buttons
        int buttonSpacing = 95; // Vertical spacing between buttons

        for (int i = 0; i < 3; i++)
        {
            SDL_SetRenderDrawColor(_renderer, buttonColors[i].r, buttonColors[i].g, buttonColors[i].b, 255);
            levelbtnRects[i].x = SCREEN_WIDTH / 2 - 150; // Center the text within the buttonRect
            levelbtnRects[i].y = startY + i * buttonSpacing;
            levelbtnRects[i].w = 300;
            levelbtnRects[i].h = 80;

            SDL_RenderFillRect(_renderer, &levelbtnRects[i]); // Render button background

            leveltextSurface = TTF_RenderText_Solid(btnFont, levelmenuOptions[i], leveltextColor);
            leveltextTexture = SDL_CreateTextureFromSurface(_renderer, leveltextSurface);
            // Center the text within the buttonRect
            leveltextRect.x = levelbtnRects[i].x + (levelbtnRects[i].w - leveltextSurface->w) / 2;
            leveltextRect.y = levelbtnRects[i].y + (levelbtnRects[i].h - leveltextSurface->h) / 2;
            leveltextRect.w = leveltextSurface->w;
            leveltextRect.h = leveltextSurface->h;

            SDL_RenderCopy(_renderer, leveltextTexture, NULL, &leveltextRect); // Render button text
            SDL_FreeSurface(leveltextSurface);
            SDL_DestroyTexture(leveltextTexture);
        }
        SDL_RenderPresent(_renderer);
    }
}