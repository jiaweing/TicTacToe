#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BOARD_SIZE 300
#define TILE_SIZE 100
#define LINE_WIDTH 10

char board[3][3];

void draw_ellipse(SDL_Renderer *renderer, int x, int y, int rx, int ry);

int check_win(char player)
{
    int i, j;
    // Check rows
    for (i = 0; i < 3; i++)
    {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
        {
            return 1;
        }
    }
    // Check columns
    for (j = 0; j < 3; j++)
    {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player)
        {
            return 1;
        }
    }
    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
    {
        return 1;
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
    {
        return 1;
    }
    return 0;
}

void get_move(char player, int *row, int *col, SDL_Window *window)
{
    SDL_Event event;
    int done = 0;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    while (!done)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    if (x >= SCREEN_WIDTH / 2 - BOARD_SIZE / 2 && x < SCREEN_WIDTH / 2 + BOARD_SIZE / 2 &&
                        y >= SCREEN_HEIGHT / 2 - BOARD_SIZE / 2 && y < SCREEN_HEIGHT / 2 + BOARD_SIZE / 2)
                    {
                        *row = (y - (SCREEN_HEIGHT / 2 - BOARD_SIZE / 2)) / TILE_SIZE;
                        *col = (x - (SCREEN_WIDTH / 2 - BOARD_SIZE / 2)) / TILE_SIZE;
                        if (board[*row][*col] == ' ')
                        {
                            done = 1;
                        }
                    }
                }
                break;
            }
        }
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        // Draw game board
        SDL_Rect board_rect = {SCREEN_WIDTH / 2 - BOARD_SIZE / 2, SCREEN_HEIGHT / 2 - BOARD_SIZE / 2, BOARD_SIZE, BOARD_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &board_rect);
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                SDL_Rect tile_rect = {board_rect.x + j * TILE_SIZE, board_rect.y + i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &tile_rect);
                if (board[i][j] == 'X')
                {
                    SDL_RenderDrawLine(renderer, tile_rect.x + LINE_WIDTH, tile_rect.y + LINE_WIDTH, tile_rect.x + TILE_SIZE - LINE_WIDTH, tile_rect.y + TILE_SIZE - LINE_WIDTH);
                    SDL_RenderDrawLine(renderer, tile_rect.x + TILE_SIZE - LINE_WIDTH, tile_rect.y + LINE_WIDTH, tile_rect.x + LINE_WIDTH, tile_rect.y + TILE_SIZE - LINE_WIDTH);
                }
                else if (board[i][j] == 'O')
                {
                    draw_ellipse(renderer, tile_rect.x + TILE_SIZE / 2, tile_rect.y + TILE_SIZE / 2, TILE_SIZE / 2 - LINE_WIDTH, TILE_SIZE / 2 - LINE_WIDTH);
                }
                if (i == *row && j == *col)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &tile_rect);
                }
            }
        }
        // Update screen
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
}

void update_board(char player, int row, int col)
{
    board[row][col] = player;
}

char switch_player(char player)
{
    if (player == 'X')
    {
        return 'O';
    }
    else
    {
        return 'X';
    }
}

void draw_ellipse(SDL_Renderer *renderer, int x, int y, int rx, int ry)
{
    int i;
    double angle, x1, y1, x2, y2;
    for (i = 0; i < 360; i++)
    {
        angle = i * M_PI / 180;
        x1 = rx * cos(angle);
        y1 = ry * sin(angle);
        x2 = x1 * cos(angle) - y1 * sin(angle) + x;
        y2 = x1 * sin(angle) + y1 * cos(angle) + y;
        SDL_RenderDrawPoint(renderer, x2, y2);
    }
}

void reset_game(char *player, int *turn)
{
    int i, j;
    // Reset game board
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            board[i][j] = ' ';
        }
    }
    // Reset player and turn counter
    *player = 'X';
    *turn = 0;
}

int main(int argc, char *argv[])
{
    int i, j, row, col, turn;
    char player = 'X';
    // Initialize game board
    reset_game(&player, &turn);
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
    // Game loop
    for (turn = 0; turn < 9; turn++)
    {
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        // Draw game board
        SDL_Rect board_rect = {SCREEN_WIDTH / 2 - BOARD_SIZE / 2, SCREEN_HEIGHT / 2 - BOARD_SIZE / 2, BOARD_SIZE, BOARD_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &board_rect);
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                SDL_Rect tile_rect = {board_rect.x + j * TILE_SIZE, board_rect.y + i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &tile_rect);
                if (board[i][j] == 'X')
                {
                    SDL_RenderDrawLine(renderer, tile_rect.x + LINE_WIDTH, tile_rect.y + LINE_WIDTH, tile_rect.x + TILE_SIZE - LINE_WIDTH, tile_rect.y + TILE_SIZE - LINE_WIDTH);
                    SDL_RenderDrawLine(renderer, tile_rect.x + TILE_SIZE - LINE_WIDTH, tile_rect.y + LINE_WIDTH, tile_rect.x + LINE_WIDTH, tile_rect.y + TILE_SIZE - LINE_WIDTH);
                }
                else if (board[i][j] == 'O')
                {
                    draw_ellipse(renderer, tile_rect.x + TILE_SIZE / 2, tile_rect.y + TILE_SIZE / 2, TILE_SIZE / 2 - LINE_WIDTH, TILE_SIZE / 2 - LINE_WIDTH);
                }
            }
        }
        // Update screen
        SDL_RenderPresent(renderer);
        // Get player's move
        get_move(player, &row, &col, window);
        // Update game board
        update_board(player, row, col);
        // Check for win
        if (check_win(player))
        {
            char message[50];
            sprintf(message, "Player %c wins!", player);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", message, window);
            reset_game(&player, &turn);
            continue;
        }
        // Check for tie
        if (turn == 8)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Tie game!", window);
            reset_game(&player, &turn);
            continue;
        }
        // Switch player
        player = switch_player(player);
    }
    // Cleanup SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}