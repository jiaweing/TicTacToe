// TODO: validation for input
// TODO: game type specific modules in seperate files
// TODO: spruce up GUI

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "include/variables/global.c"

#include "include/functions/win.c"
#include "include/ai/minimax.c"
#include "include/functions/logic.c"

#include "include/gui/board.c"
#include "include/gui/mainmenu.c"

void drawBoard(SDL_Renderer *renderer, const char board[9]);
void drawEllipse(SDL_Renderer *renderer, int x, int y, int rx, int ry);
void clearScreen(SDL_Renderer *renderer);
void playerMove(char symbol, char board[9]);
void computerMove(char board[9]);
int minimax(char board[9], char player, int depth, int alpha, int beta);
char win(const char board[9]);

int main(int argc, char *argv[])
{
    char board[9] = {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}; // computer squares are x, player squares are o, empty squares are b
    int gameType = MINIMAX_GAME;
    int game = 1;
    int turn = 0;
    char player = X_SYMBOL;

    // TODO: make into window intialisation into function
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    // Create window
    SDL_Window *window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL)
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    printf("Input game type:\n-----\n0 - Player vs Player\n1 - Minimax AI Game\n2 - Dataset AI Game \n");
    scanf("%i", &gameType);

    // TODO: if can afford, make this more modular (split into functions)
    // DONT SPLIT INTO FUNCTIONS UNTIL DESIGN IS FINALISED
    while (game)
    {
        drawBoard(renderer, board); // draw board for the first time

        if (gameType == TWO_PLAYER_GAME)
        { // check game type
            for (turn = 0; turn < 9 && win(board) == EMPTY_SYMBOL; ++turn)
            { // loop until turns finish or there is no win
                if (player == X_SYMBOL)
                {                                // check current player type
                    playerMove(X_SYMBOL, board); // do move
                    player = O_SYMBOL;           // switch to other player
                }
                else
                {
                    playerMove(O_SYMBOL, board);
                    player = X_SYMBOL;
                }

                drawBoard(renderer, board); // redraw board after changes
            }

            char winner = win(board); // check winner once got winner or out of turns

            if (winner == X_SYMBOL)
            {                                                                                                             // check winner type
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Player 1 (X) is the winner!", window); // alert user on winner
            }
            else if (winner == O_SYMBOL)
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Player 2 (O) is the winner!", window);
            }
            else
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The game ended in a draw...", window);
            }
        }
        else if (gameType == MINIMAX_GAME)
        {
            for (turn = 0; turn < 9 && win(board) == EMPTY_SYMBOL; ++turn)
            {
                // printf("new turn\n");
                if (player == X_SYMBOL)
                {
                    computerMove(board);
                    player = O_SYMBOL;
                }
                else
                {
                    playerMove(O_SYMBOL, board);
                    player = X_SYMBOL;
                }

                drawBoard(renderer, board);
            }

            char winner = win(board);

            if (winner == X_SYMBOL)
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The computer (X) is the winner!", window);
            }
            else if (winner == O_SYMBOL)
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You (O) are the winner!", window);
            }
            else
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The game ended in a draw...", window);
            }
        }
        else
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "AI Mode", "Mode is still under construction!", window);
            game = 0;
        }

        // reset game
        player = X_SYMBOL;
        for (int i = 0; i < 9; ++i)
        {
            board[i] = 'b';
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}