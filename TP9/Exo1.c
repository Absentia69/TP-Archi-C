#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "La création du processus fils a échoué\n");
        return 1;
    } else if (pid == 0) { // Processus fils
        printf("C'est le fils qui parle\n");
        printf("Mon pid est %d\n", getpid());
        printf("Le pid de mon père est %d\n", getppid());
    } else { // Processus père
        wait(NULL); // Attente de la fin du fils
        printf("C'est le père qui parle\n");
        printf("Le pid de mon fils est %d\n", pid);
        printf("Mon pid est %d\n", getpid());
        printf("Le pid du grand-père de mon fils, donc mon père, est %d\n", getppid());
    }

    return 0;
}
