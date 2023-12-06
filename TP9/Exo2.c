#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char path[100];

    while (1) {
        printf("Entrez un chemin : ");
        scanf("%s", path);

        pid_t pid = fork();

        if (pid < 0) {
            fprintf(stderr, "La création du processus fils a échoué\n");
            return 1;
        } else if (pid == 0) { // Processus fils
            execl("/bin/ls", "ls", "-l", path, NULL);
            exit(0);
        } else { // Processus père
            int status;
            waitpid(pid, &status, 0); // Attente de la fin du fils
        }
    }

    return 0;
}
