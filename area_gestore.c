//FILE area_gestore.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"cliente.h"
#include "area_gestore.h"
#include "lista_lezioni.h"
#include "hash.h"
#include "data.h"
#include "utils.h"

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
        printf("Cliente inserito con successo!\n");
    }
    else
    {
        printf("Errore: Cliente già presente!\n");
    }
}

void menu_gestore(hashtable h, list l) 
{
    int scelta;
    do 
    {
        pulisci_schermo();
        printf("====== AREA GESTORE ======\n");
        printf("1) Inserisci un nuovo cliente\n");
        printf("2) Inserisci una nuova lezione\n");
        printf("3) Visualizza la lista dei clienti\n");
        printf("4) Visualizza il report mensile\n");
        printf("0) Esci\n");
        printf("=========================\n");
        printf("Scegli un'opzione: ");
        scanf("%d", &scelta);
        // Pulisce il buffer dopo scanf
        while (getchar() != '\n'); 

        switch (scelta) 
        {
            case 1:
                pulisci_schermo();
                inserisci_cliente(h);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');  // attende un invio
                break;
            case 2:
                //TO DO
                break;

            case 3:
                pulisci_schermo();
                stampaHash(h);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');  // attende un invio
                break;
            case 4:
                //TO DO
                break;

            case 0:
                printf("Arrivederci!\n");
                break;

            default:
                printf("Opzione non valida. Riprova.\n");
                break;
        }
    } while (scelta != 0);
}
