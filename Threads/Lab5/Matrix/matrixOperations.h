#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Defining variables for matrix operations
extern int *matA, *matB;
extern int *matSumResult, *matDiffResult, *matProductResult;

// Defining a structure to be used with threads
typedef struct {
    int startRow;
    int endRow;
    int MAX; // Including MAX in the ThreadArg structure
} ThreadArg;

/* Declaring Matrix Operation functions */
int* allocateMatrix(int MAX);
void initializeMatrices(int MAX);
void freeGlobalMatrices();
void fillMatrix(int *matrix, int MAX);
void printMatrix(int *matrix, int MAX);
void performOperation(void *(*operation)(void *), int MAX);
void *computeSum(void *arg);
void *computeDiff(void *arg);
void *computeProduct(void *arg);

#endif // MATRIX_OPERATIONS_H
