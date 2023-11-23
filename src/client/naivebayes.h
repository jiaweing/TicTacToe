#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Definitions for naivebayes.c
// Constants to represent board positions
#define X 0
#define O 1
#define EMPTY 2
// Number of board positions (9 in Tic Tac Toe)
#define NUM_POSITIONS 9
// Number of classes (Positive if X wins, Negative if Draw or O wins)
#define NUM_CLASSES 2
// Define the maximum number of data rows
#define MAX_ROWS 958
#define MAX_TRAINING_ROWS 766
#define MAX_TESTING_ROWS 192
// Define the number of characters for 'positive\n' & 'negative\n'
#define MAX_CHARACTERS 9

void extract_data(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS]);
void shuffleArr (int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS]);
void split_data(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS], int training_data[MAX_TRAINING_ROWS][NUM_POSITIONS], int training_labels[MAX_TRAINING_ROWS],
                int testing_data[MAX_TESTING_ROWS][NUM_POSITIONS], int testing_labels[MAX_TESTING_ROWS], int split_ratio);
void calculatePriors(int labels[MAX_ROWS], double priors[NUM_CLASSES]);
void calculateLikelihoods(int data[][NUM_POSITIONS], int labels[], double likelihoods[NUM_POSITIONS][EMPTY+1][NUM_CLASSES], double priors[NUM_CLASSES]);
int countEmptyPositions(int int_board[NUM_POSITIONS], int empty_pos[NUM_POSITIONS]);
void changeToIntBoard(char char_board[NUM_POSITIONS], int int_board[NUM_POSITIONS]);
void naiveBayesLearn(int data[][NUM_POSITIONS], int labels[], double likelihoods[NUM_POSITIONS][EMPTY+1][NUM_CLASSES], double priors[NUM_CLASSES]);
int naiveBayesPredict(int int_board[NUM_POSITIONS], double priors[NUM_CLASSES], double likelihoods[NUM_POSITIONS][EMPTY+1][NUM_CLASSES], double *positive_possibility, double *negative_possibility);
void calcStats(int data_rows, int data[][NUM_POSITIONS], int labels[], double likelihoods[NUM_POSITIONS][EMPTY + 1][NUM_CLASSES], double priors[NUM_CLASSES], double *accuracy, double *errorcount, 
			   double *truepos, double *trueneg, double *falsepos, double *falseneg, double *positive, double *negative);
void validate(int training_data[MAX_TRAINING_ROWS][NUM_POSITIONS], int training_labels[MAX_TRAINING_ROWS], int testing_data[MAX_TESTING_ROWS][NUM_POSITIONS], 
			  int testing_labels[MAX_TESTING_ROWS], double likelihoods[NUM_POSITIONS][EMPTY+1][NUM_CLASSES], double priors[NUM_CLASSES]);