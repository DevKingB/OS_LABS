/**
 * ! This program executes the following shell command:
 * ! cat scores | grep argv[1] | sort
 * * This is a refactored version of the program in IPC/Pipes/pipes_processes2.c
 * I am assuming that when this is compiled with the make file and we insert just the number such as 28, 
 * it will perform the same function as the previously stated.
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <grep_argument>\n", argv[0]);
        return 1;
    }

    int pipe1[2], pipe2[2];
    int pid1, pid2;

    char *cat_args[] = {"cat", "scores", NULL};
    char *grep_args[] = {"grep", argv[1], NULL};
    char *sort_args[] = {"sort", NULL};

    // Create the first pipe
    if (pipe(pipe1) == -1) {
        perror("pipe");
        return 1;
    }

    // Create the second pipe
    if (pipe(pipe2) == -1) {
        perror("pipe");
        return 1;
    }

    pid1 = fork();
    if (pid1 == 0) {
        // Replace standard output with the write end of pipe1
        dup2(pipe1[1], STDOUT_FILENO);

        // Close the unused read end of pipe1
        close(pipe1[0]);

        // Close both ends of pipe2 as they are not used in this child
        close(pipe2[0]);
        close(pipe2[1]);

        // Execute "cat"
        execvp("cat", cat_args);
        perror("execvp cat"); // If execvp returns, it must have failed
        return 1;
    }

    pid2 = fork();
    if (pid2 == 0) {
        // Replace standard input with the read end of pipe1
        dup2(pipe1[0], STDIN_FILENO);

        // Replace standard output with the write end of pipe2
        dup2(pipe2[1], STDOUT_FILENO);

        // Close the unused write end of pipe1 and read end of pipe2
        close(pipe1[1]);
        close(pipe2[0]);

        // Execute "grep"
        execvp("grep", grep_args);
        perror("execvp grep"); // If execvp returns, it must have failed
        return 1;
    }

    // Close the unused ends of pipe1
    close(pipe1[0]);
    close(pipe1[1]);

    // At this point, we're in the parent again, which will handle "sort"
    // Replace standard input with the read end of pipe2
    dup2(pipe2[0], STDIN_FILENO);

    // Close the unused write end of pipe2
    close(pipe2[1]);

    // Execute "sort"
    execvp("sort", sort_args);
    perror("execvp sort"); // If execvp returns, it must have failed

    // Close the remaining file descriptors
    close(pipe2[0]);

    // Wait for all child processes to finish
    wait(NULL);
    wait(NULL);

    return 0;
}
