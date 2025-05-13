//FILE area_gestore.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"cliente.h"
#include "area_gestore.h"

void inserisci_cliente(hashtable h)
{
    char nome[50], cognome[50];
    int durata;
    Data data_iscrizione;

    printf("Inserisci il nome del cliente: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Inserisci il cognome del cliente: ");
    fgets(cognome, sizeof(cognome), stdin);
    cognome[strcspn(cognome, "\n")] = '\0';

    do
    {
        printf("Inserisci la durata dell'abbonamento (in mesi): ");
        scanf("%d", &durata);

        if (durata <= 0)
        {
            printf("Errore: la durata non può essere negativa o uguale a 0. Riprova.\n");
        }
    } while (durata <= 0);

    data_iscrizione = data_oggi();

    // Genera automaticamente l'ID
    char* id = genera_id_cliente();

    // Creazione del cliente
    Cliente nuovo_cliente = crea_cliente(id, nome, cognome, durata, data_iscrizione);
    free(id);  // Libera la memoria allocata da genera_id_cliente()

    // Salvataggio nel file
    salva_cliente_file(nuovo_cliente);

    // Inserimento nella hash
    if (insertHash(h, nuovo_cliente))
    {
        printf("Cliente inserito con successo nella hash table!\n");
    }
    else
    {
        printf("Errore: Cliente già presente nella hash table.\n");
    }
}