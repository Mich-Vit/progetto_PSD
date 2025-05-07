//FILE lezione.c

#include<stdio.h>
#include<stdlib.h>
#include"cliente.h"
#include"data.h"
#include"lezione.h"

struct lezione
{
    int id;                     // ID lezione
    char nome[50];             // es. "Zumba", "Pilates"
    Data data;                 // La data della lezione (giorno, mese, anno)
    char orario[6];            // es. "18:30"
    int posti_max;             // capacità totale
    int posti_occupati;        // quanti sono già prenotati
};

Lezione crea_lezione(int id, const char* nome, Data dat, const char* orario, int posti_max)
{
    Lezione nuova_lezione = malloc(sizeof(struct lezione));
    if (nuova_lezione == NULL) 
    {
        printf("Errore di allocazione memoria per la lezione.\n");
        exit(1); 
    }

    nuova_lezione->id = id;
    strcpy(nuova_lezione->nome, nome);
    nuova_lezione->data = copia_data(dat);  // Imposta la data della lezione
    strcpy(nuova_lezione->orario, orario);
    nuova_lezione->posti_max = posti_max;
    nuova_lezione->posti_occupati = 0; // inizialmente nessun posto prenotato

    return nuova_lezione;
}

void libera_lezione(Lezione l) 
{
    if (l != NULL) 
    {
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

    printf("ID Lezione: %d\n", l->id);
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

int get_id_lezione(Lezione l)
{
    return l->id;
}
