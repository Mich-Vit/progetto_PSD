//FILE lezione.c

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"cliente.h"
#include"data.h"
#include"lezione.h"
#include "utils.h"

struct lezione
{
    char* id;                   // ID lezione (ora è una stringa dinamica)
    char nome[50];              // es. "Zumba", "Pilates"
    Data data;                  // La data della lezione (giorno, mese, anno)
    char orario[6];             // es. "18:30"
    int posti_max;              // capacità totale
    int posti_occupati;         // quanti sono già prenotati
};

char* genera_id_lezione()
{
    static int counter = -1;  // Mantiene il contatore tra le chiamate

    if (counter == -1)
    {
        counter = carica_contatore_lezioni();  // Carica il contatore massimo dal file lezioni.txt
    }

    counter++;  // Genera un nuovo ID incrementale

    char* id = malloc(10 * sizeof(char)); 
    if (id == NULL)
    {
        printf("Errore di allocazione memoria per l'ID della lezione.\n");
        exit(1);
    }

    // Genera l'ID nel formato L001, L002, ...
    sprintf(id, "L%03d", counter);  

    return id;
}


Lezione crea_lezione(const char* id, const char* nome, Data dat, const char* orario, int posti_max)
{
    Lezione nuova_lezione = malloc(sizeof(struct lezione));
    if (nuova_lezione == NULL) 
    {
        printf("Errore di allocazione memoria per la lezione.\n");
        exit(1); 
    }

    // Allocazione dinamica per ID
    nuova_lezione->id = malloc(strlen(id) + 1);  // +1 per il terminatore '\0'
    if (nuova_lezione->id == NULL)
    {
        printf("Errore di allocazione memoria per l'ID della lezione.\n");
        free(nuova_lezione); // Dealloca la memoria già allocata per la lezione
        exit(1); // Esci con errore
    }
    strcpy(nuova_lezione->id, id);  // Copia l'ID nella memoria allocata

    strcpy(nuova_lezione->nome, nome);
    nuova_lezione->data = copia_data(dat);  // Imposta la data della lezione
    strcpy(nuova_lezione->orario, orario);
    nuova_lezione->posti_max = posti_max;
    nuova_lezione->posti_occupati = 0; // inizialmente nessun posto prenotato

    return nuova_lezione;
}

void salva_lezione_file(Lezione l)
{
    FILE *fp = fopen("lezioni.txt", "a");
    if (fp == NULL)
    {
        printf("Errore nell'aprire il file lezioni.txt.\n");
        return;
    }

    fprintf(fp, "ID: %s\n", l->id);
    fprintf(fp, "Nome: %s\n", l->nome);
    fprintf(fp, "Data: %02d/%02d/%04d\n",
            get_giorno(l->data), get_mese(l->data), get_anno(l->data));
    fprintf(fp, "Orario: %s\n", l->orario);
    fprintf(fp, "Posti massimi: %d\n", l->posti_max);
    fprintf(fp, "Posti occupati: %d\n", l->posti_occupati);
    fprintf(fp, "-----------------------\n");

    fclose(fp);
}


void libera_lezione(Lezione l) 
{
    if (l != NULL) 
    {
        // Libera la memoria per l'ID
        free(l->id); // Libera la memoria allocata per l'ID

        // Libera la memoria per la data
        libera_data(l->data);  // Funzione che libera la memoria allocata per la data

        // Libera la memoria per la lezione
        free(l);
    }
}

void visualizza_lezione(Lezione l) 
{
    if (l == NULL) 
    {
        printf("Lezione non valida.\n");
        return;
    }

    printf("===================================\n");
    printf("ID Lezione: %s\n", l->id);  // Stampa l'ID come stringa
    printf("Nome: %s\n", l->nome);
    printf("Data: ");
    visualizza_data(l->data);  // Stampa la data della lezione
    printf("Orario: %s\n", l->orario);
    printf("Posti occupati: %d/%d\n", l->posti_occupati, l->posti_max);

    if (l->posti_occupati < l->posti_max)
    {
        printf("Posti disponibili: %d\n", l->posti_max - l->posti_occupati);
    }
    else
    {
        printf("Lezione al completo.\n");
    }
}




int prenota_lezione(Lezione l, Cliente c)
{
    if (l == NULL || c == NULL) 
    {
        printf("Errore: lezione o cliente non valido.\n");
        return 0;
    }

    // Verifica se l'abbonamento del cliente è valido
    if (!abbonamento_valido(data_oggi(), get_data_scadenza(c))) 
    {
        printf("Abbonamento non valido. Impossibile prenotare la lezione.\n");
        return 0; // Prenotazione fallita
    }

    //verifica che ci sono posti disponibili 
    if(l->posti_occupati >= l->posti_max)
    {
        printf("Lezione al completo. Impossibile prenotare.\n");
        return 0; // Prenotazione fallita
    }

    // Prenotazione riuscita
    l->posti_occupati++;
    printf("Prenotazione riuscita per la lezione: %s\n", l->nome);
    return 1;
}


char* get_id_lezione(Lezione l)
{
    return l->id; 
}

void set_posti_occupati(Lezione l, int pos_occupati)
{
    if (l == NULL)
        return;

    if (pos_occupati >= 0 && pos_occupati <= l->posti_max)
    {
        l->posti_occupati = pos_occupati;
    }
    else
    {
        printf("Errore: posti occupati non validi (deve essere tra 0 e %d).\n", l->posti_max);
    }
}
