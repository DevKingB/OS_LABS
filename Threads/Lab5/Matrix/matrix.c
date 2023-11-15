#include "matrixOperations.h"

int main(int argc, char *argv[]){
    /* Error handling to check if matrix size is provided as a command line argument */
    printf("Running the program...\n");
    if (argc != 2) {
        fprintf(stderr,"Usage: %s <matrix size>\n", argv[0]);
        return 1;
    }

    // Convert the argument(string) to an integer to get the size of the matrix
    int MAX = atoi(argv[1]);
    if (MAX <= 0) {
        fprintf(stderr,"Error: matrix size must be a positive integer\n");
        return 1;
    }

    // Initializing the matrix(fill and printing)
    initializeMatrices(MAX);
    fillMatrix(matA, MAX);
    fillMatrix(matB, MAX);

    printf("Matrix A: \n");
    printMatrix(matA, MAX);
    printf("Matrix B: \n");
    printMatrix(matB, MAX);

    /* Performing the given operations on the initial matrices(matA, matB)*/
    printf("Performing Summation...\n");
    performOperation(computeSum, MAX);
    printf("Sum Matrix:\n");
    printMatrix(matSumResult, MAX);

    printf("Performing Difference...\n");
    performOperation(computeDiff, MAX);
    printf("Difference Matrix:\n");
    printMatrix(matDiffResult, MAX);

    printf("Performing Product...\n");
    performOperation(computeProduct, MAX);
    printf("Product Matrix:\n");
    printMatrix(matProductResult, MAX);

    // Free resources
    freeGlobalMatrices();

    return 0;

} 