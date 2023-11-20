#include "board.h"
#include "naivebayes.h"

// Extract all data from tic-tac-toe.data
void extract_data(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS])
{
	FILE *file = fopen("data/tic-tac-toe.data", "r");

	if (file == NULL)
	{
		perror("Error opening file\n");
		printf("Cannot open file\n");
	}

	char temp_data[MAX_ROWS][NUM_POSITIONS];

	// To temporarily store all outcomes of game states
	char temp_label;

	// Read and extract data from the file
	int row = 0;
	while (row < MAX_ROWS)
	{

		if (fscanf(file, "%c,%c,%c,%c,%c,%c,%c,%c,%c,%c",
				   &temp_data[row][0], &temp_data[row][1], &temp_data[row][2],
				   &temp_data[row][3], &temp_data[row][4], &temp_data[row][5],
				   &temp_data[row][6], &temp_data[row][7], &temp_data[row][8],
				   &temp_label) == NUM_POSITIONS + 1)
		{

			if (temp_label == 'p')
			{
				labels[row] = 1;
			}
			else if (temp_label == 'n')
			{
				labels[row] = 0;
			}

			for (int i = 0; i < NUM_POSITIONS; i++)
			{
				if (temp_data[row][i] == X_SYMBOL)
				{
					data[row][i] = X;
				}
				else if (temp_data[row][i] == O_SYMBOL)
				{
					data[row][i] = O;
				}
				else
				{
					data[row][i] = EMPTY;
				}
			}

			row++;
		}
	}

	fclose(file);

	// return 0;
}

void shuffleArr(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS])
{					   // generate random array element
	srand(time(NULL)); // use time to get different seed value to start
	for (int i = MAX_ROWS - 1; i > 0; i--)
	{
		int j = rand() % (i + 1); // randomly choose an index from 0 to i

		for (int k = 0; k < NUM_POSITIONS; k++)
		{
			int temp = data[i][k];
			data[i][k] = data[j][k]; // swap current element with element placed in jth location
			data[j][k] = temp;
			temp = labels[i];
			labels[i] = labels[j];
			labels[j] = temp;
		}
	}
}

// Split dataset into training and testing according to ratio
void split_data(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS], int training_data[MAX_ROWS][NUM_POSITIONS], int training_labels[MAX_ROWS],
				int testing_data[MAX_ROWS][NUM_POSITIONS], int testing_labels[MAX_ROWS], int split_ratio)
{

	int num_rows = round(split_ratio * MAX_ROWS / 100);

	shuffleArr(data, labels);

	for (int i = 0; i < num_rows; i++)
	{
		for (int j = 0; j < NUM_POSITIONS; j++)
		{
			training_data[i][j] = data[i][j];
			training_labels[i] = labels[i];
		}
	}

	for (int i = num_rows; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < NUM_POSITIONS; j++)
		{
			testing_data[i - num_rows][j] = data[i][j];
			testing_labels[i - num_rows] = labels[i];
		}
	}
}

// Function to calculate prior probabilities
void calculatePriors(int data[][NUM_POSITIONS], int labels[], double priors[])
{
	for (int i = 0; i < NUM_CLASSES; i++)
	{
		priors[i] = 0.0;
		for (int j = 0; j < MAX_ROWS; j++)
		{
			if (labels[j] == i)
			{
				priors[i]++;
			}
		}
		priors[i] /= MAX_ROWS;
	}
}

// Function to calculate likelihood probabilities
void calculateLikelihoods(int data[][NUM_POSITIONS], int labels[], double likelihoods[NUM_POSITIONS][MAX_ROWS][NUM_CLASSES], double priors[])
{
	for (int pos = 0; pos < NUM_POSITIONS; pos++)
	{
		for (int val = X; val <= EMPTY; val++)
		{
			for (int cls = 0; cls < NUM_CLASSES; cls++)
			{
				int count = 0;
				for (int i = 0; i < MAX_ROWS; i++)
				{
					if (data[i][pos] == val && labels[i] == cls)
					{
						count++;
					}
				}
				likelihoods[pos][val][cls] = (double)(count + 1) / (priors[cls] * MAX_ROWS + 3);
			}
		}
	}
}