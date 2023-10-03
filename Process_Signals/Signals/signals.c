/**
 * *Implement a program that will register signal handler functions
 * test signal handlers by issuing a signal from the command line (i.e. Ctrl^C) SIGINT signal.
 * we will investigate a different class of signals whose purpose is to not terminate a program, but rather instruct the program to take an action.
 * you will employ the SIGALRM signal to take periodic actions, such as printing a message to the screen.
 * The signal() system call instructs the operating system to execute the function handler when the SIGALRM signal is delivered.
*/

/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int signal_handled = 0; //global variable to handle signal change

void handler(int signum)
{ //signal handler
  printf("Hello World!\n"); // prints the message here
  signal_handled = 1; //change value of signal_handled
  //exit(1); //exit after printing
}

int main(int argc, char * argv[])
{
  signal(SIGALRM, handler); //register handler to handle SIGALRM
  while(1)
  {
    alarm(1); //Schedule a SIGALRM for 1 second
    while(!signal_handled); //busy wait for signal to be delivered
    printf("Turing was right!\n"); //prints the message here
    signal_handled = 0; //reset the flag
  }
  return 0; //never reached
}