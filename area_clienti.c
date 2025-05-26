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

/* 
* Funzione: login_cliente
* -----------------------
* Richiede all'utente di inserire l'ID cliente tramite input da tastiera.
* Cerca il cliente nella tabella hash dei clienti.
* Se il cliente non esiste, informa l'utente e ritorna NULL.
* Se il cliente esiste ma l'abbonamento è scaduto, offre la possibilità di rinnovarlo.
* Ritorna il cliente autenticato in caso di successo.
* 
* Parametri:
*   h: hashtable contenente i dati dei clienti registrati.
*
* Pre-condizione:
*   La tabella hash 'h' deve essere inizializzata e contenere eventuali clienti.
*
* Post-condizione:
*   Viene restituito un oggetto Cliente corrispondente alle credenziali inserite,
*   se il login ha successo.
*
* Ritorna:
*   Il cliente autenticato (di tipo Cliente).
* 
* Come funziona:
* - Legge l'input dall'utente.
* - Usa hashSearch per trovare il cliente.
* - Controlla la validità dell'abbonamento con abbonamento_valido.
* - Gestisce l'opzione di rinnovo richiamando rinnova_abbonamento.
* - Attende un invio per tornare al menu.
*/
Cliente login_cliente(hashtable h)
{
    char id[20];
    int sc;

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
        printf("Attenzione, abbonamento scaduto!\n");
        
        printf("\nPremi INVIO per amdare avanti...");
        while (getchar() != '\n');
    }

    return c;
}

/*
* Funzione: visualizza_lezioni 
* --------------------------------------
* Mostra la lista delle lezioni in base a criteri scelti dall’utente.
*
* Parametri:
*   l: lista delle lezioni disponibili.
*
* Pre-condizione:
*   La lista 'l' deve essere inizializzata (anche vuota).
*
* Post-condizione:
*   Vengono stampate a video le lezioni che corrispondono al filtro scelto.
*
* Come funziona:
* - Chiede se visualizzare tutte o solo le lezioni disponibili (non passate e con posti liberi).
* - Scorre la lista controllando data e disponibilità.
* - Visualizza solo le lezioni che soddisfano il filtro.
* - Gestisce il caso in cui non ci siano lezioni da mostrare.
*/
static void visualizza_lezioni(list l)
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
    printf("2) Solo lezioni disponibili (non passate e con posti liberi)\n");
    printf("0) Torna indietro\n");
    printf("Scelta: ");
    scelta = leggi_intero();

    if(scelta == 0)
    {
        return;
    }

    list tmp = l;
    int trovate = 0;

    Data oggi_data = data_oggi(); 

    while (!emptyList(tmp))
    {
        Lezione le = getFirst(tmp);
        int posti_disponibili = get_posti_max(le) - get_posti_occupati(le);

        int mostra = 0;

        if (scelta == 1)
        {
            mostra = 1;
        }
        else if (scelta == 2)
        {
            Data data_le = get_data_lezione(le);
            if (posti_disponibili > 0 && confronta_date(data_le, oggi_data) >= 0)
            {
                mostra = 1;
            }
        }

        if (mostra)
        {
            visualizza_lezione(le);
            trovate++;
        }

        tmp = tailList(tmp);
    }

    libera_data(oggi_data);

    if (trovate == 0)
    {
        printf("Nessuna lezione trovata con i criteri selezionati.\n");
    }
}

/*
* Funzione: prenota_lezione 
* -----------------------------------
* Permette a un cliente di prenotare una lezione.
*
* Parametri:
*   c: cliente autenticato che vuole prenotare.
*   l: lista delle lezioni disponibili.
*   h: hashtable delle prenotazioni.
*
* Pre-condizione:
*   Il cliente deve avere un abbonamento valido.
*   l e h devono essere valide.
*
* Post-condizione:
*   Viene creata una nuova prenotazione se possibile.
*   Vengono aggiornati i posti occupati della lezione 
*   e aggiorna il file di testo con tutte le lezioni aggiornate.
*
* Come funziona:
* - Mostra la lista delle lezioni libere.
* - Legge l’ID lezione da prenotare.
* - Verifica l’esistenza, disponibilità e validità di prenotazione.
* - Controlla che il cliente non sia già prenotato.
* - Incrementa i posti occupati.
* - Crea una nuova prenotazione e la inserisce nella hashtable e nel file di testo.
*/
static void prenota_lezione(Cliente c, list l, hashtable_p h)
{
    if(!abbonamento_valido(data_oggi(), get_data_scadenza(c)))
    {
        printf("===========================================================\n");
        printf("Abbonamento scaduto, rinnovalo per effettuare prenotazioni!\n");
        printf("===========================================================\n");
        return;
    }

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

    if (confronta_date(get_data_lezione(l_selezionata), get_data_scadenza(c)) > 0)
    {
        printf("Impossibile prenotare: la lezione si svolge dopo la scadenza del tuo abbonamento.\n");
        return;
    }
    else if (confronta_date(data_oggi(), get_data_lezione(l_selezionata)) > 0)
    {
        printf("Impossibile prenotare: la lezione gia' si e' svolta.\n");
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

    set_posti_occupati(l_selezionata, posti_occupati + 1);
    aggiorna_file_lezioni(l);

    Data data_pre = data_oggi();


    char *id_prenotazione = genera_id_generico("P", "prenotazioni.txt");
    Prenotazione p = crea_prenotazione(id_prenotazione, get_id_cliente(c), get_id_lezione(l_selezionata), data_pre);
    free(id_prenotazione);

    insertHash_p(h, p);

    salva_prenotazione_file(p);

    printf("Prenotazione effettuata con successo!\n");
}

/*
* Funzione: disdici_prenotazione  
* ----------------------------------------
* Permette a un cliente di disdire una prenotazione.
*
* Parametri:
*   c: cliente autenticato.
*   hp: hashtable delle prenotazioni.
*   l: lista delle lezioni.
*
* Pre-condizione:
*   c, hp e l devo essere validi.
*
* Post-condizione:
*   La prenotazione viene rimossa dalla struttura dati e dal file di testo.
*   Viene aggiornato il numero di posti occupati nella lezione.
*
* Come funziona:
* - Mostra le prenotazioni attive del cliente.
* - Legge l’ID prenotazione da disdire.
* - Verifica che la prenotazione appartenga al cliente.
* - Rimuove la prenotazione dalla hashtable.
* - Decrementa i posti occupati della lezione corrispondente.
* - Aggiorna il file di testo.
*/
static void disdici_prenotazione(Cliente c, hashtable_p hp, list l)
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

    Prenotazione rem = hashSearch_p(hp, id_prenotazione_scelta);

    if(rem == NULL)
    {
        printf("Prenotazione con ID %s non trovato.\n", id_prenotazione_scelta);
        return;
    }

    // Controlla che la prenotazione appartenga al cliente
    if (strcmp(get_id_cliente_prenotazione(rem), get_id_cliente(c)) != 0)
    {
        printf("Errore: non puoi disdire una prenotazione che non ti appartiene.\n");
        return;
    }

    rem = hashDelete_p(hp, id_prenotazione_scelta);

    char* id_lezione = get_id_lezione_prenotazione(rem);

    // Trova la lezione nella lista e decrementa i posti
    list temp = l;
    while (!emptyList(temp))
    {
        Lezione le = getFirst(temp);
        if (strcmp(get_id_lezione(le), id_lezione) == 0)
        {
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

/*
* Funzione: menu_cliente
* ----------------------
* Gestisce il menu interattivo riservato a un cliente autenticato.
*
* Parametri:
*   c: cliente autenticato.
*   h: hashtable dei clienti.
*   l: lista delle lezioni disponibili.
*   hp: hashtable delle prenotazioni.
*
* Pre-condizione:
*   Il cliente deve essere autenticato.
*   Le strutture dati devono essere correttamente inizializzate.
*
* Post-condizione:
*   Aggiorna eventuali modifiche effettuate dal cliente (es. prenotazioni).
*
* Come funziona:
* - Mostra un menu con varie opzioni:
*    visualizza abbonamento, rinnova, prenota lezione, disdici prenotazione, 
*    visualizza lezioni, visualizza le tue prenotazioni.
* - Legge l’input dell’utente.
* - Richiama le funzioni appropriate in base alla scelta.
* - Continua finché l’utente non sceglie di uscire.
*/
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
                if((abbonamento_valido(data_oggi(), get_data_scadenza(c))))
                    printf("Stato abbonamento: Valido\n");
                else
                    printf("Stato abbonamento: Scaduto\n");
                
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
