//FILE area_clienti.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "cliente.h"
#include "hash.h"
#include"data.h"
#include"lista_lezioni.h"
#include"utils.h"

// Funzione di login: richiede l'ID cliente e cerca nella hashtable
Cliente login_cliente(hashtable h)
{
    char id[20];

    printf("Inserisci il tuo ID cliente: ");
    fgets(id, sizeof(id), stdin); 
    id[strcspn(id, "\n")] = '\0';  

    Cliente c = hashSearch(h, id);

    if (c != NULL)
    {
        printf("Benvenuto, %s %s!\n", get_nome_cliente(c), get_cognome_cliente(c));
    }
    else
    {
        printf("Cliente non trovato. Verifica l'ID inserito.\n");
    }

    return c;
}

// Funzione di rinnovo dell'abbonamento
void rinnova_abbonamento(Cliente c, hashtable h)
{
    if (c == NULL)
    {
        printf("Cliente non valido.\n");
        return;
    }

    Data data_scadenza_attuale = get_data_scadenza(c);

    int durata;
    do
    {
        printf("Inserisci la durata del rinnovo in mesi (numero positivo): ");
        scanf("%d", &durata);

        if (durata < 0)
        {
            printf("Errore: la durata non può essere negativa. Riprova.\n");
        }
    } while (durata < 0);

    // Calcola la nuova data di scadenza aggiungendo la durata all'abbonamento
    Data nuova_data_scadenza = calcolo_scadenza_abbonamento(data_scadenza_attuale, durata);
    set_data_scadenza(c, nuova_data_scadenza);

    // Calcola la durata effettiva dell'abbonamento (differenza tra data di scadenza e data di iscrizione)
    int durata_effettiva = calcola_durata_in_mesi(get_data_iscrizione(c), get_data_scadenza(c));    
    set_durata(c, durata_effettiva);

    riscrivi_file_clienti(h);

    printf("Il tuo abbonamento è stato rinnovato fino al ");
    visualizza_data(nuova_data_scadenza);  // Funzione per visualizzare la data
    printf("\n");
}

void menu_cliente(Cliente c, hashtable h, list l) 
{
    int scelta;
    do 
    {
        printf("\n====== AREA CLIENTI ======\n");
        printf("1) Visualizza informazioni sull'abbonamento\n");
        printf("2) Rinnova abbonamento\n");
        printf("3) Prenota una lezione\n");
        printf("4) Visualizza lezioni disponibili\n");
        printf("0) Esci\n");
        printf("=========================\n");
        printf("Scegli un'opzione: ");
        scanf("%d", &scelta);
        // Pulisce il buffer dopo scanf
        while (getchar() != '\n'); 

        switch (scelta) 
        {
            case 1:
                visualizza_abbonamento_cliente(c);
                break;

            case 2:
                rinnova_abbonamento(c, h);
                break;

            case 3:
                // Prenotazione lezione
                break;

            case 4:
                outputList(l);
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
