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
#include "hash_prenotazioni.h"
#include "report.h"

/*
* Funzione: inserisci_cliente
* ----------------------------------------
* Permette di inserire un nuovo cliente nella hashtable e nel file di testo.
*
* Parametri:
*   h: hashtable dei clienti.
*
* Pre-condizione:
*   h deve essere valida.
*
* Post-condizione:
*   Un nuovo cliente viene aggiunto alla hashtable e salvato su file.
*   In caso di ID duplicato, viene segnalato un errore.
*
* Come funziona:
* - Legge nome, cognome e durata abbonamento da input, con controlli di validità.
* - Genera un ID univoco per il cliente con genera_id_generico.
* - Crea il cliente e lo salva su file.
* - Inserisce il cliente nella hashtable e nel file di testo.
*/
static void inserisci_cliente(hashtable h)
{
    char nome[50], cognome[50];
    int durata;
    Data data_iscrizione;

    printf("==============================================\n");
    printf("\tINSERISCI UN CLIENTE\n");
    printf("==============================================\n");

    int valido;
    do
    {
        printf("Inserisci il nome: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = '\0';

        valido = solo_lettere(nome);
        if (!valido)
        {
            printf("Errore: il nome deve contenere solo lettere. Riprova.\n");
        }
    } while (!valido);


    do
    {
        printf("Inserisci il cognome: ");
        fgets(cognome, sizeof(cognome), stdin);
        cognome[strcspn(cognome, "\n")] = '\0';
    
        if (!solo_lettere(cognome))
        {
            printf("Errore: il cognome deve contenere solo lettere. Riprova.\n");
        }
    } while (!solo_lettere(cognome));

    do
    {
        printf("Inserisci la durata dell'abbonamento (in mesi): ");
        durata = leggi_intero();

        if (durata <= 0)
        {
            printf("Errore: la durata non può essere negativa o uguale a 0. Riprova.\n");
        }
    } while (durata <= 0);

    data_iscrizione = data_oggi();

    char* id = genera_id_generico("C", "clienti.txt");

    Cliente nuovo_cliente = crea_cliente(id, nome, cognome, durata, data_iscrizione);
    free(id); 
    salva_cliente_file(nuovo_cliente);

    if (insertHash(h, nuovo_cliente))
    {
        printf("Cliente inserito con successo!\n");
    }
    else
    {
        printf("Errore: Cliente già presente!\n");
    }
}

/*
* Funzione: nome_lezione_valido
* ----------------------------------------
* Verifica se il nome della lezione è uno tra quelli permessi.
*
* Parametri:
*   nome: stringa che rappresenta il nome della lezione da validare.
*
* Pre-condizione:
*   nome deve essere una stringa valida (non NULL).
*
* Post-condizione:
*   1 se il nome è valido (presente nella lista delle lezioni permesse),
*   0 altrimenti.
*
* Come funziona:
* - Confronta il nome fornito con ogni nome nella lista delle lezioni permesse.
* - Restituisce 1 se trova una corrispondenza, altrimenti 0.
*/
static int nome_lezione_valido(const char* nome)
{
    const char* lezioni_permesse[] =
    {
        "Sala pesi",
        "Pugilato",
        "Danza Classica",
        "Yoga",
        "Zumba",
        "Pilates",
        "Crossfit"
    };
    
    //calcola il numero di elementi presenti in lezioni_permesse[]
    const int num_lezioni = sizeof(lezioni_permesse) / sizeof(lezioni_permesse[0]);

    for (int i = 0; i < num_lezioni; i++)
    {
        if (strcmp(nome, lezioni_permesse[i]) == 0)
            return 1;
    }

    return 0;
}

/*
* Funzione: data_valida_per_lezione
* ----------------------------------------
* Verifica che la data della lezione sia valida,
* cioè che sia nel mese corrente o in quello successivo.
*
* Parametri:
*   data: oggetto Data da controllare.
*
* Pre-condizione:
*   data deve essere un oggetto Data valido (non NULL).
*
* Valore di ritorno:
*   1 se la data è valida (nel mese corrente o nel successivo),
*   0 altrimenti.
*
* Come funziona:
* - Ottiene la data odierna.
* - Estrae mese e anno sia dalla data odierna che dalla data da controllare.
* - Calcola mese e anno del mese successivo.
* - Controlla se la data fornita appartiene al mese corrente o a quello successivo.
* - Libera la memoria della data odierna.
*/
static int data_valida_per_lezione(Data data)
{
    if (data == NULL)
        return 0;  

    Data oggi = data_oggi(); 

    int mese_oggi = get_mese(oggi);
    int anno_oggi = get_anno(oggi);

    int mese_le = get_mese(data);
    int anno_le = get_anno(data);

    // Calcola mese e anno del mese successivo
    int mese_prossimo = (mese_oggi % 12) + 1;
    int anno_prossimo;
    if (mese_oggi == 12)
        anno_prossimo = anno_oggi + 1;
    else
        anno_prossimo = anno_oggi;

    // Verifica se la data fornita è nel mese corrente o in quello successivo
    int valida = 0;
    if ((anno_le == anno_oggi && mese_le == mese_oggi) ||
        (anno_le == anno_prossimo && mese_le == mese_prossimo))
    {
        valida = 1;
    }

    libera_data(oggi); 
    return valida;
}

/*
* Funzione: inserisci_lezione
* ----------------------------------------
* Permette di inserire una nuova lezione nella lista e nel file di testo.
*
* Parametri:
*   l: lista corrente delle lezioni.
*
* Pre-condizione:
*   l deve essere valida.
*
* Post-condizione:
*   Viene aggiunta una nuova lezione alla lista e salvata nel file.
*
* Come funziona:
* - Richiede nome lezione, orario, data e numero max di posti con controlli di validità.
* - Verifica che il nome lezione sia consentito.
* - Verifica che la data sia nel mese corrente o successivo.
* - Crea la lezione con un ID generato.
* - Inserisce la lezione in testa alla lista.
* - Salva la lezione nel file.
*/
static list inserisci_lezione(list l)
{
    char nome[50];
    int posti_max;
    Data data = NULL;
    Orario orario = NULL;
    int ore, min;

    Data oggi = data_oggi();
    int mese_corrente = get_mese(oggi);
    int anno_corrente = get_anno(oggi);

    printf("===============================================\n");
    printf("\t    INSERISCI UNA LEZIONE\n");
    printf("===============================================\n");

    printf("\tLe lezioni consentite sono:\n");
    printf(" - Sala pesi - Pugilato - Danza Classica - Yoga\n - Zumba - Pilates - Crossfit\n");
    printf("-----------------------------------------------\n");
    do
    {
        printf("Inserisci il nome: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = '\0';

        if (!solo_lettere(nome))
        {
            printf("Errore: il nome deve contenere solo lettere. Riprova.\n");
            continue;
        }

        if (!nome_lezione_valido(nome))
        {
            printf("Errore: lezione non consentita, riprova.\n");
            continue;
        }

        break; 
    } while (1);

    do
    {
        printf("Inserisci l'orario della lezione (HH:MM): ");
        if (scanf("%d:%d", &ore, &min) != 2)
        {
            printf("Formato orario non valido. Riprova.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n'); 

        orario = crea_orario(ore, min);
        if (orario == NULL)
        {
            printf("Orario non valido. Riprova.\n");
        }
    } while (orario == NULL);

    do
    {
        // Se la data era stata allocata nel ciclo precedente, liberala
        if (data != NULL)  
        {   
            libera_data(data);
            data = NULL;
        }

        data = leggi_data();

        if (!data_valida_per_lezione(data))
        {
            printf("Errore: la data deve essere nel mese corrente o nel mese successivo.\n");
            libera_data(data);
            data = NULL;
        }
    } while (data == NULL);

    do
    {
        printf("Inserisci il numero massimo di posti (max 30): ");
        posti_max = leggi_intero();

        if (posti_max <= 0)
        {
            printf("Errore: il numero di posti deve essere positivo. Riprova.\n");
        }
        else if (posti_max > 30)
        {
            printf("Errore: il numero massimo consentito e' 30. Riprova.\n");
        }
    } while (posti_max <= 0 || posti_max > 30);

    char* id = genera_id_generico("L", "lezioni.txt");

    Lezione nuova_lezione = crea_lezione(id, nome, data, orario, posti_max);

    l = consList(nuova_lezione, l);

    salva_lezione_file(nuova_lezione);

    printf("Lezione inserita con successo!\n");

    free(id);

    return l;
}

/*
* Funzione: rimuovi_cliente
* ----------------------------------------
* Rimuove un cliente dalla hashtable e tutte le sue prenotazioni.
*
* Parametri:
*   h: hashtable dei clienti.
*   hp: hashtable delle prenotazioni.
*
* Pre-condizione:
*   h e hp devono essere validi.
*
* Post-condizione:
*   Il cliente con l’ID specificato viene rimosso da h e da file di testo.
*   Vengono rimosse tutte le prenotazioni associate a quel cliente in hp.
*   I file di clienti e prenotazioni vengono aggiornati.
*
* Come funziona:
* - Legge l’ID cliente da input.
* - Cerca e rimuove il cliente dalla hashtable.
* - Scorre la hash table delle prenotazioni rimuovendo quelle collegate a quel cliente.
* - Aggiorna i file di clienti e prenotazioni.
*/
static void rimuovi_cliente(hashtable h, hashtable_p hp)
{
    printf("==============================================\n");
    printf("\t      RIMUOVI UN CLIENTE\n");
    printf("==============================================\n");

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

    distruggi_cliente(rem); 

    printf("Cliente rimosso!\n");
    aggiorna_file_clienti(h);

    // === Rimuovi le prenotazioni collegate a quel cliente ===
    Prenotazione* table = get_table_hash_p(hp);
    int size = get_size_hash_p(hp);

    for (int i = 0; i < size; i++)
    {
        Prenotazione prev = NULL;
        Prenotazione curr = table[i];

        while (curr != NULL)
        {
            Prenotazione next = get_next_prenotazione(curr);

            if (strcmp(get_id_cliente_prenotazione(curr), id) == 0)
            {
                if (prev == NULL)
                    table[i] = next;
                else
                    set_next_prenotazione(prev, next);

                libera_prenotazione(curr);
            }
            else
            {
                prev = curr;
            }
            curr = next;
        }
    }

    aggiorna_file_prenotazioni(hp);
}

/*
* Funzione: rimuovi_lezione
* ----------------------------------------
* Rimuove una lezione dalla lista e tutte le prenotazioni associate ad essa.
*
* Parametri:
*   l: lista delle lezioni.
*   hp: hashtable delle prenotazioni.
*
* Pre-condizione:
*   l e hp devono essere validi.
*
* Post-condizione:
*   La lezione con l’ID specificato viene rimossa dalla lista e dal file.
*   Vengono rimosse tutte le prenotazioni associate a quella lezione.
*   I file delle lezioni e prenotazioni vengono aggiornati.
*
* Come funziona:
* - Legge l’ID lezione da input.
* - Rimuove la lezione dalla lista, liberando la memoria.
* - Scorre la tabella delle prenotazioni eliminando quelle collegate alla lezione.
* - Aggiorna i file di lezioni e prenotazioni.
*/
static list rimuovi_lezione(list l, hashtable_p hp)
{
    printf("======================================================\n");
    printf("\t            RIMUOVI UNA LEZIONE\n");
    printf("======================================================\n");

    if (emptyList(l))
    {
        printf("Nessuna lista per le lezioni trovata.\n");
        return l;
    }

    char id[20];

    printf("Inserisci l'ID della lezione da rimuovere: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    list app = newList();
    Lezione x;
    int trovata = 0;

    while(!emptyList(l))
    {
        x = getFirst(l);
        if (strcmp(get_id_lezione(x), id) == 0)
        {
            libera_lezione(x);
            trovata = 1;
        }
        else
        {
            app = consList(x, app);
        }
        l = tailList(l);
    }

    if (!trovata)
    {
        printf("Lezione con ID %s non trovata.\n", id);
        return app;
    }

    list nuova = reverseList(app); // Ripristina l'ordine
    printf("Lezione rimossa!\n");

    aggiorna_file_lezioni(nuova);

    // === Rimuovi le prenotazioni collegate a quella lezione ===
    Prenotazione* table = get_table_hash_p(hp);
    int size = get_size_hash_p(hp);

    for (int i = 0; i < size; i++)
    {
        Prenotazione prev = NULL;
        Prenotazione curr = table[i];

        while (curr != NULL)
        {
            //Salva in anticipo il prossimo nodo
            Prenotazione next = get_next_prenotazione(curr); 

            if (strcmp(get_id_lezione_prenotazione(curr), id) == 0)
            {
                //Se è il primo nodo, la lista deve ora partire dal successivo 
                if (prev == NULL)
                    table[i] = next;
                else //Altrimenti, salta il nodo corrente collegando prev → next
                    set_next_prenotazione(prev, next);

                libera_prenotazione(curr);
            }
            else
            {
                prev = curr;
            }
            curr = next;
        }
    }

    aggiorna_file_prenotazioni(hp);

    return nuova;
}

/*
* Funzione: ricerca_cliente
* ----------------------------------------
* Permette di cercare un cliente nella hashtable tramite il suo ID.
*
* Parametri:
*   h: hashtable dei clienti.
*
* Pre-condizione:
*   h deve essere una hashtable valida.
*
* Post-condizione:
*   Visualizza i dati del cliente se trovato, altrimenti mostra un messaggio di errore.
*
* Come funziona:
* - Legge l’ID cliente da input.
* - Cerca il cliente nella hashtable.
* - Se trovato, mostra i dati del cliente.
*/
static void ricerca_cliente(hashtable h)
{
    printf("==============================================\n");
    printf("\t      RICERCA UN CLIENTE\n");
    printf("==============================================\n");

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
        printf("Cliente con ID %s non trovato.\n", id);
        return;
    }

    printf("Cliente trovato:\n");
    visualizza_cliente(ricercato);
}

/*
* Funzione: visualizza_prenotazioni_cliente
* ----------------------------------------
* Visualizza tutte le prenotazioni associate a un cliente.
*
* Parametri:
*   h: hashtable dei clienti.
*   hp: hashtable delle prenotazioni.
*   l: lista delle lezioni.
*
* Pre-condizione:
*   h, hp e l devono essere validi.
*
* Post-condizione:
*   Mostra le prenotazioni attive per il cliente.
*
* Come funziona:
* - Legge l’ID cliente da input.
* - Verifica se il cliente esiste nella hashtable.
* - Visualizza tutte le prenotazioni di quel cliente.
*/
static void visualizza_prenotazioni_cliente(hashtable h, hashtable_p hp, list l)
{
    printf("------------------------------------------------------\n");
    char id[20];
    printf("Inserisci l'ID del cliente: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    Cliente c = hashSearch(h, id);
    if (c == NULL)
    {
        printf("Cliente con ID %s non trovato.\n", id);
        return;
    }

    pulisci_schermo();

    stampa_prenotazioni_cliente(c, hp, l);
}

/*
* Funzione: visualizza_prenotazioni_lezione
* ----------------------------------------
* Permette di visualizzare tutte le prenotazioni associate a una specifica lezione.
*
* Parametri:
*   h: hashtable dei clienti.
*   hp: hashtable delle prenotazioni.
*   l: lista delle lezioni.
*
* Pre-condizione:
*   h, hp e l devono essere validi.
*
* Post-condizione:
*   Vengono stampate tutte le prenotazioni relative alla lezione specificata dall’utente.
*
* Come funziona:
* - Chiede all’utente di inserire l’ID della lezione.
* - Cerca la lezione nella lista.
* - Se la lezione esiste, pulisce lo schermo e stampa le prenotazioni associate.
* - Se la lezione non esiste, mostra un messaggio di errore.
*/
static void visualizza_prenotazioni_lezione(hashtable h, hashtable_p hp, list l)
{
    if (h == NULL || hp == NULL || l == NULL) 
        return;

    printf("------------------------------------------------------\n");
    char id_lezione[20];
    printf("Inserisci l'ID della lezione: ");
    fgets(id_lezione, sizeof(id_lezione), stdin);
    id_lezione[strcspn(id_lezione, "\n")] = '\0';

    Lezione lezione_trovata = NULL;
    list temp = l;
    while (!emptyList(temp))
    {
        Lezione le = getFirst(temp);
        if (strcmp(get_id_lezione(le), id_lezione) == 0)
        {
            lezione_trovata = le;
            break;
        }
        temp = tailList(temp);
    }

    if (lezione_trovata == NULL)
    {
        printf("Lezione con ID '%s' non trovata.\n", id_lezione);
        return;
    }

    pulisci_schermo();

    stampa_prenotazioni_lezione(h, lezione_trovata, hp);
}

/*
* Funzione: menu_gestore
* ----------------------------------------
* Gestisce il menu interattivo per l’area gestore, con tutte le operazioni disponibili.
*
* Parametri:
*   h: hashtable dei clienti.
*   l: lista delle lezioni.
*   hp: hashtable delle prenotazioni.
*
* Pre-condizione:
*   h, l e hp devono essere validi e inizializzati.
*
* Post-condizione:
*   Esegue l’azione selezionata dall’utente modificando le strutture dati e
*   i file di testo di conseguenza.
*
* Come funziona:
* - Mostra un menu con le opzioni disponibili.
* - Legge la scelta dell’utente.
* - Esegue la funzione corrispondente alla scelta.
* - Dopo ogni operazione, attende che l’utente prema INVIO prima di tornare al menu.
* - Continua finché l’utente non sceglie di uscire.
*/
void menu_gestore(hashtable h, list l, hashtable_p hp) 
{
    int scelta;

    do
    {
        pulisci_schermo();
        printf("==============================================\n");
        printf("\tBENVENUTO NELLA AREA GESTORE!\n");
        printf("==============================================\n");
        printf("1) Inserisci un nuovo cliente\n");
        printf("2) Inserisci una nuova lezione\n");
        printf("3) Visualizza la lista dei clienti\n");
        printf("4) Rinnova l'abbonamento di un cliente\n");
        printf("5) Visualizza la lista delle lezioni\n");
        printf("6) Rimuovi un cliente\n");
        printf("7) Rimuovi una lezione\n");
        printf("8) Ricerca un cliente\n");
        printf("9) Visualizza le prenotazioni di un cliente\n");
        printf("10) Visualizza le prenotazioni di una lezione\n");
        printf("11) Genera il report mensile\n");
        printf("0) Esci\n");
        printf("==============================================\n");
        printf("Scegli un'opzione: ");
        scelta = leggi_intero();

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
                printf("===================================\n");
                printf("         Elenco Clienti\n");
                stampaHash(h);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 4:
                pulisci_schermo();
                printf("==============================================\n");
                printf("\t      LISTA DEI CLIENTI\n");
                printf("==============================================\n");

                stampaMinimaHash(h);
                printf("\nInserisci l'ID del cliente a cui rinnovare: ");
                char id_cliente[20];
                fgets(id_cliente, sizeof(id_cliente), stdin);
                id_cliente[strcspn(id_cliente, "\n")] = '\0';

                Cliente c = hashSearch(h, id_cliente);
                if (c == NULL)
                {
                    printf("Cliente con ID %s non trovato.\n", id_cliente);
                }
                else
                {   
                    rinnova_abbonamento(c, h);
                    printf("Abbonamento rinnovato con successo per %s.\n", get_nome_cliente(c));
                }

                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 5:
                pulisci_schermo();
                outputList(l);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 6:
                pulisci_schermo();
                printf("==============================================\n");
                printf("\t      LISTA DEI CLIENTI\n");
                printf("==============================================\n");

                stampaMinimaHash(h);
                rimuovi_cliente(h, hp);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 7:
                pulisci_schermo();
                printf("======================================================\n");
                printf("\t            LISTA DELLE LEZIONI\n");
                printf("======================================================\n");

                stampaMinimaList(l);
                l = rimuovi_lezione(l, hp);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 8:
                pulisci_schermo();
                ricerca_cliente(h);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            case 9:
                pulisci_schermo();
                printf("=====================================================\n");
                printf("\t      VISUALIZZA PRENOTAZIONI\n");
                printf("=====================================================\n");
                
                stampaMinimaHash(h);
                visualizza_prenotazioni_cliente(h, hp, l);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;

            case 10:
                pulisci_schermo();
                printf("=====================================================\n");
                printf("\t      VISUALIZZA PRENOTAZIONI\n");
                printf("=====================================================\n");

                stampaMinimaList(l);
                visualizza_prenotazioni_lezione(h, hp, l);
                printf("\nPremi INVIO per tornare al menu...");
                while (getchar() != '\n');
                break;
            
            case 11:
                pulisci_schermo();
                genera_report_mensile(hp, l);
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
