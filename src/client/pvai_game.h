#include <stdlib.h>

#include "naivebayes.h"

int pvaiGame(int difficulty);
int computerMove(char symbol, char board[9], int difficulty, double priors[NUM_CLASSES], double likelihoods[NUM_POSITIONS][EMPTY+1][NUM_CLASSES], int turn);
void drawAIGameScreen(char board[9], char player);