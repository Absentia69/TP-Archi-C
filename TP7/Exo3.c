#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

void copyFile(int input, int output) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(input, buffer, BUFFER_SIZE)) > 0) {
        if (write(output, buffer, bytesRead) != bytesRead) {
            perror("Error writing to output");
            exit(EXIT_FAILURE);
        }
    }

    if (bytesRead == -1) {
        perror("Error reading from input");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) { // No files specified, copy from stdin to stdout
        copyFile(STDIN_FILENO, STDOUT_FILENO);
    } else {
        for (int i = 1; i < argc; i++) {
            int file = open(argv[i], O_RDONLY);
            if (file == -1) {
                perror("Error opening file");
                return EXIT_FAILURE;
            }

            copyFile(file, STDOUT_FILENO);
            close(file);
        }
    }

    return EXIT_SUCCESS;
}
