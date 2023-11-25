# Tic Tac Toe Game ![Build Status](https://github.com/my3t/TicTacToe/workflows/Build/badge.svg)

This is a simple Tic Tac Toe game written in C using the SDL2 library.

## Features

- Online/Offline Multiplayer Mode
- Versus AI

## Builds

## Mac

We only have prebuilt binaries for Macs running on Apple Intel, as it is not possible to build the Apple Silicon binary on a Linux system. You have to compile yourself.

## Dependencies

- SDL2
- SDL2_ttf
- SDL2_image
- GNU make

### Windows

1. Install [MYS2](https://www.msys2.org)
2. Install [Chocalatey](https://chocolatey.org/install)
3. Install GNU make using Chocolatey
   - `choco install make`

### Mac

1. Install [Homebrew](https://brew.sh/)
2. Install dependencies
   - `brew install sdl2 sdl2_ttf make`

### Linux

1. Install dependencies
   - `sudo apt-get install libsdl2-dev libsdl2-ttf-dev make`

## Compilation

1. Navigate to the project directory
2. Compile the game and the server
   - `make client -j4`
   - `make server -j4`
3. Run the game
   - `./ttt_client`
4. Run the server
   - `./ttt_server [port number (optional)]`
