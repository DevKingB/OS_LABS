/*
Copy your current signals.c to a new program timer.c such that after exiting (via CTRL-C), will print out the total time the program was executing in seconds. 
To accomplish this task, you will need to register a second signal handler for the SIGINT signal, the signal that is delivered when CTRL-C is pressed. 
Conceptually, your program will request a SIGALRM signal to occur every second, 
tracking the number of alarms delivered, and when the program exits via CTRL-C, 
it will print how many alarms occurred, and the number of seconds it was executed.

*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

int alarm_signal_handled = 0; //global variable to handle signal change
int kill_signal_handled = 0; //global variable to handle signal change
int alarm_count = 0; //global variable to count the number of alarms
// clock_t start_time; //global variable to store the start time
// clock_t end_time; //global variable to store the end time
struct timeval start_time, end_time; //global variable to store the start time

void alarm_handler(int signum)
{ //alarm signal handler
  printf("Hello World!\n"); // prints the message here
  alarm_signal_handled = 1; //change value of signal_handled
  //exit(1); //exit after printing
}

void kill_handler(int signum)
{ //kill signal handler
  gettimeofday(&end_time, NULL); //end the clock
  double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + ((end_time.tv_usec - start_time.tv_usec)/1000000.0); //calculate the elapsed time
  printf("Total time: %.2f seconds\n", elapsed_time); // prints the message here
  printf("Number of alarms: %d\n", alarm_count); // prints the message here
  kill_signal_handled = 1; //change value of signal_handled
  exit(0); //exit after printing
}

int main(int argc, char * argv[])
{
  signal(SIGALRM, alarm_handler); //register handler to handle SIGALRM
  signal(SIGINT, kill_handler); //register handler to handle SIGINT
  gettimeofday(&start_time, NULL); //start the clock
  while(1)
  {
    alarm(1); //Schedule a SIGALRM for 1 second
    while(!alarm_signal_handled); //busy wait for signal to be delivered
    printf("Turing was right!\n"); //prints the message here
    alarm_signal_handled = 0; //reset the flag
    alarm_count++; //increment the alarm count
  }
//   end_time = clock(); //end the clock
  return 0; //never reached
}
