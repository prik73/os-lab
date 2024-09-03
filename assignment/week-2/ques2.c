#include <stdio.h>

#define MAX_SIZE 100

void readMatrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    printf("Enter the elements of the matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
}

void transposeMatrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE], int transpose[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
}

void printMatrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int rows, cols;

    printf("Enter the number of rows: ");
    scanf("%d", &rows);
    printf("Enter the number of columns: ");
    scanf("%d", &cols);

    if (rows > MAX_SIZE || cols > MAX_SIZE) {
        printf("Error: matrix size exceeds maximum allowed size of %d\n", MAX_SIZE);
        return 1;
    }

    int matrix[MAX_SIZE][MAX_SIZE];
    int transpose[MAX_SIZE][MAX_SIZE];

    readMatrix(rows, cols, matrix);
    transposeMatrix(rows, cols, matrix, transpose);

    printf("The transpose of the matrix is:\n");
    printMatrix(cols, rows, transpose);

    return 0;
}
