//FILE utils.c

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "data.h"
#include "hash.h"
#include "cliente.h"


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

void carica_clienti_da_file(hashtable h)
{
    FILE *fp = fopen("clienti.txt", "r");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file clienti.txt\n");
        return;
    }

    char id[20], nome[50], cognome[50];
    int durata, giorno, mese, anno, gs, ms, as;

    while (fscanf(fp, "ID: %s\n", id) == 1)
    {
        fscanf(fp, "Nome: %s\n", nome);
        fscanf(fp, "Cognome: %s\n", cognome);
        fscanf(fp, "Durata abbonamento: %d\n", &durata);
        fscanf(fp, "Data d'iscrizione: %d/%d/%d\n", &giorno, &mese, &anno);
        fscanf(fp, "Data scadenza: %d/%d/%d\n", &gs, &ms, &as);
        fscanf(fp, "-----------------------\n");

        Data data_is = crea_data(giorno, mese, anno);
        Data data_sca =  crea_data(gs, ms, as);

        Cliente c = crea_cliente(id, nome, cognome, durata, data_is);
        set_data_scadenza(c, data_sca);
        insertHash(h, c);
    }

    fclose(fp);
}

// Funzione per pulire lo schermo, portabile su Windows, Linux e macOS
void pulisci_schermo()
{
    #ifdef _WIN32  // Verifica se il sistema è Windows
        system("cls");
    #else  // Se il sistema è Linux o macOS
        system("clear");
    #endif
}