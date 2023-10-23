//! This is the refactored version of the original pipes_processes2.c file

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <grep_argument>\n", argv[0]);
    return 1;
  }

  int pipe1[2];
  int pipe2[2];
  int pid1, pid2;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL};

  // Create the first pipe (pipe1)
  if (pipe(pipe1) == -1) {
    perror("pipe");
    return 1;
  }

  pid1 = fork();

  if (pid1 == 0) {
    // Child process P2: execute "grep" and connect it to the first pipe

    // Close the write end of pipe1
    close(pipe1[1]);

    // Replace standard input with the read end of pipe1
    dup2(pipe1[0], 0);

    // Execute "grep"
    execvp("grep", grep_args);
  } else {
    // Parent process
    // Create the second pipe (pipe2)
    if (pipe(pipe2) == -1) {
      perror("pipe");
      return 1;
    }

    pid2 = fork();

    if (pid2 == 0) {
      // Child process P3: execute "sort" and connect it to the second pipe

      // Close the read end of pipe1
      close(pipe1[0]);

      // Close the write end of pipe2
      close(pipe2[1]);

      // Replace standard input with the read end of pipe2
      dup2(pipe2[0], 0);

      // Execute "sort"
      execvp("sort", sort_args);
    } else {
      // Parent process (P1)
      // Close the read end of pipe1
      close(pipe1[0]);

      // Replace standard output with the write end of pipe1
      dup2(pipe1[1], 1);

      // Execute "cat"
      execvp("cat", cat_args);
    }
  }

  // Close the remaining file descriptors in the parent
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);

  // Wait for all child processes to finish
  wait(NULL);
  wait(NULL);
  wait(NULL);

  return 0;
}