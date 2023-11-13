#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

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
int sortBy = 0; // 0 for arrival time, 1 for priority
 
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
    return 0;
}