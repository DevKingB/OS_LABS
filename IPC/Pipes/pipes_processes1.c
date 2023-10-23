#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd1[2];  // Used to store two ends of the first pipe
    int fd2[2];  // Used to store two ends of the second pipe

    char fixed_str[] = "howard.edu";
    char input_str[100];
    pid_t p;

    if (pipe(fd1) == -1) {
        fprintf(stderr, "Pipe 1 Failed");
        return 1;
    }
    if (pipe(fd2) == -1) {
        fprintf(stderr, "Pipe 2 Failed");
        return 1;
    }

    printf("Enter a string to concatenate: ");
    scanf("%s", input_str);

    p = fork();

    if (p < 0) {
        fprintf(stderr, "fork Failed");
        return 1;
    } else if (p > 0) {
        close(fd1[0]);  // Close reading end of the first pipe
        close(fd2[1]);  // Close writing end of the second pipe

        // Write input string to P2 and close writing end of the first pipe.
        write(fd1[1], input_str, strlen(input_str) + 1);

        // Read the concatenated string from P2
        char concat_str[100];
        read(fd2[0], concat_str, 100);

        printf("Concatenated string: %s\n", concat_str);

        close(fd1[1]); // Close writing end of the first pipe
        close(fd2[0]); // Close reading end of the second pipe
    } else {
        close(fd1[1]); // Close writing end of the first pipe
        close(fd2[0]); // Close reading end of the second pipe

        // Read a string from P1 using the first pipe
        char received_str[100];
        read(fd1[0], received_str, 100);

        // Concatenate a fixed string with it
        int k = strlen(received_str);
        int i;
        for (i = 0; i < strlen(fixed_str); i++)
            received_str[k++] = fixed_str[i];

        received_str[k] = '\0'; // String ends with '\0'

        printf("Concatenated string: %s\n", received_str);

        // Prompt the user for a second input
        char second_input_str[100];
        printf("Enter a second string: ");
        scanf("%s", second_input_str);

        // Concatenate the second input with the result from P1
        k = strlen(received_str);
        for (i = 0; i < strlen(second_input_str); i++)
            received_str[k++] = second_input_str[i];

        received_str[k] = '\0';

        // Send the final concatenated string back to P1 using the second pipe
        write(fd2[1], received_str, strlen(received_str) + 1);

        close(fd1[0]); // Close reading end of the first pipe
        close(fd2[1]); // Close writing end of the second pipe

        exit(0);
    }
}
