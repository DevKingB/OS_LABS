#include "matrixOperations.h"

static int *matA, *matB;
static int *matSumResult, *matDiffResult, *matProductResult;

// Function to allocate memory for matrices in contiguous memory
int* allocateMatrix(int MAX) {
    int *matrix = (int *)malloc(MAX * MAX * sizeof(int));
    if (!matrix) {
        fprintf(stderr, "Failed to allocate memory for a matrix.\n");
        exit(1);
    }
    return matrix;
}

// Function to initialize global matrices
void initializeMatrices(int MAX) {
    matA = allocateMatrices(MAX);
    matB = allocateMatrices(MAX);
    matSumResult = allocateMatrices(MAX);
    matDiffResult = allocateMatrices(MAX);
    matProductResult = allocateMatrices(MAX);
}

// Function to free global Matrices
void freeGlobalMatrices() {
    free(matA);
    free(matB);
    free(matSumResult);
    free(matDiffResult);
    free(matProductResult);
}

// Function to fill global Matrices with random numbers
void fillMatrix(int *matrix, int MAX) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matrix[i * MAX + j] = rand() % 10 + 1;
        }
    }
}

// Function to print global Matrices
void printMatrix(int *matrix, int MAX) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i * MAX + j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to perform operations
void performOperation(void* (*operation)(void *), int MAX){
    pthread_t threads[MAX];
    for (int i = 0; i < MAX; i++) {
        ThreadArg *arg = malloc(sizeof(ThreadArg));
        arg->startRow = i;
        arg->endRow = i + 1;
        if (pthread_create(&threads[i], NULL, operation, arg) != 0) {
            fprintf(stderr, "Failed to create thread.\n");
            freeGlobalMatrices();
            exit(1);
        };
    }

    for (int i = 0; i < MAX; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Failed to join thread.\n");
            freeGlobalMatrices();
            exit(1);
        }
    }
}

// Function to compute the sum of two matrices
void *computeSum(void *arg, int MAX) {
    ThreadArg *args = (ThreadArg *)arg;
    for (int i = args->startRow; i < args->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matSumResult[i * MAX + j] = matA[i * MAX + j] + matB[i * MAX + j];
        }
    }
    free(arg);
    return NULL;
}

// Function to computer the difference of two matrices
void* computeDiff(void *arg, int MAX){
    ThreadArg *args = (ThreadArg *)arg;
    for (int i = args->startRow; i < args->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matDiffResult[i * MAX + j] = matA[i * MAX + j] - matB[i * MAX + j];
        }
    }
    free(arg);
    return NULL;
}

// Function to compute the product of two matrices
void* computeProduct(void *arg, int MAX){
    ThreadArg *args = (ThreadArg *)arg;
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

