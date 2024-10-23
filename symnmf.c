#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double **sym_c(double **pointsMatrix, int N, int dim);
double **ddg_c(double **pointsMatrix, int N, int dim);
double **norm_c(double **pointsMatrix, int N, int dim);
double **symnmf_c(double **initialH, double **W, int N, int k);

int my_strcmp(char *str1, char *str2);
void printMatrix(double **mtrx, int rows, int cols);
double squaredDistance(double *p1, double *p2, int dim);
double calcSquaredFrobenius(double **A, int rows, int cols);
double **mallocMatrix(int rows, int cols);
void freeMatrix(double **mtrx, int rows);

int main(int argc, char **argv) {
    int i, rows, cols, row, col;
    double n;
    char c;
    double **dataPointsMatrix, **returnMatrix;
    char *goal;
    FILE *file;

    if (argc != 4) { // Change the expected arguments to include k
        printf("Usage: %s <goal> <data_file> <k>\n", argv[0]);
        exit(1);
    }

    goal = argv[1];
    int k = atoi(argv[3]); // Read k from command line
    file = fopen(argv[2], "r");

    if (file == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    rows = 0;
    cols = 0;
    while (fscanf(file, "%lf%c", &n, &c) == 2) {
        if (rows == 0) {
            cols++;
        }
        if (c == '\n') {
            rows++;
        }
    }
    rewind(file);

    dataPointsMatrix = mallocMatrix(rows, cols);

    rewind(file);
    row = 0, col = 0;
    while (fscanf(file, "%lf%c", &n, &c) == 2) {
        dataPointsMatrix[row][col] = n;
        col++;
        if (c == '\n') {
            row++;
            col = 0;
        }
    }

    fclose(file);

    if (my_strcmp(goal, "sym") == 0) {
        returnMatrix = sym_c(dataPointsMatrix, rows, cols);
        printMatrix(returnMatrix, rows, rows);
        freeMatrix(returnMatrix, rows);
    }
    if (my_strcmp(goal, "ddg") == 0) {
        returnMatrix = ddg_c(dataPointsMatrix, rows, cols);
        printMatrix(returnMatrix, rows, rows);
        freeMatrix(returnMatrix, rows);
    }
    if (my_strcmp(goal, "norm") == 0) {
        returnMatrix = norm_c(dataPointsMatrix, rows, cols);
        printMatrix(returnMatrix, rows, rows);
        freeMatrix(returnMatrix, rows);
    }
    if (my_strcmp(goal, "symnmf") == 0) {
        // Assuming initialH and W are provided; you need to set them up properly.
        double **initialH = mallocMatrix(rows, k); // Example initialization
        double **W = mallocMatrix(rows, cols); // Replace with actual W matrix

        // Call symnmf_c with the provided k value
        returnMatrix = symnmf_c(initialH, W, rows, k);
        printMatrix(returnMatrix, rows, k); // Adjust printMatrix to show k columns
        freeMatrix(returnMatrix, rows);
        
        freeMatrix(initialH, rows);
        freeMatrix(W, rows);
    }

    freeMatrix(dataPointsMatrix, rows);
    return 0;
}

int my_strcmp(char *str1, char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return *str1 - *str2;
        }
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

void printMatrix(double **mtrx, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (j < cols - 1) {
                printf("%.4f,", mtrx[i][j]);
            } else {
                printf("%.4f", mtrx[i][j]);
            }
        }
        printf("\n");
    }
}

double squaredDistance(double *p1, double *p2, int dim) {
    double sum = 0.0;
    for (int i = 0; i < dim; i++) {
        double diff = p1[i] - p2[i];
        sum += pow(diff, 2);
    }
    return sum;
}

double calcSquaredFrobenius(double **A, int rows, int cols) {
    double norm = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            norm += pow(A[i][j], 2);
        }
    }
    return norm;
}

double **mallocMatrix(int rows, int cols) {
    double **A = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        A[i] = (double *)malloc(cols * sizeof(double));
        for (int j = 0; j < cols; j++) {
            A[i][j] = 0.0;
        }
    }
    return A;
}

void freeMatrix(double **mtrx, int rows) {
    for (int i = 0; i < rows; i++) {
        free(mtrx[i]);
    }
    free(mtrx);
}

double **sym_c(double **pointsMatrix, int N, int dim) {
    double **similarityMatrix = mallocMatrix(N, N);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) {
                double dist = squaredDistance(pointsMatrix[i], pointsMatrix[j], dim);
                similarityMatrix[i][j] = exp((-dist) / 2.0);
            } else {
                similarityMatrix[i][j] = 0.0;
            }
        }
    }

    return similarityMatrix;
}

double **ddg_c(double **pointsMatrix, int N, int dim) {
    double **similarityMatrix = sym_c(pointsMatrix, N, dim);
    double **degreeMatrix = mallocMatrix(N, N);

    for (int i = 0; i < N; i++) {
        double degree = 0.0;
        for (int j = 0; j < N; j++) {
            degree += similarityMatrix[i][j];
        }
        degreeMatrix[i][i] = degree;
    }

    freeMatrix(similarityMatrix, N);
    return degreeMatrix;
}

double **norm_c(double **pointsMatrix, int N, int dim) {
    double **similarityMatrix = sym_c(pointsMatrix, N, dim);
    double **degreeMatrix = ddg_c(pointsMatrix, N, dim);
    double **normalizedMatrix = mallocMatrix(N, N);

    for (int i = 0; i < N; i++) {
        double d = degreeMatrix[i][i];
        d = (d == 0) ? 1e-10 : d;  // Prevent division by zero
        for (int j = 0; j < N; j++) {
            normalizedMatrix[i][j] = similarityMatrix[i][j] * pow(d, -0.5);
        }
    }

    for (int j = 0; j < N; j++) {
        double d = degreeMatrix[j][j];
        d = (d == 0) ? 1e-10 : d;  // Prevent division by zero
        for (int i = 0; i < N; i++) {
            normalizedMatrix[i][j] *= pow(d, -0.5);
        }
    }

    freeMatrix(similarityMatrix, N);
    freeMatrix(degreeMatrix, N);
    return normalizedMatrix;
}

double **symnmf_c(double **initialH, double **W, int N, int k) {
    double **prevH = mallocMatrix(N, k);
    double **nextH = mallocMatrix(N, k);
    double **WH = mallocMatrix(N, k);
    double **HHTH = mallocMatrix(N, k);
    double **temp = mallocMatrix(N, N);
    double **condition = mallocMatrix(N, k);

    // Initialize prevH
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < k; j++) {
            prevH[i][j] = initialH[i][j];
        }
    }

    for (int iter = 0; iter < 300; iter++) {
        // Compute WH
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < k; j++) {
                WH[i][j] = 0.0;
                for (int m = 0; m < N; m++) {
                    WH[i][j] += W[i][m] * prevH[m][j];
                }
            }
        }

        // Compute HHTH
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                temp[i][j] = 0.0;
                for (int m = 0; m < k; m++) {  // Use k
                    temp[i][j] += prevH[i][m] * prevH[j][m];
                }
            }
        }

        // Populate nextH
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < k; j++) {
                double c = (HHTH[i][j] == 0) ? 0 : (WH[i][j] / HHTH[i][j]);
                nextH[i][j] = prevH[i][j] * (1.0 - 0.5 + 0.5 * c);
            }
        }

        // Check stopping condition
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < k; j++) {
                condition[i][j] = nextH[i][j] - prevH[i][j];
            }
        }

        if (calcSquaredFrobenius(condition, N, k) < 0.0001) {
            break;
        }

        // Update prevH
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < k; j++) {
                prevH[i][j] = nextH[i][j];
            }
        }
    }

    freeMatrix(prevH, N);
    freeMatrix(WH, N);
    freeMatrix(HHTH, N);
    freeMatrix(temp, N);
    freeMatrix(condition, N);

    return nextH;  // This should return the updated H matrix
}
