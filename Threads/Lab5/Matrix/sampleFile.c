#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Global variables for matrices and their size
int *matA, *matB; 
int *matSumResult, *matDiffResult, *matProductResult; 
int MAX;

// Function to allocate memory for a matrix in contiguous memory
int* allocateMatrix() {
    int *matrix = (int *)malloc(MAX * MAX * sizeof(int));
    if (!matrix) {
        fprintf(stderr, "Failed to allocate memory for a matrix.\n");
        exit(1);
    }
    return matrix;
}

// Function to initialize global matrices
void initializeMatrices() {
    matA = allocateMatrix();
    matB = allocateMatrix();
    matSumResult = allocateMatrix();
    matDiffResult = allocateMatrix();
    matProductResult = allocateMatrix();
}

// Function to free a matrix
void freeMatrix(int *matrix) {
    free(matrix);
}

// Function to free global matrices
void freeGlobalMatrices() {
    freeMatrix(matA);
    freeMatrix(matB);
    freeMatrix(matSumResult);
    freeMatrix(matDiffResult);
    freeMatrix(matProductResult);
}

// Function to fill a matrix with random numbers
void fillMatrix(int *matrix) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matrix[i * MAX + j] = rand() % 10 + 1;
        }
    }
}

// Function to print a matrix
void printMatrix(int *matrix) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i * MAX + j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Structure to hold thread arguments
typedef struct {
    int startRow;
    int endRow;
} ThreadArg;

// Thread function for computing the sum of two matrices
void* computeSum(void* arg) {
    ThreadArg *args = (ThreadArg *)arg;
    for (int i = args->startRow; i < args->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matSumResult[i * MAX + j] = matA[i * MAX + j] + matB[i * MAX + j];
        }
    }
    free(arg);
    return NULL;
}

// Thread function for computing the difference of two matrices
void* computeDiff(void* arg) {
    ThreadArg *args = (ThreadArg *)arg;
    for (int i = args->startRow; i < args->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matDiffResult[i * MAX + j] = matA[i * MAX + j] - matB[i * MAX + j];
        }
    }
    free(arg);
    return NULL;
}

// Thread function for computing the product of two matrices
void* computeProduct(void* arg) {
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

// Function to create and manage threads for a matrix operation
void performOperation(void* (*operation)(void*)) {
    pthread_t threads[MAX];
    for (int i = 0; i < MAX; i++) {
        ThreadArg *arg = malloc(sizeof(ThreadArg));
        arg->startRow = i;
        arg->endRow = i + 1;
        if (pthread_create(&threads[i], NULL, operation, arg) != 0) {
            fprintf(stderr, "Failed to create thread.\n");
            freeGlobalMatrices();
            exit(1);
        }
    }

    for (int i = 0; i < MAX; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Failed to join thread.\n");
            freeGlobalMatrices();
            exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <matrix size>\n", argv[0]);
        return 1;
    }

    MAX = atoi(argv[1]);
    if (MAX <= 0) {
        fprintf(stderr, "Matrix size must be a positive integer\n");
        return 1;
    }

    srand(time(0)); // Seed for random number generation

    initializeMatrices(); // Initializing matrices
    fillMatrix(matA); // Filling matrices with random numbers
    fillMatrix(matB);

    printf("Matrix A:\n"); // Printing matrices
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    // Performing matrix operations using threads
    performOperation(computeSum);
    performOperation(computeDiff);
    performOperation(computeProduct);

    printf("Results:\n"); // Printing results
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);

    freeGlobalMatrices(); // Freeing allocated memory
    return 0;
}
