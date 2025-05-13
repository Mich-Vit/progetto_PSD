//FILE utils.c

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

// Funzione per salvare il contatore in un file
void salva_contatore(int counter)
{
    FILE* file = fopen("counter.txt", "w");  // Apre il file in modalità scrittura (sovrascrive)
    if (file == NULL)
    {
        printf("Errore nell'aprire il file per salvare il contatore.\n");
        exit(1);
    }

    fprintf(file, "%d", counter);  // Scrive il valore del contatore nel file
    fclose(file);  // Chiude il file
}

// Funzione per caricare il contatore da un file
int carica_contatore()
{
    FILE* file = fopen("counter.txt", "r");  // Apre il file in modalità lettura
    int counter = 0;

    if (file != NULL)
    {
        fscanf(file, "%d", &counter);  // Legge il valore del contatore dal file
        fclose(file);  // Chiude il file
    }
    else
    {
        printf("File 'counter.txt' non trovato. Inizializzo il contatore a 0.\n");
    }

    return counter;
}