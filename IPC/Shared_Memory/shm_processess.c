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

#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

//Structure for shared memory
typedef struct {
    int BankAccount;
    int Turn;
} shared_mem;

void depositMoney(shared_mem *shared, int balance) {
    if (balance % 2 == 0) {
    shared->BankAccount += balance;
    printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, shared->BankAccount);
    } 
    else {
        printf("Dear old Dad: Doesn't have any money to give\n");
    }
    shared->Turn = 1;
}

void withdrawMoney (shared_mem *shared, int balance) {
    if (balance <= shared->BankAccount) {
        shared->BankAccount -= balance;
        printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, shared->BankAccount);
    }
    else {
        printf("Poor Student: Not Enough Cash ($%d)\n", shared->BankAccount);
    }
    shared->Turn = 0;
}

int main() {
    //defining variables for shared memory
    int shmid_bank, shmid_turn;
    key_t key;
    shared_mem *shared;

    //TODO(DevKingB): Implement shared memory segmentation


    shared = (shared_mem *) shmat(shmid, NULL, 0);
    shared->BankAccount = 0;
    shared->Turn = 0;
    
    //Create child process
    pid_t pid;
    pid = fork();
    
    //If child process
    if (pid == 0) {
        int i;
        for (i = 0; i < 25; i++) {
            sleep(rand() % 6);
            int balance = rand() % 51;
            int account = shared->BankAccount;
            printf("Poor Student needs $%d\n", balance);
            while (shared->Turn != 1) {
                //Do nothing
            }
            withdrawMoney(shared, balance);
        }
    }
    //If parent process(aka Dear old Dad)
    else {
        int i;
        for (i = 0; i < 25; i++) {
            sleep(rand() % 6);
            int balance = rand() % 101;
            int account = shared->BankAccount;
            while (shared->Turn != 0) {
                //Do nothing
            }
            if (shared->BankAccount <= 100) {
                depositMoney(shared, balance);
            }
            else {
                printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
            }
        }
        wait(NULL);
        shmdt(shared);
        shmctl(shmid_bank, IPC_RMID, NULL);
        shmctl(shmid_turn, IPC_RMID, NULL);
    }
    return 0;
}

