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
#include "lezione.h"
#include "orario.h"

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

list inserisci_lezione(list l)
{
    char nome[50];
    int posti_max;
    Data data;
    Orario orario = NULL;
    int ore, min;

    // Input nome lezione
    printf("Inserisci il nome della lezione: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';  // rimuove newline

    // Input orario con controllo formato e valori
    do {
        printf("Inserisci l'orario della lezione (HH:MM): ");
        if (scanf("%d:%d", &ore, &min) != 2)
        {
            printf("Formato orario non valido. Riprova.\n");
            while(getchar() != '\n'); // pulisce buffer
            continue;
        }
        while(getchar() != '\n'); // pulisce buffer

        orario = crea_orario(ore, min);
        if (orario == NULL)
        {
            printf("Orario non valido. Riprova.\n");
        }
    } while (orario == NULL);

    do
    {
        printf("Inserisci il numero massimo di posti: ");
        if (scanf("%d", &posti_max) != 1)
        {
            printf("Input non valido. Riprova.\n");
            while(getchar() != '\n'); // pulisce buffer
            continue;
        }
        while (getchar() != '\n');  // Pulisce il buffer

        if (posti_max <= 0)
        {
            printf("Errore: il numero di posti deve essere positivo. Riprova.\n");
        }
    } while (posti_max <= 0);

    data = copia_data(leggi_data());

    char* id = genera_id_lezione();

    Lezione nuova_lezione = crea_lezione(id, nome, data, orario, posti_max);

    l = consList(nuova_lezione, l);

    salva_lezione_file(nuova_lezione);

    printf("Lezione inserita con successo!\n");

    free(id);

    return l;
}

void rimuovi_cliente(hashtable h)
{
    printf("===================================\n");
    printf("         Rimuovi un cliente\n");
    printf("===================================\n");

    if (h == NULL)
    {
        printf("Nessuna tabella trovata.\n");
        return;
    }

    char id[20];

    printf("Inserisci l'ID del cliente da rimuovere: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    Cliente rem = hashDelete(h, id);

    if (rem == NULL)
    {
        printf("Cliente con ID %s non trovato.\n", id);
        return;
    }

    distruggi_cliente(rem);  // libera la memoria solo se rem non è NULL

    printf("Cliente rimosso dalla tabella hash.\n");

    FILE *file = fopen("clienti.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Errore nell'apertura dei file.\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    char riga[256];
    int saltare = 0;

    while (fgets(riga, sizeof(riga), file))
    {
        if (strncmp(riga, "ID:", 3) == 0)
        {
            char id_letto[20];
            sscanf(riga, "ID: %s", id_letto);

            if (strcmp(id_letto, id) == 0)
            {
                saltare = 1;
                continue;  // salta la riga dell'ID
            }
            else
            {
                saltare = 0;
            }
        }

        if (saltare)
        {
            continue;  // salta le righe del cliente da eliminare
        }

        fputs(riga, temp);
    }

    fclose(file);
    fclose(temp);

    remove("clienti.txt");
    rename("temp.txt", "clienti.txt");
}

void ricerca_cliente(hashtable h)
{
    printf("===================================\n");
    printf("         Ricerca un cliente\n");
    printf("===================================\n");

    if (h == NULL)
    {
        printf("Nessuna tabella trovata.\n");
        return;
    }

    char id[20];

    printf("Inserisci l'ID del cliente da cercare: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';  // Rimuove il newline

    if (strlen(id) == 0)
    {
        printf("ID non valido.\n");
        return;
    }

    Cliente ricercato = hashSearch(h, id);

    if (ricercato == NULL)
    {
        printf("Cliente con ID %s non trovato nella tabella hash.\n", id);
        return;
    }

    printf("Cliente trovato:\n");
    visualizza_cliente(ricercato);
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
        printf("4) Visualizza la lista delle lezioni\n");
        printf("5) Rimuovi un cliente\n");
        printf("6) Rimuovi una lezione\n");
        printf("7) Ricerca un cliente\n");
        printf("8) Visualizza il report mensile\n");
        printf("0) Esci\n");
        printf("==========================\n");
        printf("Scegli un'opzione: ");
        scanf("%d", &scelta);
        while (getchar() != '\n'); // pulizia buffer

        switch (scelta)
        {
            case 1:
                pulisci_schermo();
                inserisci_cliente(h);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 2:
                pulisci_schermo();
                l = inserisci_lezione(l);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 3:
                pulisci_schermo();
                stampaHash(h);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 4:
                pulisci_schermo();
                outputList(l);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 5:
                pulisci_schermo();
                rimuovi_cliente(h);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 6:
                pulisci_schermo();
                // rimuovi_lezione(l);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 7:
                pulisci_schermo();
                ricerca_cliente(h);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 8:
                pulisci_schermo();
                // visualizza_report_mensile(h, l);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
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
