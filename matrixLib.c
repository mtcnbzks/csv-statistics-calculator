/**
 * @file matrixLib.c
 * @description Some vector and matrix functions.
 * @assignment Project 1
 * @date 28.11.2022
 * @author Mehmetcan Bozkuş - mehmetcan.bozkus@stu.fsm.edu.tr
 */

#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc and free
#include <math.h>   // for sqrt()
#include <time.h>   // for random seed

#include "matrixLib.h" // for function prototypes

float *returnVector(int size) {
    float *vector = (float *) malloc(size * sizeof(float)); // allocate memory for the vector

    if (vector == NULL) // if the memory allocation fails
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    return vector;
}

float **returnMatrix(int row, int col) {

    float **matrix = (float **) malloc(row * sizeof(float *)); // allocate memory for the matrix rows
    for (int i = 0; i < row; i++) {
        matrix[i] = (float *) malloc(col * sizeof(float)); // allocate memory for the matrix columns
    }

    if (matrix == NULL || matrix[0] == NULL) // memory allocation failed
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    return matrix;
}

void freeMatrix(float **mat, int row) {
    for (int i = 0; i < row; i++) {
        free(mat[i]); // free the memory of the matrix rows
    }
    free(mat); // free the memory of the matrix
}

float mean(float *vec, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += vec[i]; // sum the elements of the vector
    }

    return sum / size; // return the mean of the vector
}

float correlation(float *vec, float *vec2, int size) {
    float cov = covariance(vec, vec2, size); // calculate the covariance of the vectors
    float mean1 = mean(vec, size);           // calculate the mean of the first vector
    float mean2 = mean(vec2, size);          // calculate the mean of the second vector

    float std1 = 0; // standard deviation of the first vector
    float std2 = 0; // standard deviation of the second vector

    for (int i = 0; i < size; i++) {
        std1 += pow(vec[i] - mean1, 2);  // sum the squared differences of the first vector
        std2 += pow(vec2[i] - mean2, 2); // sum the squared differences of the second vector
    }
    std1 = sqrt(std1 / (size - 1)); // calculate the standard deviation of the first vector
    std2 = sqrt(std2 / (size - 1)); // calculate the standard deviation of the second vector

    return cov / (std1 * std2); // return the correlation of the vectors
}

float covariance(float *vec1, float *vec2, int size) {
    float mean1 = mean(vec1, size); // calculate the mean of the first vector
    float mean2 = mean(vec2, size); // calculate the mean of the second vector

    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += (vec1[i] - mean1) * (vec2[i] - mean2); // sum the differences of the vectors
    }

    return sum / (size - 1); // return the covariance of the vectors
}

float **matrixMultiplication(float **mat1, float **mat2, int row1, int col1, int row2, int col2) {
    if (col1 != row2) {
        printf("Error: matrixMultiplication: col1 != row2\n"); // check if the matrices can be multiplied
        exit(1);
    }

    float **result = returnMatrix(row1, col2); // allocate memory for the result matrix

    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            float sum = 0;
            for (int k = 0; k < col1; k++) {
                sum += mat1[i][k] * mat2[k][j]; // multiply the matrices
            }
            result[i][j] = sum; // store the result in the result matrix
        }
    }

    return result; // return the result matrix
}

float **matrixTranspose(float **mat, int row, int col) {
    float **transpose = returnMatrix(col, row);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            transpose[j][i] = mat[i][j]; // transpose the matrix
        }
    }
    return transpose;
}

float *rowMeans(float **mat, int row, int col) {
    float *means = returnVector(row);
    for (int i = 0; i < row; i++) {
        means[i] = mean(mat[i], col); // calculate the mean of each row
    }

    return means;
}

float *columnMeans(float **mat, int row, int col) {
    float *means = returnVector(col);
    float **transpose = matrixTranspose(mat, row, col); // transpose the matrix to calculate the column means easily
    for (int i = 0; i < col; i++) {
        means[i] = mean(transpose[i],
                        row); // calculate the mean of each column actaullay the row means of the transposed matrix
    }
    freeMatrix(transpose, col); // free the memory of the transposed matrix

    return means;
}

float **covarianceMatrix(float **mat, int row, int col) {
    float **covMat;
    float *means = columnMeans(mat, row, col); // calculate the column means of the matrix

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            mat[i][j] -= means[j]; // subtract the column means from the matrix
        }
    }
    float **trans = matrixTranspose(mat, row, col);


    covMat = matrixMultiplication(trans, mat, col, row, row,
                                  col); // multiply the matrix with its transposed matrix

    for (int i = 0; i < col; i++) {
        for (int j = 0; j < col; j++) {
            covMat[i][j] /= (row); // divide the elements of the covariance matrix by (row - 1)
        }
    }
    free(means); // free the memory of the column means
    freeMatrix(trans, col); // free the memory of the transposed matrix

    return covMat;
}

float determinant(float **mat, int row) {
    if (row != 3) // check if the matrix is 3x3
    {
        printf("Error: determinant: row != 3\n");
        printf("Because this function is only for 3x3 matrix\n");
        exit(1);
    }

    int col = row + 2;                         // the number of columns of the augmented matrix
    mat = realloc(mat, row * sizeof(float *)); // reallocate memory for the augmented matrix rows
    for (int i = 0; i < row; i++) {
        mat[i] = realloc(mat[i], col * sizeof(float)); // allocate memory for the augmented matrix
    }

    for (int i = 0; i < row; i++) {
        for (int j = row; j < col; j++) {
            mat[i][j] = mat[i][j - row]; // copy the matrix to the right side of the augmented matrix
        }
    }

    float det = 0;

    for (int i = 0; i < row; i++) {
        float product = 1;
        for (int j = 0; j < row; j++) {
            int k = row - 1 + j - i; // calculate the index of the element to multiply
            product *= mat[j][k];  // multiply the elements of the diagonal
            /// printf("i + j: %d,\n", i + j);
        }
        det += product; // calculate the determinant of the matrix
    }

    for (int i = 0; i < row; ++i) {
        float product = 1;
        for (int j = 0; j < row; j++) {
            int k = row - 1 + i - j; // calculate the index of the element to multiply
            product *= mat[j][k]; // multiply the elements of the diagonal
        }
        det -= product; // calculate the determinant of the matrix
    }

    /*
    * using the formula of the determinant of the augmented matrix

    det += mat[0][0] * mat[1][1] * mat[2][2];
    det += mat[0][1] * mat[1][2] * mat[2][3];
    det += mat[0][2] * mat[1][3] * mat[2][4];

    det -= mat[0][2] * mat[1][1] * mat[2][0];
    det -= mat[0][3] * mat[1][2] * mat[2][1];
    det -= mat[0][4] * mat[1][3] * mat[2][2];

    */
    return det;
}

void printVector(float *vec, int N) {
    for (int i = 0; i < N; i++) {
        printf("%f ", vec[i]); // print the vector
    }
    printf("\n"); // print a new line
}

void printMatrix(float **mat, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%f ", mat[i][j]); // print the matrix
        }
        printf("\n"); // print a new line after each row
    }
}

// my functions

float calculateVariance(float *vec, int row) {
    float m = mean(vec, row);
    float sum = 0;
    for (int i = 0; i < row; ++i) {
        sum += (float) pow(vec[i] - m, 2);
    }
    return sum / (float) (row);
}

void fillVector(float *vec, int size, int seed) {
    srand(seed);

    for (int i = 0; i < size; i++) {
        vec[i] = (float) (1 + rand() % 10); // fill the vector with random numbers
    }
}

void fillMatrix(float **mat, int row, int col, int seed) {
    srand(seed);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            mat[i][j] = (float) (1 + rand() % 10); // fill the matrix with random numbers
        }
    }
}

