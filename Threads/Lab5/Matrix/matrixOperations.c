#include "matrixOperations.h"
#include <math.h>
// #include <stdio.h>
// #include <stdlib.h>

int *matA, *matB;
int *matSumResult, *matDiffResult, *matProductResult;

// Define a maximum number of threads
#define MAX_THREADS 16

// Function to allocate memory for matrices in contiguous memory
int* allocateMatrix(int MAX) {
    int *matrix = (int *)malloc(MAX * MAX * sizeof(int));
    if (!matrix) {
        fprintf(stderr, "Failed to allocate memory for a matrix of size %d x %d.\n", MAX, MAX);
        exit(1);
    }
    return matrix;
}

// Function to initialize global matrices
void initializeMatrices(int MAX) {
    matA = allocateMatrix(MAX);
    matB = allocateMatrix(MAX);
    matSumResult = allocateMatrix(MAX);
    matDiffResult = allocateMatrix(MAX);
    matProductResult = allocateMatrix(MAX);
}

// Function to free global matrices
void freeGlobalMatrices() {
    free(matA);
    free(matB);
    free(matSumResult);
    free(matDiffResult);
    free(matProductResult);
}

// Function to fill a matrix with random numbers
void fillMatrix(int *matrix, int MAX) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matrix[i * MAX + j] = rand() % 10 + 1;
        }
    }
}

// Function to print a matrix
void printMatrix(int *matrix, int MAX) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i * MAX + j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to perform operations using pthreads
void performOperation(void* (*operation)(void *), int MAX){
    // Calculate the number of threads dynamically
    int numThreads = ceil((double)MAX / 10); // One thread per 10 rows as an example
    numThreads = numThreads > MAX_THREADS ? MAX_THREADS : numThreads; // Limiting to MAX_THREADS

    pthread_t threads[numThreads];
    int rowsPerThread = MAX / numThreads;
    int remainingRows = MAX % numThreads;

    for (int i = 0; i < numThreads; i++) {
        ThreadArg *arg = malloc(sizeof(ThreadArg));
        if (arg == NULL) {
            fprintf(stderr, "Failed to allocate memory for thread arguments.\n");
            freeGlobalMatrices();
            exit(1);
        }

        arg->startRow = i * rowsPerThread;
        arg->endRow = arg->startRow + rowsPerThread;
        arg->MAX = MAX;

        // Distribute remaining rows among the threads, if any
        if (i == numThreads - 1) {
            arg->endRow += remainingRows;
        }

        // Create threads to perform the operation
        if (pthread_create(&threads[i], NULL, operation, arg) != 0) {
            fprintf(stderr, "Failed to create thread.\n");
            free(arg);
            freeGlobalMatrices();
            exit(1);
        }
    }

    // Join threads after completion
    for (int i = 0; i < numThreads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Failed to join thread.\n");
            freeGlobalMatrices();
            exit(1);
        }
    }
}

// Function to compute the sum of two matrices
void *computeSum(void *arg) {
    ThreadArg *args = (ThreadArg *)arg;
    int MAX = args->MAX;
    for (int i = args->startRow; i < args->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matSumResult[i * MAX + j] = matA[i * MAX + j] + matB[i * MAX + j];
        }
    }
    free(arg);
    return NULL;
}

// Function to compute the difference of two matrices
void *computeDiff(void *arg) {
    ThreadArg *args = (ThreadArg *)arg;
    int MAX = args->MAX;
    for (int i = args->startRow; i < args->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matDiffResult[i * MAX + j] = matA[i * MAX + j] - matB[i * MAX + j];
        }
    }
    free(arg);
    return NULL;
}

// Function to compute the product of two matrices
void *computeProduct(void *arg) {
    ThreadArg *args = (ThreadArg *)arg;
    int MAX = args->MAX;
    for (int i = args->startRow; i < args->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            int sum = 0;
            for (int k = 0; k < MAX; k++) {
                sum += matA[i * MAX + k] * matB[k * MAX + j];
            }
            matProductResult[i * MAX + j] = sum;
        }
    }
    free(arg);
    return NULL;
}
