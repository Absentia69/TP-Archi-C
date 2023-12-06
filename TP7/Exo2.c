#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 10240 // 10k octets

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int file = open(argv[1], O_RDONLY);
    if (file == -1) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    off_t fileSize = lseek(file, 0, SEEK_END);
    if (fileSize == -1) {
        perror("Error seeking file");
        close(file);
        return EXIT_FAILURE;
    }

    off_t numChunks = fileSize / BUFFER_SIZE;
    off_t remainingBytes = fileSize % BUFFER_SIZE;

    lseek(file, 0, SEEK_SET);

    unsigned char buffer[BUFFER_SIZE];

    for (off_t i = 1; i <= numChunks; i++) {
        ssize_t bytesRead = read(file, buffer, BUFFER_SIZE);
        if (bytesRead != BUFFER_SIZE) {
            perror("Error reading file");
            close(file);
            return EXIT_FAILURE;
        }

        char outputFileName[50];
        sprintf(outputFileName, "%s.%ld", argv[1], i);

        int output = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output == -1) {
            perror("Error creating output file");
            close(file);
            return EXIT_FAILURE;
        }

        if (write(output, buffer, BUFFER_SIZE) != BUFFER_SIZE) {
            perror("Error writing to output file");
            close(file);
            close(output);
            return EXIT_FAILURE;
        }

        close(output);
    }


if (remainingBytes > 0) {
    ssize_t bytesRead = read(file, buffer, remainingBytes);
    if (bytesRead != remainingBytes) {
        perror("Error reading remaining bytes");
        close(file);
        return EXIT_FAILURE;
    }

    char outputFileName[50]; // Adjust size as needed
    sprintf(outputFileName, "%s.%ld", argv[1], numChunks + 1);

    int output = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output == -1) {
        perror("Error creating output file");
        close(file);
        return EXIT_FAILURE;
    }

    if (write(output, buffer, remainingBytes) != remainingBytes) {
        perror("Error writing remaining bytes to output file");
        close(file);
        close(output);
        return EXIT_FAILURE;
    }

    close(output);
}


    close(file);
    return EXIT_SUCCESS;
}
