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
// Define the number of characters for 'positive\n' & 'negative\n'
#define MAX_CHARACTERS 9

void extract_data(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS]);
void shuffleArr(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS]);
void split_data(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS], int training_data[MAX_ROWS][NUM_POSITIONS], int training_labels[MAX_ROWS],
				int testing_data[MAX_ROWS][NUM_POSITIONS], int testing_labels[MAX_ROWS], int split_ratio);
void calculatePriors(int data[][NUM_POSITIONS], int labels[], double priors[]);
void calculateLikelihoods(int data[][NUM_POSITIONS], int labels[], double likelihoods[NUM_POSITIONS][MAX_ROWS][NUM_CLASSES], double priors[]);
