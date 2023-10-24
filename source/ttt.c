// TODO: validation for input
// TODO: game type specific modules in seperate files
// TODO: spruce up GUI

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BOARD_SIZE 300
#define TILE_SIZE 100
#define LINE_WIDTH 10

#define EMPTY_SYMBOL 'b'
#define X_SYMBOL 'x'
#define O_SYMBOL 'o'

#define TWO_PLAYER_GAME 0
#define MINIMAX_GAME 1
#define AI_GAME 2

void drawBoard(SDL_Renderer *renderer, const char board[9]);
void drawEllipse(SDL_Renderer *renderer, int x, int y, int rx, int ry);
void clearScreen(SDL_Renderer *renderer);
void playerMove(char symbol, char board[9]);
void computerMove(char board[9]);
int minimax(char board[9], char player);
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

void drawBoard(SDL_Renderer *renderer, const char board[9])
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect board_rect = {SCREEN_WIDTH / 2 - BOARD_SIZE / 2, SCREEN_HEIGHT / 2 - BOARD_SIZE / 2, BOARD_SIZE, BOARD_SIZE};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &board_rect);

    for (int i = 0; i < 9; ++i)
    {
        int row = i % 3;
        int col = i / 3;
        SDL_Rect tileRect = {board_rect.x + row * TILE_SIZE, board_rect.y + col * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &tileRect);

        if (board[i] == X_SYMBOL)
        {
            SDL_RenderDrawLine(renderer, tileRect.x + LINE_WIDTH, tileRect.y + LINE_WIDTH, tileRect.x + TILE_SIZE - LINE_WIDTH, tileRect.y + TILE_SIZE - LINE_WIDTH);
            SDL_RenderDrawLine(renderer, tileRect.x + TILE_SIZE - LINE_WIDTH, tileRect.y + LINE_WIDTH, tileRect.x + LINE_WIDTH, tileRect.y + TILE_SIZE - LINE_WIDTH);
        }
        else if (board[i] == O_SYMBOL)
        {
            drawEllipse(renderer, tileRect.x + TILE_SIZE / 2, tileRect.y + TILE_SIZE / 2, TILE_SIZE / 2 - LINE_WIDTH, TILE_SIZE / 2 - LINE_WIDTH);
        }
    }

    SDL_RenderPresent(renderer);
}

void drawEllipse(SDL_Renderer *renderer, int x, int y, int rx, int ry)
{
    double angle, x1, y1, x2, y2;

    for (int i = 0; i < 360; i++)
    {
        angle = i * M_PI / 180;
        x1 = rx * cos(angle);
        y1 = ry * sin(angle);
        x2 = x1 * cos(angle) - y1 * sin(angle) + x;
        y2 = x1 * sin(angle) + y1 * cos(angle) + y;
        SDL_RenderDrawPoint(renderer, x2, y2);
    }
}

void clearScreen(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void playerMove(char symbol, char board[9])
{
    SDL_Event event;
    int done = 0;

    // Poll for event
    while (!done)
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
                            board[pos] = symbol;
                            done = 1;
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
}

void computerMove(char board[9])
{
    int move = -1;
    int score = -2;
    int i;
    for (i = 0; i < 9; ++i)
    {
        if (board[i] == EMPTY_SYMBOL)
        {
            board[i] = X_SYMBOL;
            int tempScore = -minimax(board, O_SYMBOL);
            board[i] = EMPTY_SYMBOL;
            if (tempScore > score)
            {
                score = tempScore;
                move = i;
            }
        }
    }

    // returns a score based on minimax tree at a given node.
    board[move] = X_SYMBOL;
}

// something's not working here
int minimax(char board[9], char player)
{
    // How is the position like for player (their turn) on board?
    char winner = win(board);
    if (winner != EMPTY_SYMBOL)
        return winner == X_SYMBOL ? 1 : -1;

    int move = -1;
    int score = -2; // Losing moves are preferred to no move
    for (int i = 0; i < 9; ++i)
    { // For all moves
        if (board[i] == EMPTY_SYMBOL)
        {                      // If legal
            board[i] = player; // Try the move
            int thisScore = -minimax(board, player == X_SYMBOL ? O_SYMBOL : X_SYMBOL);
            if (thisScore > score)
            {
                score = thisScore;
                move = i;
            }                        // Pick the one that's worst for the opponent
            board[i] = EMPTY_SYMBOL; // Reset board after try
        }
    }

    if (move == -1)
        return 0;
    return score;
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