// Extract all data from tic-tac-toe.data
void extract_data(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS]) {
    FILE *file = fopen("../data/tic-tac-toe.data", "r"); 

    if (file == NULL) {
        perror("Error opening file");
        printf("Cannot open file");
    }

    char temp_data[MAX_ROWS][NUM_POSITIONS];

    // To temporarily store all outcomes of game states
    char temp_label;

    // Read and extract data from the file
    int row = 0;
    while (row < MAX_ROWS) {

        if (fscanf(file, "%c,%c,%c,%c,%c,%c,%c,%c,%c,%c",
            &temp_data[row][0], &temp_data[row][1], &temp_data[row][2],
            &temp_data[row][3], &temp_data[row][4], &temp_data[row][5],
            &temp_data[row][6], &temp_data[row][7], &temp_data[row][8],
            &temp_label) == NUM_POSITIONS + 1) {
                
            if (temp_label == 'p') {
                labels[row] = 1;
            }
            else if (temp_label == 'n') { 
                labels[row] = 0;
            }

            for (int i = 0; i < NUM_POSITIONS; i++) {
                if (temp_data[row][i] == 'x') {
                    data[row][i] = X;
                }
                else if (temp_data[row][i] == 'o')
                {
                    data[row][i] = O;
                }
                else {
                    data[row][i] = EMPTY;
                }
            }

            row++;
        }
        
    }

    fclose(file);

    // return 0;
}

void shuffleArr (int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS]) {           //generate random array element
   srand (time(NULL));                 //use time to get different seed value to start
   for (int i = MAX_ROWS-1; i > 0; i--) {
      int j = rand() % (i+1);           //randomly choose an index from 0 to i

      for (int k = 0; k < NUM_POSITIONS; k++) {
        int temp = data[i][k];
        data[i][k] = data[j][k]; //swap current element with element placed in jth location
        data[j][k] = temp;
        temp = labels[i];
        labels[i] = labels[j];
        labels[j] = temp; 
      }                
   }
}

// Split dataset into training and testing according to ratio
void split_data(int data[MAX_ROWS][NUM_POSITIONS], int labels[MAX_ROWS], int training_data[MAX_ROWS][NUM_POSITIONS], int training_labels[MAX_ROWS], 
                   int testing_data[MAX_ROWS][NUM_POSITIONS], int testing_labels[MAX_ROWS], int split_ratio) {
                    
    int num_rows = round(split_ratio * MAX_ROWS / 100);

    shuffleArr(data, labels);

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < NUM_POSITIONS; j++) {
            training_data[i][j] = data[i][j];
            training_labels[i] = labels[i];
        }
    }

    for (int i = num_rows; i < MAX_ROWS; i++) {
        for (int j = 0; j < NUM_POSITIONS; j++) {
            testing_data[i - num_rows][j] = data[i][j];
            testing_labels[i - num_rows] = labels[i];
        }
    }
}

// Function to calculate prior probabilities
void calculatePriors(int data[][NUM_POSITIONS], int labels[], double priors[]) {
    for (int i = 0; i < NUM_CLASSES; i++) {
        priors[i] = 0.0;
        for (int j = 0; j < MAX_ROWS; j++) {
            if (labels[j] == i) {
                priors[i]++;
            }
        }
        priors[i] /= MAX_ROWS;
    }
}

// Function to calculate likelihood probabilities
void calculateLikelihoods(int data[][NUM_POSITIONS], int labels[], double likelihoods[NUM_POSITIONS][MAX_ROWS][NUM_CLASSES], double priors[]) {
    for (int pos = 0; pos < NUM_POSITIONS; pos++) {
        for (int val = X; val <= EMPTY; val++) {
            for (int cls = 0; cls < NUM_CLASSES; cls++) {
                int count = 0;
                for (int i = 0; i < MAX_ROWS; i++) {
                    if (data[i][pos] == val && labels[i] == cls) {
                        count++;
                    }
                }
                likelihoods[pos][val][cls] = (double)(count + 1) / (priors[cls] * MAX_ROWS + 3);
            }
        }
    }
}

// Function to predict the next move
int predictNextMove(char board[], double priors[], double likelihoods[NUM_POSITIONS][MAX_ROWS][NUM_CLASSES]) {
    // Calculate the posterior probability for each possible move
    int temp_board[NUM_POSITIONS];
    
    for (int pos = 0; pos < NUM_POSITIONS; pos++) {
        if (board[pos] == EMPTY_SYMBOL) {
            temp_board[pos] = EMPTY;
        }
        else if (board[pos] == X_SYMBOL)
        {
            temp_board[pos] = X;
        }
        else if (board[pos] == O_SYMBOL)
        {
            temp_board[pos] = O;
        }    
        
    }

    double posteriors[NUM_POSITIONS] = {0.0};
    
    for (int pos = 0; pos < NUM_POSITIONS; pos++) {
        if (temp_board[pos] == EMPTY) {
            for (int cls = 0; cls < NUM_CLASSES; cls++) {
                posteriors[pos] += priors[cls];
                posteriors[pos] *= likelihoods[pos][temp_board[pos]][cls];
            }
        }
    }

    // Find the move with the highest posterior probability
    int best_move = 0;
    double max_posterior = posteriors[0];

    for (int pos = 1; pos < NUM_POSITIONS; pos++) {
        if (temp_board[pos] == EMPTY && posteriors[pos] > max_posterior) {
            max_posterior = posteriors[pos];
            best_move = pos;
        }
    }

    return best_move;
}

// Test split_dataset function. Should return training.data and testing.data with respective instances and outcomes according to split ratio.
/* void test_split_data(int training_data[MAX_ROWS][NUM_POSITIONS], int training_labels[MAX_ROWS], 
                        int testing_data[MAX_ROWS][NUM_POSITIONS], int testing_labels[MAX_ROWS]) {

    FILE *file = fopen("training.data", "w");
    for (int i = 0; i < 766; i++) {
        char temp[NUM_POSITIONS];
        for (int j = 0; j < NUM_POSITIONS; j++) {
            if (training_data[i][j] == X)
            {
                temp[j] = 'x';
            }
            else if (training_data[i][j] == O)
            {
                temp[j] = 'o';
            }
            else {
                temp[j] = 'b';
            }
        }  
        fprintf(file,"%c,%c,%c,%c,%c,%c,%c,%c,%c,%d\n",
            temp[0], temp[1], temp[2],
            temp[3], temp[4], temp[5],
            temp[6], temp[7], temp[8],
            training_labels[i]);
    }
    fclose(file);

    FILE *file2 = fopen("testing.data", "w");
    for (int i = 0; i < 192; i++) {
        char temp[NUM_POSITIONS];
        for (int j = 0; j < NUM_POSITIONS; j++) {
            if (testing_data[i][j] == X)
            {
                temp[j] = 'x';
            }
            else if (testing_data[i][j] == O)
            {
                temp[j] = 'o';
            }
            else {
                temp[j] = 'b';
            }
        }
        fprintf(file2,"%c,%c,%c,%c,%c,%c,%c,%c,%c,%d\n",
            temp[0], temp[1], temp[2],
            temp[3], temp[4], temp[5],
            temp[6], temp[7], temp[8],
            training_labels[i]);
    }
    fclose(file2); 
} */
