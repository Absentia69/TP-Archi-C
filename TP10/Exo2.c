#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct Data {
    int length;
    int sequence[32];
};

int main() {
    int pipe_pere_fils[2];
    int pipe_fils_pere[2];
    pid_t pid;

    // Création des tubes
    if (pipe(pipe_pere_fils) == -1 || pipe(pipe_fils_pere) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Création du fils
    if ((pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Le fils
        struct Data data;

        close(pipe_pere_fils[1]); // Ferme écriture père->fils non utilisée
        close(pipe_fils_pere[0]); // Ferme lecture fils->père non utilisée

        // Lecture des données envoyées par le père
        read(pipe_pere_fils[0], &data, sizeof(struct Data));
        close(pipe_pere_fils[0]); // Ferme lecture après la lecture des données

        // Calcul du max, min, moyenne
        int max = data.sequence[0], min = data.sequence[0], sum = 0;
        for (int i = 0; i < data.length; ++i) {
            if (data.sequence[i] > max) max = data.sequence[i];
            if (data.sequence[i] < min) min = data.sequence[i];
            sum += data.sequence[i];
        }
        double moyenne = (double)sum / data.length;

        // Envoi des résultats au père
        int results[3] = {max, min, (int)moyenne};
        write(pipe_fils_pere[1], results, sizeof(results));
        close(pipe_fils_pere[1]); // Ferme écriture après envoi

        exit(EXIT_SUCCESS);
    } else { // Le père
        struct Data data;
        int results[3];

        printf("Entrez la longueur de la séquence (<= 32) : ");
        scanf("%d", &data.length);

        printf("Entrez la séquence d'entiers : ");
        for (int i = 0; i < data.length; ++i) {
            scanf("%d", &data.sequence[i]);
        }

        close(pipe_pere_fils[0]); // Ferme lecture père->fils non utilisée
        close(pipe_fils_pere[1]); // Ferme écriture fils->père non utilisée

        // Envoi des données au fils
        write(pipe_pere_fils[1], &data, sizeof(struct Data));
        close(pipe_pere_fils[1]); // Ferme écriture après envoi

        // Lecture des résultats envoyés par le fils
        read(pipe_fils_pere[0], results, sizeof(results));
        close(pipe_fils_pere[0]); // Ferme lecture après la lecture des résultats

        // Affichage des résultats
        printf("Max: %d\nMin: %d\nMoyenne: %d\n", results[0], results[1], results[2]);

        wait(NULL); // Attente de la fin du fils
    }

    return 0;
}
