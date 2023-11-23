#include "naivebayes.h"

#define MAX_SCORE 1000
#define MIN_SCORE -1000

#define EASY_DIFFICULTY 1
#define MEDIUM_DIFFICULTY 2
#define HARD_DIFFICULTY 3
#define IMPOSSIBLE_DIFFICULTY 4

int predictNextMove(int difficulty_level, char char_board[NUM_POSITIONS], double priors[NUM_CLASSES], double likelihoods[NUM_POSITIONS][EMPTY + 1][NUM_CLASSES]);
int minimax(char board[9], char player, int depth, int alpha, int beta);
int randminimax(char board[9], char player, int depth, int alpha, int beta);