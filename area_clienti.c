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

    if (c == NULL)
    {
        printf("Cliente non trovato. Verifica l'ID inserito.\n");
        printf("\nPremi INVIO per tornare al menu...");
        while (getchar() != '\n');
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
    char buffer[100];
    int valido = 0;

    do
    {
        printf("Inserisci la durata del rinnovo in mesi (numero intero positivo): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Controlla che sia un numero intero e che non ci siano caratteri extra
        if (sscanf(buffer, "%d", &durata) == 1 && durata >= 0)
        {
            valido = 1;
        }
        else
        {
            printf("Errore: inserisci un numero intero valido e positivo.\n");
        }
    } while (!valido);

    // Calcola la nuova data di scadenza aggiungendo la durata all'abbonamento
    Data nuova_data_scadenza = calcolo_scadenza_abbonamento(data_scadenza_attuale, durata);
    set_data_scadenza(c, nuova_data_scadenza);

    // Calcola la durata effettiva dell'abbonamento (differenza tra data di scadenza e data di iscrizione)
    int durata_effettiva = calcola_durata_in_mesi(get_data_iscrizione(c), get_data_scadenza(c));    
    set_durata(c, durata_effettiva);

    riscrivi_file_clienti(h);

    printf("Il tuo abbonamento e' stato rinnovato fino al ");
    visualizza_data(nuova_data_scadenza);  // Funzione per visualizzare la data
    printf("\n");
}

void visualizza_lezioni(list l)
{
    printf("==============================================\n");
    printf("\t      LISTA DELLE LEZIONI\n");
    printf("==============================================\n");
    
    if (l == NULL || emptyList(l))
    {
        printf("Nessuna lista di lezioni trovata.\n");
        return;
    }

    int scelta;
    printf("Cosa vuoi visualizzare?\n");
    printf("1) Tutte le lezioni\n");
    printf("2) Solo lezioni disponibili\n");
    printf("Scelta: ");
    scanf("%d", &scelta);
    getchar(); // consuma l'\n rimasto nel buffer

    list tmp = l;
    int trovate = 0;

    while (!emptyList(tmp))
    {
        Lezione le = getFirst(tmp);

        if (scelta == 1 || (scelta == 2 && (get_posti_max(le) - get_posti_occupati(le)) > 0))
        {
            visualizza_lezione(le); 
            trovate++;
        }

        tmp = tailList(tmp);
    }

    if (trovate == 0)
    {
        printf("Nessuna lezione trovata con i criteri selezionati.\n");
    }
}

void menu_cliente(Cliente c, hashtable h, list l) 
{
    int scelta;
    do 
    {
        pulisci_schermo();
        printf("==============================================\n");
        printf("\tBenvenuto, %s %s!\n", get_nome_cliente(c), get_cognome_cliente(c));
        printf("==============================================\n");
        printf("1) Visualizza informazioni sul tuo abbonamento\n");
        printf("2) Rinnova abbonamento\n");
        printf("3) Prenota una lezione\n");
        printf("4) Visualizza lezioni\n");
        printf("0) Esci\n");
        printf("==============================================\n");
        printf("Scegli un'opzione: ");
        scanf("%d", &scelta);
        // Pulisce il buffer dopo scanf
        while (getchar() != '\n'); 

        switch (scelta) 
        {
            case 1:
                pulisci_schermo();
                printf("==============================================\n");
                printf("\tINFORMAZIONI SULL'ABBONAMENTO\n");
                printf("==============================================\n");
                visualizza_abbonamento_cliente(c);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;

            case 2:
                rinnova_abbonamento(c, h);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;

            case 3:
                // Prenotazione lezione
                break;

            case 4:
                pulisci_schermo();
                visualizza_lezioni(l);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;

            case 0:
                printf("Arrivederci!\n");
                break;

            default:
                printf("Opzione non valida. Riprova.\n");
                printf("\nPremi INVIO...");
                while (getchar() != '\n');
                break;
        }
    } while (scelta != 0);
}
