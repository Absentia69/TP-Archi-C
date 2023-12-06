#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct rec_t {
    char name[20];
    char surname[20];
    int ine;
} rec_t;

int rec_count(int fd);
int rec_read(int fd, int n, rec_t *pr);
int rec_write(int fd, int n, rec_t *pr);
int rec_search(int fd, unsigned ine);

int main(int argc, char *argv[]) {
  int fd, go = 1;
  char fname[256];
  unsigned a;
  
  do {
    printf("Fichier des enregistrements [256 chars max] : ");
    scanf("%s", fname);

    if ((fd = open(fname, O_RDWR | O_CREAT, 0660)) == -1)
      printf("Impossible d'ouvrir le fichier %s\n", fname);
  } while (fd == -1);

  while(go) {
    printf(">>> Choisir une action\n");
    printf("  [ 0: exit | 1: read | 2: write | 3: count | 4:recherche ] : ");
    scanf("%u", &a);
    switch(a) {
    case 0: {
      go = 0;
      printf("Au revoir !\n\n");
      break;
    }
    case 1: {
      int n, nrec = rec_count(fd);
      rec_t rec;
      if (nrec == 0) {
	printf("Le fichier est vide.\n\n");
	break;
      }
      printf("Position de l'enregistrement à lire [0..%d] : ", nrec-1);
      scanf("%d", &n);
      if (n < 0 || n >= nrec) {
	printf("Position de l'enregistrement non valid.\n\n");
	break;
      }
      rec_read(fd, n, &rec);
      printf("%s %s %08d\n\n", rec.name, rec.surname, rec.ine);
      break;
    }
    case 2: {
      int n, nrec = rec_count(fd);
      rec_t rec;
      printf("Position de l'enregistrement à écrire [0..%d] : ", nrec);
      scanf("%d", &n);
      if (n < 0 || n > nrec) {
	printf("Position de l'enregistrement non valid.\n\n");
	break;
      }
      printf("Prénom : ");
      scanf("%s", rec.name);
      printf("Nom : ");
      scanf("%s", rec.surname);
      printf("INE : ");
      scanf("%d", &rec.ine);
      rec_write(fd, n, &rec);
      printf("%s %s %08d\n\n", rec.name, rec.surname, rec.ine);
      break;
    }
    case 3: {
      int nrec = rec_count(fd);
      printf("Le fichier contient %d enregistrements.\n\n", nrec);
      break;
    }
    case 4: {
            unsigned search_ine;
            printf("Valeur de l'INE à rechercher : ");
            scanf("%u", &search_ine);

            int position = rec_search(fd, search_ine);
            if (position == -1) {
                printf("Aucun enregistrement correspondant à l'INE recherché.\n\n");
            } else {
                printf("Enregistrement trouvé à la position : %d\n", position);
                rec_t found_record;
                rec_read(fd, position, &found_record);
                printf("%s %s %08d\n\n", found_record.name, found_record.surname, found_record.ine);
            }
            break;
        }
    default:
      printf("%d n'est une action connue.\n", a);
    }
  }
  
  return EXIT_SUCCESS;
}

/*
 * Définition des fonctions rec_count, rec_read et rec_write
 */

int rec_count(int fd) {
    off_t curr_position = lseek(fd, 0, SEEK_CUR); // Save current position
    off_t file_size = lseek(fd, 0, SEEK_END); // Move to end to get file size
    lseek(fd, curr_position, SEEK_SET); // Restore original position

    if (file_size == -1) {
        perror("Error getting file size");
        return -1; // Error
    }

    return file_size / sizeof(rec_t); // Return number of records
}

int rec_read(int fd, int n, rec_t *pr) {
    int num_records = rec_count(fd);

    if (n < 0 || n >= num_records) {
        printf("Invalid record position.\n");
        return 0; // Unable to read
    }

    off_t offset = n * sizeof(rec_t);
    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("Error seeking");
        return 0; // Unable to seek to the position
    }

    if (read(fd, pr, sizeof(rec_t)) != sizeof(rec_t)) {
        perror("Error reading record");
        return 0; // Unable to read the record
    }

    return 1; // Successful read
}

int rec_write(int fd, int n, rec_t *pr) {
    int num_records = rec_count(fd);

    if (n < 0 || n > num_records) {
        printf("Invalid record position.\n");
        return 0; // Unable to write
    }

    off_t offset = n * sizeof(rec_t);
    if (n == num_records) {
        if (lseek(fd, 0, SEEK_END) == -1) {
            perror("Error seeking to end of file");
            return 0; // Unable to seek to the end of file
        }
    } else {
        if (lseek(fd, offset, SEEK_SET) == -1) {
            perror("Error seeking");
            return 0; // Unable to seek to the position
        }
    }

    if (write(fd, pr, sizeof(rec_t)) != sizeof(rec_t)) {
        perror("Error writing record");
        return 0; // Unable to write the record
    }

    return 1; // Successful write
}

int rec_search(int fd, unsigned ine) {
    rec_t record;
    int position = 0;

    off_t original_position = lseek(fd, 0, SEEK_CUR); // Save original position

    while (read(fd, &record, sizeof(rec_t)) == sizeof(rec_t)) {
        if (record.ine == ine) {
            lseek(fd, original_position, SEEK_SET); // Restore original position
            return position; // Return position of found record
        }
        position++;
    }

    lseek(fd, original_position, SEEK_SET); // Restore original position
    return -1; // INE not found
}