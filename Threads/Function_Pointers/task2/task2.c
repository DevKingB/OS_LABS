#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* IMPLEMENT ME: Declare and Implement functions here your functions here */
int add (int num1, int num2) {return num1 + num2 ;}
int multiply(int num1, int num2) {return num1 * num2 ;}
int divide(int num1, int num2) {return num2 != 0? num1 / num2 : 0 ;}
int subtract(int num1, int num2) {return num1 - num2 ;}
int exitProgram(int num1, int num2){printf("Exiting Program\n"); exit(0); return 0; /*Exits the program; Arbitrary return value.*/ }


int main (void)
{
	/* Initializing function pointer array here*/
    int (*funcPtrArr[5])(int, int) = {add, subtract, multiply, divide, exitProgram};
    int num1, num2;

    srand((unsigned int)time(NULL)); //Seeding the random number generator

    char input; //Stores the user input

    do {
        //* Creates two random integers
        num1 = rand() % 15;
        num2 = rand() % 15; //? Could modify this to make sure num2 is not 0 for division

        printf("Operand A : %d | Operand B : %d\n", num1, num2);
        printf("Specify the operation to perform (0 : add | 1 : subtract | 2 : Multiply | 3 : divide | 4 : exit):");
        scanf(" %c", &input); //! Space before %c is important. It consumes the newline character from the previous input

        //Convert the character to an integer and execute operations
        int functionIndx = input - '0';
        (functionIndx >= 0 && functionIndx <= 4) ? printf("Result = %d\n", funcPtrArr[functionIndx](num1, num2)) : printf("Invalid input. Please enter a number between 0 and 4.\n");
    } while (input != '4');

    return 0;
}
