//FILE area_clienti.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "cliente.h"
#include "hash.h"
#include"data.h"
#include"lista_lezioni.h"
#include"utils.h"
#include "hash_prenotazioni.h"

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
    else if(!abbonamento_valido(data_oggi(), get_data_scadenza(c)))
    {
        printf("Abbonamento scaduto!\n");
        printf("\nPremi INVIO per tornare al menu...");
        while (getchar() != '\n');
        return NULL;
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
    scelta = leggi_intero();

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

void prenota_lezione(Cliente c, list l, hashtable_p h)
{
    printf("======================================================\n");
    printf("\t    LISTA DELLE LEZIONI DISPONIBILI\n");
    printf("======================================================\n");

    list tmp = l;
    int trovate = 0;

    if(emptyList(l))
    {
        printf("Nessuna lista di lezioni disponibile\n");
        return;
    }

    stampa_lezioni_libere(l); 

    printf("------------------------------------------------------\n");
    char id_lezione_scelta[20];
    printf("Inserisci l'ID della lezione da prenotare: ");
    fgets(id_lezione_scelta, sizeof(id_lezione_scelta), stdin); 
    id_lezione_scelta[strcspn(id_lezione_scelta, "\n")] = '\0';

    // Cerca la lezione nella lista
    Lezione l_selezionata = cerca_lezione_per_id(l, id_lezione_scelta);
    if (l_selezionata == NULL)
    {
        printf("Lezione con ID %s non trovata.\n", id_lezione_scelta);
        return;
    }

    int posti_occupati = get_posti_occupati(l_selezionata);
    int posti_max = get_posti_max(l_selezionata);
    int posti_disponibili = posti_max - posti_occupati;

    if (posti_disponibili <= 0)
    {
        printf("Spiacente, non ci sono piu' posti disponibili per questa lezione.\n");
        return;
    }

    // Verifica se il cliente è già prenotato alla lezione
    Prenotazione* table = get_table_hash_p(h);
    for (int i = 0; i < get_size_hash_p(h); i++)
    {
        Prenotazione curr = table[i];
        while (curr != NULL)
        {
            if (strcmp(get_id_cliente_prenotazione(curr), get_id_cliente(c)) == 0 &&
            strcmp(get_id_lezione_prenotazione(curr), get_id_lezione(l_selezionata)) == 0)
            {
                printf("Gia' sei prenotato a questa lezione.\n");
                return;
            }
            curr = get_next_prenotazione(curr);
        }
    }

    // Incrementa i posti occupati
    set_posti_occupati(l_selezionata, posti_occupati + 1);

    // Aggiorna il file con tutte le lezioni aggiornate
    aggiorna_file_lezioni(l);

    Data data_pre = data_oggi();


    char *id_prenotazione = genera_id_generico("P", "prenotazioni.txt");
    Prenotazione p = crea_prenotazione(id_prenotazione, get_id_cliente(c), get_id_lezione(l_selezionata), data_pre);

    // Inserisci nella tabella hash delle prenotazioni
    insertHash_p(h, p);

    salva_prenotazione_file(p);

    printf("Prenotazione effettuata con successo!\n");
}

void disdici_prenotazione(Cliente c, hashtable_p hp, list l)
{
    if(hp == NULL)
    {
        printf("Nessuna lista di prenotazioni trovata\n");
        return;
    }

    stampa_prenotazioni_cliente(c, hp, l);

    printf("------------------------------------------------------\n");
    char id_prenotazione_scelta[20];
    printf("Inserisci l'ID della prenotazione da disdire: ");
    fgets(id_prenotazione_scelta, sizeof(id_prenotazione_scelta), stdin); 
    id_prenotazione_scelta[strcspn(id_prenotazione_scelta, "\n")] = '\0';

    Prenotazione rem = hashDelete_p(hp, id_prenotazione_scelta);

    if(rem == NULL)
    {
        printf("Prenotazione con ID %s non trovato.\n", id_prenotazione_scelta);
        return;
    }

    char* id_lezione = get_id_lezione_prenotazione(rem);

    // Trova la lezione nella lista e decrementa i posti
    list temp = l;
    while (!emptyList(temp))
    {
        Lezione le = getFirst(temp);
        if (strcmp(get_id_lezione(le), id_lezione) == 0)
        {
            // Decrementa i posti occupati
            int attuali = get_posti_occupati(le);
            set_posti_occupati(le, attuali - 1);
            break;
        }
        temp = tailList(temp);
    }
    libera_prenotazione(rem);

    aggiorna_file_prenotazioni(hp);  
    aggiorna_file_lezioni(l);

    printf("Prenotazione disdetta con successo!\n");
}

void menu_cliente(Cliente c, hashtable h, list l, hashtable_p hp) 
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
        printf("4) Disdici una prenotazione\n");
        printf("5) Visualizza lezioni\n");
        printf("6) Visualizza le tue prenotazione\n");
        printf("0) Esci\n");
        printf("==============================================\n");
        printf("Scegli un'opzione: ");
        scelta = leggi_intero(); 

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
                pulisci_schermo();
                prenota_lezione(c, l, hp);

                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;

            case 4:
                pulisci_schermo();
                disdici_prenotazione(c, hp, l);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;    

            case 5:
                pulisci_schermo();
                visualizza_lezioni(l);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;

            case 6:
                pulisci_schermo();
                stampa_prenotazioni_cliente(c, hp, l);
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
