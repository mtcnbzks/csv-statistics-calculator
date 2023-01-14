float *returnVector(int size);                                                                    // tested
float **returnMatrix(int row, int col);                                                           // tested
void freeMatrix(float **mat, int row);                                                            // tested
float mean(float *vec, int size);                                                                 // tested
float correlation(float *vec, float *vec2, int size);                                             // tested
float covariance(float *vec1, float *vec2, int size);                                             // tested
float **matrixMultiplication(float **mat1, float **mat2, int row1, int col1, int row2, int col2); // tested
float **matrixTranspose(float **mat, int row, int col);                                           // tested
float *rowMeans(float **mat, int row, int col);                                                   // tested
float *columnMeans(float **mat, int row, int col);                                                // tested
float **covarianceMatrix(float **mat, int row, int col);                                          // tested
float determinant(float **mat, int row);                                                          // tested
void printVector(float *vec, int N);                                                              // tested
void printMatrix(float **mat, int row, int col);                                                  // tested

float calculateVariance(float *vec, int row);                                                     // tested
void fillVector(float *vec, int size, int seed);                                                  // tested
void fillMatrix(float **mat, int row, int col, int seed);                                         // tested