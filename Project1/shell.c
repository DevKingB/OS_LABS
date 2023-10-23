#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[] = "> ";
char delimiters[] = " =\t\r\n";
extern char **environ;
bool background = false;
int chld_term = 0;
int timer_set = 0;

/**
 * * Add set timer function for the alarm
 * @param int: number of seconds to set the timer for
*/
void set_timer(int seconds=10) {
    timer_set = 1;
    alarm(seconds);
}

/**
 * * Add cancel timer function for the alarm
 * @param void
*/
void cancel_timer(){
    timer_set = 0;
    alarm(0);
}

void tokenize(char *command_line, char **args) {
    int i = 0;
    char *token = strtok(buf, delimiters);
    while (token != NULL) {
        if (token[0] == '$') {
            char *env = getenv(token + 1);
            if (env != NULL) {
                args[i++] = var_value;
            }
            else {
                args[i++] = ""; //empty string if not found
            }
        }
        else {
            args[i++] = token;
        }
        token = strtok(NULL, delimiters);
    }
    args[i] = NULL; //null terminate the array
}

void handle_SIGINT(int signum) {
    if (chld_term == 0) {
        signal(SIGINT, handle_SIGINT);
        printf("\n");
        char current_dir[MAX_COMMAND_LINE_LEN]; //setting the array to size 1024
        getcwd(current_dir, sizeof(current_dir)) != NULL? printf("%s> ", current_dir): fprintf(stderr, "getcwd error");printf("> ");
        fflush(stdout);
        fflush(stderr);
    }
}

void handle_SIGALARM(int signum){
    if (timer_set) {
        printf("\nTimer has expired. Terminating process. \n");
        kill(0, SIGINT);
    }
}

void handle_SIGCHLD(int signum){
    int status;
    pid_t pid;

    pid = waitpid(-1, &status, WNOHANG);
    while pid > 0 {
        if (status == 0){
            printf("Child process %d has exited with status %d\n", pid, status);
        }
        else if  (status > 1){
            printf("Child process %d has exited by command\n", pid);
        }
    }
    chld_term = status;
}


