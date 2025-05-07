//FILE prenotazione.c

#include<stdio.h>
#include<stdlib.h>
#include"prenotazione.h"
#include"cliente.h"
#include"lezione.h"
#include"data.h"

struct prenotazione
{
    int id_cliente;   // ID del cliente che prenota
    int id_lezione;   // ID della lezione prenotata
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

    // Usa i get per ottenere gli ID
    nuova_prenotazione->id_cliente = get_id_cliente(c);  // Ottieni ID del cliente
    nuova_prenotazione->id_lezione = get_id_lezione(l);  // Ottieni ID della lezione
    
    // Usa copia_data per copiare correttamente la data
    nuova_prenotazione->data_prenotazione = copia_data(data_prenotazione);

    return nuova_prenotazione;
}