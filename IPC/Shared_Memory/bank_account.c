/*
TASK:
You are to write a program that creates two shared variables of type int. One integer is the BankAccount, another is Turn. 
Both BankAccount and Turn are initialized to 0. 
Your program must create two processes, one parent process and one child process, and allow the parent to deposit (i.e. add) money to the BankAccount, 
and also allow the child process to withdraw (i.e. subtract) money from the BankAccount using Tanenbaum’s strict alternation (see OS textbook Chap 4). 
Both the Parent and Child should loop 25 times and follow the rules below each time through the loop

DADS RULES:
1. Sleep some random amount of time between 0 - 5 seconds
2. After waking up, copy the value in BankAccount to a local variable account
3. loop while Turn != 0 do no-op; //!NEED TO FIGURE WHAT THIS MEANS
4. if the account is <= 100, then try to Deposit Money, else call printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);

DEPOSTING RULES(DAD):
1. Randomly generate a balance amount to give the Student between 0 - 100
2. If the random number is even: Deposit the amount into the account, then call printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, account);
3. If the random number is odd: Then call printf("Dear old Dad: Doesn't have any money to give\n");
4. Copy value from local variable account back to BankAccount.
5. Set Turn = 1

CHILD RULES:
1. Sleep some random amount of time between 0 - 5 seconds
2. After waking up, copy the value in BankAccount to a local variable account
3. loop while Turn != 1 do no-op; //!NEED TO FIGURE WHAT THIS MEANS
4. Randomly generate a balance amount that the Student needs between 0-50, then call printf("Poor Student needs $%d\n", balance);
5. If balance is less than or equal to account, then withdraw balance from the account, then call printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, account);
6. If balance is greater than account, then call printf("Poor Student: Not Enough Cash ($%d)\n", account);
7. Copy value from local variable account back to BankAccount.
8. Set Turn = 0

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// Define a key for our shared memory segment
#define SHMKEY ((key_t) 1497)

// Shared memory data structure
typedef struct {
    int BankAccount;
    int Turn;
} shared_mem;

// Function to deposit money
void depositMoney(shared_mem *shared, int balance) {
    if (balance % 2 == 0) {
        shared->BankAccount += balance;
        printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, shared->BankAccount);
    } else {
        printf("Dear old Dad: Doesn't have any money to give\n");
    }
}

// Function to withdraw money
void withdrawMoney(shared_mem *shared, int balance) {
    if (balance <= shared->BankAccount) {
        shared->BankAccount -= balance;
        printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, shared->BankAccount);
    } else {
        printf("Poor Student: Not Enough Cash ($%d)\n", shared->BankAccount);
    }
}


int main() {
    // Initialize random number generator
    srand(time(NULL));

    int shmid;
    shared_mem *shared;

    // Create shared memory segment
    shmid = shmget(SHMKEY, sizeof(shared_mem), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment
    shared = (shared_mem *)shmat(shmid, NULL, 0);
    shared->BankAccount = 0;
    shared->Turn = 0;

    // Fork to create child process
    pid_t pid = fork();

    // If child process
    // Child process loop
    if (pid == 0) {
        for (int i = 0; i < 25; i++) {
            sleep(rand() % 6);
            while (shared->Turn != 1) {
                    // Busy waiting (No Operation)
            }
            int balance = rand() % 51;
            printf("Poor Student needs $%d\n", balance);
            withdrawMoney(shared, balance);
            shared->Turn = 0; // Explicitly give the turn to the parent
        }
    } 
    // Parent process loop
    else {
        for (int i = 0; i < 25; i++) {
            sleep(rand() % 6);
            while (shared->Turn != 0) {
                    // Busy waiting (No Operation)
            }
            int balance = rand() % 101;
            int account = shared->BankAccount; // Copy the value to local variable
            if (account <= 100) {
                    depositMoney(shared, balance);
            } else {
                    printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
            }
            shared->Turn = 1; // Explicitly give the turn to the child
        }
        wait(NULL);
        // Detach and remove the shared memory segment
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
    }
    return 0;
}
