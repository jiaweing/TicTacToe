# Tic Tac Toe Game

This is a simple Tic Tac Toe game written in C using the SDL2 library.

## Dependencies

- SDL2
- SDL2_ttf

## Compilation

### Windows

1. Install [MSYS2](https://www.msys2.org/)
2. Open MSYS2 terminal
3. Navigate to the project directory
4. Compile the game: `gcc -std=c17 ttt.c -I<project path>\SDL2\include -L<project path>\SDL2\lib -Wall -lmingw32 -lSDL2main -lSDL2 -o ttt`
5. Run the game: `./ttt.exe`

### Linux

1. Install SDL2 and SDL2_ttf libraries: `sudo apt-get install libsdl2-dev libsdl2-ttf-dev`
2. Navigate to the project directory
3. Compile the game: `gcc -o ttt ttt.c -lSDL2`
4. Run the game: `./ttt`

### Mac

1. Install [Homebrew](https://brew.sh/)
2. Install SDL2 and SDL2_ttf libraries: `brew install sdl2`
3. Navigate to the project directory
4. Compile the game: `gcc -o ttt ttt.c -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -rpath /Library/Frameworks`
5. Run the game: `./ttt`

## How to Play

1. The game starts with player X.
2. Click on an empty tile to place your mark.
3. The first player to get three marks in a row (horizontally, vertically, or diagonally) wins.
4. If all tiles are filled and no player has won, the game is a tie.

## To Do

- [ ] Versus AI
- [ ] Mode Selection Menu
- [ ] Display Current Player
- [ ] Scoring System
