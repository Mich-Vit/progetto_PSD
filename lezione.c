//FILE lezione.c

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"cliente.h"
#include"data.h"
#include"lezione.h"
#include "utils.h"
#include "orario.h"

struct lezione
{
    char* id;                   // ID lezione (ora è una stringa dinamica)
    char nome[50];              // es. "Zumba", "Pilates"
    Data data;                  // La data della lezione (giorno, mese, anno)
    Orario ora_lezione;            // es. "18:30"
    int posti_max;              // capacità totale
    int posti_occupati;         // quanti sono già prenotati
};

Lezione crea_lezione(const char* id, const char* nome, Data dat, Orario orario, int posti_max)
{
    Lezione nuova_lezione = malloc(sizeof(struct lezione));
    if (nuova_lezione == NULL) 
    {
        printf("Errore di allocazione memoria per la lezione.\n");
        exit(1); 
    }

    // Allocazione dinamica per ID
    nuova_lezione->id = malloc(strlen(id) + 1);
    if (nuova_lezione->id == NULL)
    {
        printf("Errore di allocazione memoria per l'ID della lezione.\n");
        free(nuova_lezione);
        exit(1);
    }
    strcpy(nuova_lezione->id, id);

    strcpy(nuova_lezione->nome, nome);
    nuova_lezione->data = copia_data(dat);
    nuova_lezione->ora_lezione = copia_orario(orario);
    nuova_lezione->posti_max = posti_max;
    nuova_lezione->posti_occupati = 0;

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

    fprintf(fp, "Orario: %02d:%02d\n", get_ora(l->ora_lezione), get_minuti(l->ora_lezione));

    fprintf(fp, "Posti massimi: %d\n", l->posti_max);
    fprintf(fp, "Posti occupati: %d\n", l->posti_occupati);
    fprintf(fp, "-----------------------\n");

    fclose(fp);
}


void libera_lezione(Lezione l) 
{
    if (l != NULL) 
    {
        free(l->id);               // libera l'ID (se mallocato)
        libera_data(l->data);      // libera la data
        libera_orario(l->ora_lezione);  // libera l'orario
        free(l);                   // libera la struttura Lezione
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
    printf("ID Lezione: %s\n", l->id);
    printf("Nome: %s\n", l->nome);
    printf("Data: ");
    visualizza_data(l->data);
    printf("Orario: ");
    stampa_orario(l->ora_lezione);
    printf("\nPosti occupati: %d/%d\n", l->posti_occupati, l->posti_max);

    if (l->posti_occupati < l->posti_max)
    {
        printf("Posti disponibili: %d\n", l->posti_max - l->posti_occupati);
    }
    else
    {
        printf("Lezione al completo.\n");
    }
}

int confronta_lezioni(Lezione l1, Lezione l2)
{
    int cmp_data = confronta_date(l1->data, l2->data);
    if (cmp_data != 0) //non sono uguali le date
        return cmp_data;

    //se le date sono uguali confronta l'orario
    return confronta_orario(l1->ora_lezione, l2->ora_lezione);
}

void visualizza_essenziale_lezione(Lezione le)
{
    char buffer_ora[6]; 
    char buffer_data[11]; 

    snprintf(buffer_ora, sizeof(buffer_ora), "%02d:%02d", get_ora(le->ora_lezione), get_minuti(le->ora_lezione));
    snprintf(buffer_data, sizeof(buffer_data), "%02d/%02d/%04d", get_giorno(le->data), get_mese(le->data), get_anno(le->data));

    // Stampa allineata
    printf("%-8s %-15s %-10s %-12s\n", le->id, le->nome, buffer_ora, buffer_data);
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

int get_posti_occupati(Lezione l)
{
    return l->posti_occupati; 
}

int get_posti_max(Lezione l)
{
    return l->posti_max; 
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
