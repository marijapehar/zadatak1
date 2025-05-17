#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

#define OBRADA_FILE "obrada.txt"
#define STATUS_FILE "status.txt"

int current_number = 1;

void handle_sigusr1(int sig) {
    printf("Trenutni broj koji se koristi u obradi: %d\n", current_number);
}

void handle_sigterm(int sig) {
    FILE *status_file = fopen(STATUS_FILE, "w");
    if (status_file == NULL) {
        perror("Otvaranje status.txt");
        exit(1);
    }
    fprintf(status_file, "%d\n", current_number);
    fclose(status_file);
    printf("Zatvaranje programa, trenutni broj je: %d\n", current_number);
    exit(0);
}

void handle_sigint(int sig) {
    FILE *status_file = fopen(STATUS_FILE, "w");
    if (status_file == NULL) {
        perror("Otvaranje status.txt");
        exit(1);
    }
    fprintf(status_file, "0\n");
    fclose(status_file);
    printf("Program prekinut, status postavljen na 0.\n");
    exit(0);
}

// Funkcija koja učitava zadnji broj iz obrada.txt
int load_last_processed_from_obrada() {
    FILE *obrada_file = fopen(OBRADA_FILE, "r");
    if (obrada_file == NULL) {
        return 0;  // Ako nije moguće otvoriti, vraća 0
    }

    int last_processed = 0;
    int num;
    // Čitaj sve brojeve i zadrži zadnji
    while (fscanf(obrada_file, "%d", &num) != EOF) {
        last_processed = num;
    }
    fclose(obrada_file);
    return last_processed;
}

int main() {
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGTERM, handle_sigterm);
    signal(SIGINT, handle_sigint);

    // Prvo učitaj zadnji broj iz obrada.txt
    int last_processed = load_last_processed_from_obrada();
    
    // Ako postoji zadnji broj, nastavi s brojem + 1, inače počni od 1
    if (last_processed > 0) {
        current_number = (int)sqrt(last_processed) + 1;  // Koristi kvadratni korijen i dodaj 1
    } else {
        current_number = 1;
    }

    while (1) {
        FILE *status_file = fopen(STATUS_FILE, "w");
        if (status_file == NULL) {
            perror("Otvaranje status.txt");
            return 1;
        }
        fprintf(status_file, "0\n"); // Postavlja status na 0
        fclose(status_file);

        FILE *obrada_file = fopen(OBRADA_FILE, "a");
        if (obrada_file == NULL) {
            perror("Otvaranje obrada.txt");
            return 1;
        }

        fprintf(obrada_file, "%d\n", current_number * current_number);  // Piši kvadrat trenutnog broja
        fclose(obrada_file);

        printf("Kvadrat broja %d upisan u obrada.txt\n", current_number);

        current_number++;  // Povećaj broj za 1

        sleep(5);  // Uspori izvođenje
    }
    
    return 0;
}

