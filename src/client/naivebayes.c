#include "board.h"
#include "naivebayes.h"

// Extract all data from tic-tac-toe.data
void extract_data(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS])
{
    FILE *file = fopen("data/tic-tac-toe.data", "r");

    if (file == NULL)
    {
        perror("Error opening file");
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

            for (int i = 0; i < NUM_POSITIONS; ++i)
            {
                if (temp_data[row][i] == 'x')
                {
                    data[row][i] = X;
                }
                else if (temp_data[row][i] == 'o')
                {
                    data[row][i] = O;
                }
                else
                {
                    data[row][i] = EMPTY;
                }
            }

            ++row;
        }
    }

    fclose(file);

    // return 0;
}

void shuffleArr(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS])
{                      // generate random array element
    srand(time(NULL)); // use time to get different seed value to start
    for (int i = MAX_ROWS - 1; i > 0; --i)
    {
        int j = rand() % (i + 1); // randomly choose an index from 0 to i

        for (int k = 0; k < NUM_POSITIONS; ++k)
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
void split_data(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS], int training_data[MAX_TRAINING_ROWS][NUM_POSITIONS], int training_labels[MAX_TRAINING_ROWS],
                int testing_data[MAX_TESTING_ROWS][NUM_POSITIONS], int testing_labels[MAX_TESTING_ROWS], int split_ratio)
{

    int num_rows = round(split_ratio * MAX_ROWS / 100);

    shuffleArr(data, labels);

    for (int i = 0; i < num_rows; ++i)
    {
        for (int j = 0; j < NUM_POSITIONS; ++j)
        {
            training_data[i][j] = data[i][j];
            training_labels[i] = labels[i];
        }
    }

    for (int i = num_rows; i < MAX_ROWS; ++i)
    {
        for (int j = 0; j < NUM_POSITIONS; ++j)
        {
            testing_data[i - num_rows][j] = data[i][j];
            testing_labels[i - num_rows] = labels[i];
        }
    }
}

// Function to calculate prior probabilities
void calculatePriors(int labels[MAX_ROWS], double priors[NUM_CLASSES])
{
    for (int i = 0; i < NUM_CLASSES; ++i)
    {
        priors[i] = 0.0;
        for (int j = 0; j < MAX_ROWS; ++j)
        {
            if (labels[j] == i)
            {
                ++priors[i];
            }
        }
        priors[i] /= MAX_ROWS;
    }
}

// Function to calculate conditional probabilities
void calculateLikelihoods(int data[][NUM_POSITIONS], int labels[], double likelihoods[NUM_POSITIONS][EMPTY + 1][NUM_CLASSES], double priors[NUM_CLASSES])
{
    for (int pos = 0; pos < NUM_POSITIONS; ++pos)
    {
        for (int val = X; val <= EMPTY; ++val)
        {
            for (int cls = 0; cls < NUM_CLASSES; ++cls)
            {
                int count = 0;
                for (int i = 0; i < MAX_ROWS; ++i)
                {
                    if (data[i][pos] == val && labels[i] == cls)
                    {
                        ++count;
                    }
                }
                likelihoods[pos][val][cls] = (double)(count + 1) / (priors[cls] * MAX_ROWS + 3);
            }
        }
    }
}

// Function to count the number of empty positions on the board
int countEmptyPositions(int int_board[NUM_POSITIONS], int empty_pos[NUM_POSITIONS])
{
    int count = 0;
    for (int pos = 0; pos < NUM_POSITIONS; ++pos)
    {
        if (int_board[pos] == EMPTY)
        {
            empty_pos[count] = pos;
            count++;
        }
    }

    return count;
}

void changeToIntBoard(char char_board[NUM_POSITIONS], int int_board[NUM_POSITIONS])
{
    for (int pos = 0; pos < NUM_POSITIONS; ++pos)
    {
        if (char_board[pos] == EMPTY_SYMBOL)
        {
            int_board[pos] = EMPTY;
        }
        else if (char_board[pos] == X_SYMBOL)
        {
            int_board[pos] = X;
        }
        else if (char_board[pos] == O_SYMBOL)
        {
            int_board[pos] = O;
        }
    }
}

void naiveBayesLearn(int data[][NUM_POSITIONS], int labels[], double likelihoods[NUM_POSITIONS][EMPTY + 1][NUM_CLASSES], double priors[NUM_CLASSES])
{
    calculatePriors(labels, priors);
    calculateLikelihoods(data, labels, likelihoods, priors);
}

int naiveBayesPredict(int int_board[NUM_POSITIONS], double priors[NUM_CLASSES], double likelihoods[NUM_POSITIONS][EMPTY + 1][NUM_CLASSES], double *positive_possibility, double *negative_possibility)
{
    double posteriors[NUM_CLASSES];

    for (int cls = 0; cls < NUM_CLASSES; ++cls)
    {
        posteriors[cls] = priors[cls];
        for (int pos = 0; pos < NUM_POSITIONS; ++pos)
        {
            posteriors[cls] *= likelihoods[pos][int_board[pos]][cls];
        }
    }

    if (posteriors[0] > posteriors[1])
    {
        *negative_possibility = posteriors[0];
        return 0;
    }

    else
    {
        *positive_possibility = posteriors[1];
        return 1;
    }
}

void calcStats(int data_rows, int training_data[MAX_TRAINING_ROWS][NUM_POSITIONS], int training_labels[MAX_TRAINING_ROWS], int testing_data[MAX_TESTING_ROWS][NUM_POSITIONS], int testing_labels[MAX_TESTING_ROWS], double likelihoods[NUM_POSITIONS][EMPTY + 1][NUM_CLASSES], double priors[NUM_CLASSES], double *accuracy, double *errorcount, double *truepos, double *trueneg, double *falsepos, double *falseneg, double *positive, double *negative)
{
    double pos;
    double neg;
    *errorcount = 0;
    *accuracy = 0;
    *truepos = 0;
    *trueneg = 0;
    *falsepos = 0;
    *falseneg = 0;
    *positive = 0;
    *negative = 0;

    for (int i = 0; i < data_rows; ++i)
    {
        int result = naiveBayesPredict(testing_data[i], priors, likelihoods, &pos, &neg);

        if (result == 1 && result == testing_labels[i])
        {
            *accuracy += 1;
            *truepos += 1;
            *positive += 1;
        }

        else if (result == 0 && result == testing_labels[i])
        {
            *accuracy += 1;
            *trueneg += 1;
            *negative += 1;
        }

        if (result == 1 && result != testing_labels[i])
        {
            *errorcount += 1;
            *falsepos += 1;
            *negative += 1;
        }

        else if (result == 0 && result != testing_labels[i])
        {
            *errorcount += 1;
            *falseneg += 1;
            *positive += 1;
        }
    }

    *accuracy /= data_rows;
    *errorcount /= data_rows;
}

void validate(int training_data[MAX_TRAINING_ROWS][NUM_POSITIONS], int training_labels[MAX_TRAINING_ROWS], int testing_data[MAX_TESTING_ROWS][NUM_POSITIONS], int testing_labels[MAX_TESTING_ROWS], double likelihoods[NUM_POSITIONS][EMPTY + 1][NUM_CLASSES], double priors[NUM_CLASSES])
{
    double errorcount, accuracy, truepos, trueneg, falsepos, falseneg, positive, negative;

    naiveBayesLearn(training_data, training_labels, likelihoods, priors);

    FILE *file = fopen("data/confusion-matrix.txt", "w");

    if (file == NULL)
    {
        perror("Error opening file");
    }

    calcStats(MAX_TESTING_ROWS, training_data, training_labels, testing_data, testing_labels, likelihoods, priors, &accuracy, &errorcount, &truepos, &trueneg, &falsepos, &falseneg, &positive, &negative);
    fprintf(file, "TEST DATA STATISTICS & CONFUSION MATRIX \n");
    fprintf(file, "---------------------------------------------\n");
    fprintf(file, "Accuracy: %.4f\n", accuracy);
    fprintf(file, "Probability of Error: %.4f\n", errorcount);
    fprintf(file, "True Positive: %d / %d\n", (int)truepos, (int)positive);
    fprintf(file, "False Positive: %d / %d\n", (int)falsepos, (int)negative);
    fprintf(file, "True Negative: %d / %d\n", (int)trueneg, (int)negative);
    fprintf(file, "False Negative: %d / %d\n", (int)falseneg, (int)positive);
    fprintf(file, "Recall: %.4f\n", truepos / (truepos + falseneg));
    fprintf(file, "Precision: %.4f\n\n", truepos / (truepos + falsepos));
    fprintf(file, "     									   Confusion Matrix (Test Dataset)\n");
    fprintf(file, "         										  Actual Values\n");
    fprintf(file, "    										Positive          Negative\n");
    fprintf(file, "										-----------------------------------\n");
    fprintf(file, "										| True Positive  | False Positive |\n");
    fprintf(file, "							  Positive	|        =       |       =        |\n");
    fprintf(file, "										|    %d         |    %d          |\n", (int)truepos, (int)falsepos);
    fprintf(file, "		    Predicted Values			|----------------|----------------|\n");
    fprintf(file, "										| False Negative | True Negative  |\n");
    fprintf(file, "							  Negative	|       =        |       =        |\n");
    fprintf(file, "										|    %d          |    %d          |\n", (int)falseneg, (int)trueneg);
    fprintf(file, "										-----------------------------------\n");

    fprintf(file, "\n\n");

    calcStats(MAX_TRAINING_ROWS, training_data, training_labels, testing_data, testing_labels, likelihoods, priors, &accuracy, &errorcount, &truepos, &trueneg, &falsepos, &falseneg, &positive, &negative);
    fprintf(file, "TRAINING DATA STATISTICS & CONFUSION MATRIX \n");
    fprintf(file, "---------------------------------------------\n");
    fprintf(file, "Accuracy: %.4f\n", accuracy);
    fprintf(file, "Probability of Error: %.4f\n", errorcount);
    fprintf(file, "True Positive: %d / %d\n", (int)truepos, (int)positive);
    fprintf(file, "False Positive: %d / %d\n", (int)falsepos, (int)negative);
    fprintf(file, "True Negative: %d / %d\n", (int)trueneg, (int)negative);
    fprintf(file, "False Negative: %d / %d\n", (int)falseneg, (int)positive);
    fprintf(file, "Recall: %.4f\n", truepos / (truepos + falseneg));
    fprintf(file, "Precision: %.4f\n\n", truepos / (truepos + falsepos));
    fprintf(file, "     									Confusion Matrix (Training Dataset)\n");
    fprintf(file, "         										  Actual Values\n");
    fprintf(file, "    										Positive          Negative\n");
    fprintf(file, "										-----------------------------------\n");
    fprintf(file, "										| True Positive  | False Positive |\n");
    fprintf(file, "							  Positive	|        =       |       =        |\n");
    fprintf(file, "										|    %d         |    %d         |\n", (int)truepos, (int)falsepos);
    fprintf(file, "		    Predicted Values			|----------------|----------------|\n");
    fprintf(file, "										| False Negative | True Negative  |\n");
    fprintf(file, "							  Negative	|       =        |       =        |\n");
    fprintf(file, "										|    %d         |    %d         |\n", (int)falseneg, (int)trueneg);
    fprintf(file, "										-----------------------------------\n");

    fclose(file);
}