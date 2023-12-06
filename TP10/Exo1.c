#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int pipefd[2];
    pid_t pid1, pid2;

    // Création du tube
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Création du premier fils (F1)
    if ((pid1 = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) { // F1
        close(pipefd[0]); // Ferme lecture du tube non utilisée

        // Redirection de la sortie standard de F1 vers l'entrée du tube
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]); // Ferme sortie du tube déjà redirigée

        // Exécution de la commande ps -aj
        execlp("ps", "ps", "-aj", NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else { // Le père
        // Création du deuxième fils (F2)
        if ((pid2 = fork()) == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid2 == 0) { // F2
            close(pipefd[1]); // Ferme écriture du tube non utilisée

            // Redirection de l'entrée standard de F2 depuis la sortie du tube
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]); // Ferme entrée du tube déjà redirigée

            // Exécution de la commande tr "[:upper:][:lower:]" "[:lower:][:upper:]"
            execlp("tr", "tr", "[:upper:][:lower:]", "[:lower:][:upper:]", NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        } else { // Le père
            close(pipefd[0]); // Ferme les extrémités non utilisées du tube
            close(pipefd[1]);

            // Attend la terminaison des fils
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);

            // Affichage des messages demandés
            printf("pid0 : ps -aj | tr \"[:upper:][:lower:]" "[:lower:][:upper:]\n");
            printf("pid1 : ps -aj\n");
            printf("pid2 : tr \"[:upper:][:lower:]" "[:lower:][:upper:]\n");
        }
    }
    return 0;
}
