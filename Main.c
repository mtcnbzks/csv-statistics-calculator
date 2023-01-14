/**
 * @file Main.c
 * @description // Reading CSV file and writing some calculations to a new file
 * @assignment Project 2
 * @date 17.12.2022
 * @author Mehmetcan Bozkuş - mehmetcan.bozkus@stu.fsm.edu.tr
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // for strtrim func
#include "matrixLib.h"

typedef enum {
    Irissetosa = 1,
    IrisVersicolor,
    IrisVirginica
} IrisType;

typedef struct {
    float SepalLengthCm;
    float SepalWidthCm;
    float PetalLengthCm;
    float PetalWidthCm;
    IrisType IrisType;
} IrisDataType;

void writeStringToFile(FILE *, char *);

void strtrim(char *);

void writeMatrixToFile(FILE *, float **, int, int);

int main() {
    // read Iris.csv and IrisStatistic file
    FILE *fp = fopen("Iris.csv", "r"); // read file
    FILE *statsPtr = fopen("IrisStatistic.txt", "w"); // write file

    // check if file opened successfully
    if (fp == NULL || statsPtr == NULL) {
        printf("Error opening file\n");
    } else {
        printf("File opened successfully\n");
    }

    // read first line for column count
    char firstLine[1024];
    int countOfIris = atoi(fgets(firstLine, 1024, fp));
    // pass one line (column names line)
    fgets(firstLine, 1024, fp);

    // create array for IrisDataType
    IrisDataType *IrisDataArray = (IrisDataType *) malloc(sizeof(IrisDataType) * countOfIris);

    // read file line by line
    for (int i = 0; i < countOfIris; i++) {
        // get line
        char line[512];
        fgets(line, 512, fp);

        // get first token
        char *token;
        strtok(line, ","); // pass id column

        // get each column value and switch case
        for (int j = 1; j <= 5; ++j) {

            token = strtok(NULL, ",");

            switch (j) {
                case 1:
                    IrisDataArray[i].SepalLengthCm = atof(token);
                    continue;
                case 2:
                    IrisDataArray[i].SepalWidthCm = atof(token);
                    continue;
                case 3:
                    IrisDataArray[i].PetalLengthCm = atof(token);
                    continue;
                case 4:
                    IrisDataArray[i].PetalWidthCm = atof(token);
                    continue;
                case 5:
                    strtrim(token); // trim token for IrisType comparison

                    // assign IrisType to IrisDataArray element according to token value
                    // (Iris-setosa, Iris-versicolor, Iris-virginica)
                    if (strcmp(token, "Iris-setosa") == 0) {
                        IrisDataArray[i].IrisType = Irissetosa;
                    } else if (strcmp(token, "Iris-versicolor") == 0) {
                        IrisDataArray[i].IrisType = IrisVersicolor;
                    } else if (strcmp(token, "Iris-virginica") == 0) {
                        IrisDataArray[i].IrisType = IrisVirginica;
                    }
                    continue;
                default:
                    printf("Error in switch case. Exiting...");
                    exit(1);
            }
        }
    }
    fclose(fp);

    // print IrisDataArray
    // for (int i = 0; i < countOfIris; i++)
    // {
    //     // print array
    //     printf("%d. SepalLengthCm: %f, SepalWidthCm: %f, PetalLengthCm: %f, PetalWidthCm: %f, IrisType: %d\n", i + 1,
    //            IrisDataArray[i].SepalLengthCm, IrisDataArray[i].SepalWidthCm,
    //            IrisDataArray[i].PetalLengthCm, IrisDataArray[i].PetalWidthCm, IrisDataArray[i].IrisType);
    // }

    // create matrix for IrisDataArray
    float **IrisDataMatrix = returnMatrix(countOfIris, 5);
    for (int i = 0; i < countOfIris; i++) {
        IrisDataMatrix[i][0] = IrisDataArray[i].SepalLengthCm;
        IrisDataMatrix[i][1] = IrisDataArray[i].SepalWidthCm;
        IrisDataMatrix[i][2] = IrisDataArray[i].PetalLengthCm;
        IrisDataMatrix[i][3] = IrisDataArray[i].PetalWidthCm;
        IrisDataMatrix[i][4] = IrisDataArray[i].IrisType;
    }

    // print IrisDataMatrix
    printf("SepLen   SepWid   PetLen   PetWid   IrisType\n");
    printMatrix(IrisDataMatrix, countOfIris, 5);

    // creating vectors for columns
    float *SepalLengthCmVector = returnVector(countOfIris);
    float *SepalWidthCmVector = returnVector(countOfIris);
    float *PetalLengthCmVector = returnVector(countOfIris);
    float *PetalWidthCmVector = returnVector(countOfIris);

    // assign values to vectors
    for (int i = 0; i < countOfIris; i++) {
        SepalLengthCmVector[i] = IrisDataMatrix[i][0];
        SepalWidthCmVector[i] = IrisDataMatrix[i][1];
        PetalLengthCmVector[i] = IrisDataMatrix[i][2];
        PetalWidthCmVector[i] = IrisDataMatrix[i][3];
    }

    // calculate column variances
    float SepalLengthCmVariance = calculateVariance(SepalLengthCmVector, countOfIris);
    float SepalWidthCmVariance = calculateVariance(SepalWidthCmVector, countOfIris);
    float PetalLengthCmVariance = calculateVariance(PetalLengthCmVector, countOfIris);
    float PetalWidthCmVariance = calculateVariance(PetalWidthCmVector, countOfIris);

    // write variances to file
    char varianceString[2048];
    sprintf(varianceString,
            "SepalLengthCmVariance: %f\nSepalWidthCmVariance: %f\nPetalLengthCmVariance: %f\nPetalWidthCmVariance: %f\n",
            SepalLengthCmVariance, SepalWidthCmVariance, PetalLengthCmVariance,
            PetalWidthCmVariance);
    writeStringToFile(statsPtr, varianceString);

    // calculate column means
    float *means = returnVector(4);
    for (int i = 0; i < 4; ++i) {
        means[i] = columnMeans(IrisDataMatrix, countOfIris, 4)[i];
    }

    // each column mean
    float meanSepalLengthCm = means[0];
    float meanSepalWidthCm = means[1];
    float meanPetalLengthCm = means[2];
    float meanPetalWidthCm = means[3];

    // write means to file
    char meansString[1024];

    // write means to file. create new string for each mean with sprintf
    sprintf(meansString, "Means of SepalLengthCm = %f", meanSepalLengthCm);
    writeStringToFile(statsPtr, meansString);

    sprintf(meansString, "Means of SepalWidthCm = %f", meanSepalWidthCm);
    writeStringToFile(statsPtr, meansString);

    sprintf(meansString, "Means of PetalLengthCm = %f", meanPetalLengthCm);
    writeStringToFile(statsPtr, meansString);

    sprintf(meansString, "Means of PetalWidthCm = %f", meanPetalWidthCm);
    writeStringToFile(statsPtr, meansString);

    // calculate correlation of 3 different pairs
    float corre1 = correlation(SepalLengthCmVector, SepalWidthCmVector, countOfIris);
    float corre2 = correlation(PetalLengthCmVector, PetalWidthCmVector, countOfIris);
    float corre3 = correlation(SepalLengthCmVector, PetalWidthCmVector, countOfIris);

    // write correlations to file
    char correString[1024];

    // write correlations to file
    sprintf(correString, "\nCorrelation of SepalLengthCm and SepalWidthCm = %f", corre1);
    writeStringToFile(statsPtr, correString);

    sprintf(correString, "Correlation of PetalLengthCm and PetalWidthCm = %f", corre2);
    writeStringToFile(statsPtr, correString);

    sprintf(correString, "Correlation of SepalLengthCm and PetalWidthCm = %f", corre3);
    writeStringToFile(statsPtr, correString);

    // calculate covariance matrix of IrisDataMatrix
    float **covarianceMat = covarianceMatrix(IrisDataMatrix, countOfIris, 4);

    // write covariance matrix to file
    char covMatLine[1024];
    sprintf(covMatLine, "\nCovariance Matrix of IrisDataMatrix:");
    writeStringToFile(statsPtr, covMatLine);
    writeMatrixToFile(statsPtr, covarianceMat, 4, 4);

    return 0;
}

void writeMatrixToFile(FILE *pFile, float **mat, int row, int col) { // write matrix to file
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            fprintf(pFile, "%f ", mat[i][j]);
        }
        fprintf(pFile, "\n");
    }
}

void writeStringToFile(FILE *fp, char *str) { // write string to file
    fprintf(fp, "%s\n", str);
}

void strtrim(char *str) { // trim string
    int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace((unsigned char) str[begin])) // if current char is space, increment begin
        begin++;

    while ((end >= begin) &&
           isspace((unsigned char) str[end])) // if current char is space and end is greater than begin, decrement end
        end--;

    // Shift all characters back to the start of the string array.
    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0'; // Null terminate string.
}
