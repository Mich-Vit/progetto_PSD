//FILE prenotazione.c

#include<stdio.h>
#include<stdlib.h>
#include"prenotazione.h"
#include"cliente.h"
#include"lezione.h"
#include"data.h"

struct prenotazione
{
    char *id_cliente;   
    char *id_lezione;   
    Data data_prenotazione;
};


Prenotazione crea_prenotazione(Cliente c, Lezione l, Data data_prenotazione) 
{
    Prenotazione nuova_prenotazione = malloc(sizeof(struct prenotazione));
    if (nuova_prenotazione == NULL) 
    {
        printf("Errore nell'allocazione memoria per la prenotazione\n");
        exit(1);
    }

    nuova_prenotazione->id_cliente = malloc(strlen(get_id_cliente(c)) + 1);
    if (nuova_prenotazione->id_cliente == NULL)
    {
        printf("Errore nell'allocazione memoria per l'ID del cliente\n");
        free(nuova_prenotazione);
        exit(1);
    }
    strcpy(nuova_prenotazione->id_cliente, get_id_cliente(c));  // Copia l'ID del cliente

    nuova_prenotazione->id_lezione = malloc(strlen(get_id_lezione(l)) + 1);
    if (nuova_prenotazione->id_lezione == NULL)
    {
        printf("Errore nell'allocazione memoria per l'ID della lezione\n");
        free(nuova_prenotazione->id_cliente);
        free(nuova_prenotazione);
        exit(1);
    }
    strcpy(nuova_prenotazione->id_lezione, get_id_lezione(l));  // Copia l'ID della lezione
    
    // Copia la data della prenotazione
    nuova_prenotazione->data_prenotazione = copia_data(data_prenotazione);

    return nuova_prenotazione;
}

void libera_prenotazione(Prenotazione p)
{
    if (p != NULL)
    {
        free(p->id_cliente); 
        free(p->id_lezione);
        libera_data(p->data_prenotazione);
        free(p);
    }
}
