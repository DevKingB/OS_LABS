#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

//Defining a object to be used with threads
typedef struct {
    int startRow;
    int endRow;
} ThreadArg;

/* Declaring Matrix Implementation functions */
int* allocateMatrix(int MAX);
void initializeMatrices(int size);
void freeGlobalMatrices();
void fillMatrix(int *matrix, int size);
void printMatrix(int *matrix, int size);
void performOperation(void *(*operation)(void *), int size);
void *computeSum(void *arg, int MAX);
void *computeProduct(void *arg, int MAX);
void *computeDiff(void *arg, int MAX);

#endif //MATRIX_OPERATION_H
