// ! THIS FILE IS FOR TASK 1 IMPLEMENTATION //

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#include "../Headers/process.h"
#include "../Headers/util.h"

#define DEBUG 0	 //! change this to 1 to enable verbose output


/**
 * Signature for an function pointer that can compare
 * You need to cast the input into its actual 
 * type and then compare them according to your
 * custom logic
 */
typedef int (*Comparer) (const void *a, const void *b);
int sortBy = 0; //* Global variable to determine sorting criterion (0 for arrival time, 1 for priority)
 
/**
 * compares 2 processes
 * You can assume: 
 * - Process ids will be unique
 * - No 2 processes will have same arrival time
 */

int compareProcess(const void *this, const void *that) {
    Process *firstProcess = (Process *) this;
    Process *secondProcess = (Process *) that;

    if (sortBy) {
        //* Primary sorting by priority (descending order)
        if (firstProcess->priority != secondProcess->priority) {
            return secondProcess->priority - firstProcess->priority;
        }
        //* Secondary sorting by priority(arrival time)
        return firstProcess->arrival_time - secondProcess->arrival_time;
    }
    //* Primary sorting by arrival time(ascending order)
    else {
        return firstProcess->arrival_time - secondProcess->arrival_time;
    }
}


int main (int argc, char *argv[]) {
    // Ceheck for the correct number of arguments
    if ( argc < 3) {
        fprintf(stderr, "Usage: ./func-ptr <input-file-path> <sort-by>\n");
        fflush(stdout);
        return 1;
    }

    //Determinig sorting criterion
    if (strcmp(argv[2], "priority") == 0) {
        sortBy = 1;
    }
    else if (strcmp(argv[2], "arrival") != 0) {
        fprintf(stderr, "Error: Invalid sorting criterion. Use 'arrival' or 'priority'.\n");
        fflush(stdout);
        return 1;
    }

    /*******************/
	/* Parse the input */
	/*******************/
    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        fprintf(stderr, "Error: Cannot open file '%s': %s\n", argv[1], strerror(errno));
        fflush(stdout);
        return 1;
    }

    Process *processes = parse_file(inputFile); //parses the input file and returns an array of type Process

    /*******************/
	/* sort the input  */
	/*******************/

    Comparer processComparer = compareProcess; //function pointer to compareProcess function

    /**
     * Function belows allows for robust debugging for the file
    */
    #if DEBUG
	for (int i = 0; i < P_SIZE; i++) {
		    printf("%d (%d, %d) ",
				processes[i].pid,
				processes[i].priority, processes[i].arrival_time);
	}
	printf("\n");
    #endif

    qsort(processes, P_SIZE, sizeof(Process), processComparer); //sorts the processes array using the compareProcess function

    //* Printing the sorted data
    for (int i = 0; i < P_SIZE; i++) {
        printf("%d (%d, %d) \n", processes[i].pid, processes[i].priority, processes[i].arrival_time);
    }
    fflush(stdout);
    fflush(stderr);

    //! Cleaning Up
    free(processes);
    fclose(inputFile);
    return 0;
}