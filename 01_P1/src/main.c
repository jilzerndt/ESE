#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    char message[] = "Hello, World!                           ";
    while (1) {
        printf("\r"); // Return to start of line
        // Print the message
        printf("%s", message);
        
        // shift the message by one character, wrapping around
        char first_char = message[0];
        memmove(message, message + 1, strlen(message) - 1);
        message[strlen(message) - 1] = first_char;

        fflush(stdout);
        usleep(100000); // 100 ms delay (100,000 microseconds)
    }
    return 0;
}