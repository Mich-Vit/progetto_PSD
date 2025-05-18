//FILE prenotazione.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"prenotazione.h"
#include"cliente.h"
#include"lezione.h"
#include"data.h"
#include"utils.h"

struct prenotazione
{
    char *id_prenotazione;
    char *id_cliente;   
    char *id_lezione;   
    Data data_prenotazione;

    struct prenotazione *next;
};


Prenotazione crea_prenotazione(char *id_pren, char* id_cli, char* id_lez, Data data_prenotazione) 
{
    Prenotazione nuova_prenotazione = malloc(sizeof(struct prenotazione));
    if (nuova_prenotazione == NULL) 
    {
        printf("Errore nell'allocazione memoria per la prenotazione\n");
        exit(1);
    }

    nuova_prenotazione->id_prenotazione = malloc(strlen(id_pren) + 1);
    if (nuova_prenotazione->id_prenotazione == NULL)
    {
        printf("Errore nell'allocazione memoria per l'ID della prenotazione\n");
        free(nuova_prenotazione);
        exit(1);
    }
    strcpy(nuova_prenotazione->id_prenotazione, id_pren);

    nuova_prenotazione->id_cliente = malloc(strlen(id_cli) + 1);
    if (nuova_prenotazione->id_cliente == NULL)
    {
        printf("Errore nell'allocazione memoria per l'ID del cliente\n");
        free(nuova_prenotazione->id_prenotazione);
        free(nuova_prenotazione);
        exit(1);
    }
    strcpy(nuova_prenotazione->id_cliente, id_cli);

    nuova_prenotazione->id_lezione = malloc(strlen(id_lez) + 1);
    if (nuova_prenotazione->id_lezione == NULL)
    {
        printf("Errore nell'allocazione memoria per l'ID della lezione\n");
        free(nuova_prenotazione->id_cliente);
        free(nuova_prenotazione->id_prenotazione);
        free(nuova_prenotazione);
        exit(1);
    }
    strcpy(nuova_prenotazione->id_lezione, id_lez);

    nuova_prenotazione->data_prenotazione = copia_data(data_prenotazione);
    nuova_prenotazione->next = NULL;

    return nuova_prenotazione;
}


void salva_prenotazione_file(Prenotazione p)
{
    FILE *fp = fopen("prenotazioni.txt", "a");
    if (fp == NULL)
    {
        printf("Errore nell'aprire il file prenotazioni.txt.\n");
        return;
    }

    fprintf(fp, "ID: %s\n", p->id_prenotazione);
    fprintf(fp, "ID Cliente: %s\n", p->id_cliente);
    fprintf(fp, "ID Lezione: %s\n", p->id_lezione);
    fprintf(fp, "Data Prenotazione: %02d/%02d/%04d\n",
            get_giorno(p->data_prenotazione),
            get_mese(p->data_prenotazione),
            get_anno(p->data_prenotazione));

    fprintf(fp, "-----------------------\n");

    fclose(fp);
}


void visualizza_prenotazione(Prenotazione p)
{
    if (p == NULL)
    {
        printf("Prenotazione nulla.\n");
        return;
    }

    printf("ID Prenotazione: %s\n", p->id_prenotazione);
    printf("ID Cliente: %s\n", p->id_cliente);
    printf("ID Lezione: %s\n", p->id_lezione);
    printf("Data Prenotazione: ");
    visualizza_data(p->data_prenotazione);
    printf("\n");
}

void visualizza_essenziale_prenotazione(Prenotazione p)
{
    if (p == NULL)
    {
        printf("Prenotazione nulla.\n");
        return;
    }

    printf("%-15s %-15s %-15s\n",
           p->id_prenotazione,
           p->id_cliente,
           p->id_lezione);
}


void libera_prenotazione(Prenotazione p)
{
    if (p != NULL)
    {
        free(p->id_prenotazione); 
        free(p->id_cliente);
        free(p->id_lezione);
        libera_data(p->data_prenotazione);
        free(p);
    }
}

//====GETTER===

char* get_id_prenotazione(Prenotazione p)
{
    if (p == NULL)
        return NULL;
    return p->id_prenotazione;
}

char* get_id_cliente_prenotazione(Prenotazione p)
{
    if (p == NULL)
        return NULL;
    return p->id_cliente;
}

char* get_id_lezione_prenotazione(Prenotazione p)
{
    if (p == NULL)
        return NULL;
    return p->id_lezione;
}

Data get_data_prenotazione(Prenotazione p)
{
    if (p == NULL)
        return NULL;
    return p->data_prenotazione;
}

Prenotazione get_next_prenotazione(Prenotazione p)
{
    if (p == NULL)
        return NULL;
    return p->next;
}

// Setter 
void set_next_prenotazione(Prenotazione p, Prenotazione next)
{
    if (p != NULL)
        p->next = next;
}
