/*
Creating two child process from 1 parent
Both child processes will loop for some random number of iterations but no more than 30x.
Parent process will wait for both child processes to finish

Inside the loop:
Each child should print the message "Child PId:<pid> is going to sleep" then call the sleep() method
Sleeps for a random number of seconds but no more than 10 second
When child awakes it should print the message  “Child Pid: is awake!\nWhere is my Parent: <ppid>?”.
use getpid() to obtain the <pip> of the running process
use getppid() to obtain the <ppid> of the parent
After the loop is over, each child should print the message "Child PId:<pid> is exiting" and call exist method


For the Parent Process:
parent process must call the wait function to wait for the child processes to finish
parent process should print the message "Child Pid: <pid> has completed" using the wait(&status)

*/

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int main() {
    srand(time(NULL)); // seed random number generator
    pid_t child1, child2;
    int status;

    child1 = fork(); //creates the process and stores PID in child1

    if (child1 == -1) {
        perror("Fork process failed");
        return -1;
    }
    else if (child1 == 0){
        //Entering the child 1 process and executing the code below
        int random_iterations = rand() % 31;
        int i, sleep_time;
        for (i = 0; i < random_iterations; i++) {
            printf("Child PID:%d is going to sleep!\n", getpid());
            sleep_time = rand() % 11; //sleep time is between 0 and 10
            sleep(sleep_time);
            printf("Child PId:%d is awake!\nWhere is my Parent: %d\n", getpid(), getppid());
        }

        printf("Child PId:%d has completed and is now exiting\n", getpid());
        exit(0);
    }
    else {
        //Entering the parent process and executing the code below
        child2 = fork(); //creates the process and stores PID in child2

        if (child2 == -1) {
            perror("Fork process failed");
            return -1;
        }
        else if (child2 == 0) {
            //Entering the child 2 process and executing the code below
            int random_iterations = rand() % 31;
            int i, sleep_time;
            for (i = 0; i < random_iterations; i++) {
                printf("Child PID:%d is going to sleep!\n", getpid());
                sleep_time = rand() % 11; //sleep time is between 0 and 10
                sleep(sleep_time);
                printf("Child PId:%d is awake!\nWhere is my Parent: %d\n", getpid(), getppid());
            }

            printf("Child PId:%d has completed and is now exiting\n", getpid());
            exit(0);
        }
        else {
            //Entering the parent process and executing the code below
            wait(&status);
            printf("Child PId:%d has completed\n", child1);
            wait(&status);
            printf("Child PId:%d has completed\n", child2);
        }
    }
    
    return 0;
}
