# Tic Tac Toe Game ![Build Status](https://github.com/my3t/TicTacToe/workflows/Build/badge.svg)

This is a simple Tic Tac Toe game written in C using the SDL2 library.

## Builds



## Mac

We only have prebuilt binaries for Macs running on Apple Intel, as it is not possible to build the Apple Silicon binary on a Linux system. You have to compile yourself.

## Dependencies

**These are required to run the app. Dependencies for Windows are packaged together with the project.**

- SDL2
- SDL2_ttf
- GNU Make

## Compilation

### Windows

1. Install [Chocalatey](https://chocolatey.org/install)
2. Install GNU Make using Chocolatey
   - `choco install make`
3. Navigate to the project directory
4. Compile the game and the server
   - `make client -j4`
   - `make server -j4`
5. Run the game 
   - `./ttt_client.exe`
6. Run the server 
   - `./ttt_server.exe`

### Mac

1. Install [Homebrew](https://brew.sh/)
2. Install SDL2, SDL2_ttf, and make libraries
   - `brew install sdl2 sdl2_ttf make`
3. Navigate to the project directory
4. Compile the game and the server
   - `make client -j4`
   - `make server -j4`
5. Run the game 
   - `./ttt_client`
6. Run the server 
   - `./ttt_server`

### Linux

1. Install SDL2 and SDL2_ttf libraries: `sudo apt-get install libsdl2-dev libsdl2-ttf-dev make`
2. Navigate to the project directory
3. Compile the game and the server
   - `make client -j4`
   - `make server -j4`
4. Run the game 
   - `./ttt_client`
5. Run the server 
   - `./ttt_server`

## How to Play

1. The game starts with player X.
2. Click on an empty tile to place your mark.
3. The first player to get three marks in a row (horizontally, vertically, or diagonally) wins.
4. If all tiles are filled and no player has won, the game is a tie.

gcc -std=c17 ttt.c ui.c game_logic.c board.c main_menu.c naivebayes.c pvai_game.c pvp_online_game.c pvp_offline_game.c -I "C:\Users\Tay Jeung Hong\Documents\SIT\Y1S1\CSC 1103 - Programming Methodology\PMGrpAsg\TicTacToe(WORKING)\sdl2\include" -L "C:\Users\Tay Jeung Hong\Documents\SIT\Y1S1\CSC 1103 - Programming Methodology\PMGrpAsg\TicTacToe(WORKING)\SDL2\lib"  -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lwsock32 -o ttt_client

gcc -std=c17 src/client/ttt.c src/client/ui.c src/client/game_logic.c src/client/board.c src/client/main_menu.c src/client/naivebayes.c src/client/pvai_game.c src/client/pvp_online_game.c src/client/pvp_offline_game.c -I "C:\Users\Tay Jeung Hong\Documents\SIT\Y1S1\CSC 1103 - Programming Methodology\PMGrpAsg\TicTacToe(WORKING)\sdl2\include" -L "C:\Users\Tay Jeung Hong\Documents\SIT\Y1S1\CSC 1103 - Programming Methodology\PMGrpAsg\TicTacToe(WORKING)\SDL2\lib"  -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lwsock32 -o ttt_client

gcc -std=c17 src/client/ttt.c src/client/ui.c src/client/game_logic.c src/client/board.c src/client/main_menu.c src/client/naivebayes.c src/client/pvai_game.c src/client/pvp_online_game.c src/client/pvp_offline_game.c -I "C:\Users\Tay Jeung Hong\Desktop\TicTacToe - Copy\include\sdl2\include" -L "C:\Users\Tay Jeung Hong\Desktop\TicTacToe - Copy\include\SDL2\lib"  -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lwsock32 -o ttt_client

172.16.150.222
27428