//FILE menu_testing.c

#include"hash.h"
#include"hash_prenotazioni.h"
#include"lista_lezioni.h"
#include"stdlib.h"
#include"stdio.h"
#include"string.h"
#include"area_clienti.h"
#include"prenotazione.h"
#include"utils.h"

/*
* Funzione: salva_prenotazione_test
* ----------------------------------------
* Salva una prenotazione su un file di output associato al caso di test.
*
* Parametri:
*   p: puntatore alla prenotazione da salvare.
*   po: numero di posti occupati aggiornato.
*   ncase: numero del caso di test.
*
* Pre-condizione:
*   p deve essere un puntatore valido a una prenotazione.
*   ncase deve corrispondere a un test esistente.
*
* Post-condizione:
*   I dati della prenotazione sono salvati nel file "1_output_test_<ncase>.txt".
*
* Come funziona:
* - Costruisce dinamicamente il nome del file in base al numero di test.
* - Apre il file in modalità scrittura.
* - Scrive i dettagli della prenotazione e i posti occupati.
* - Chiude il file.
*/
void salva_prenotazione_test(Prenotazione p, int po,int ncase)
{
    char output_fname[256];
   
    sprintf(output_fname, "1_output_test_%d.txt", ncase);

    FILE *fp = fopen(output_fname, "w");
    if (fp == NULL)
    {
        printf("Errore nell'aprire il file output_test_1.txt.\n");
        return;
    }


    fprintf(fp, "ID: %s\n", get_id_prenotazione(p));
    fprintf(fp, "ID Cliente: %s\n", get_id_cliente_prenotazione(p));
    fprintf(fp, "ID Lezione: %s\n", get_id_lezione_prenotazione(p));
    fprintf(fp, "Data Prenotazione: %02d/%02d/%04d\n",
            get_giorno(get_data_prenotazione(p)),
            get_mese(get_data_prenotazione(p)),
            get_anno(get_data_prenotazione(p)));

    fprintf(fp, "-----------------------\n");


    fprintf(fp, "Posti occupati: %d", po);
    fclose(fp);
}

/*
* Funzione: prenota_lezione_test_1
* -----------------------------------
* Permette a un cliente di prenotare una lezione in un ambiente di test.
*
* Parametri:
*   c: cliente autenticato che vuole prenotare.
*   l: lista delle lezioni disponibili.
*   hp: hashtable delle prenotazioni.
*   id_lezione_scelta: ID della lezione da prenotare.
*   ncase: numero del caso di test.
*
* Pre-condizione:
*   Il cliente deve esistere ed essere autenticato.
*   Deve avere un abbonamento valido.
*   La lista delle lezioni e la hashtable devono essere inizializzate.
*
* Post-condizione:
*   Se la prenotazione è possibile, viene effettuata e salvata su file.
*   Al termine del test, lo stato viene ripristinato.
*
* Come funziona:
* - Verifica la validità dell’abbonamento e dell’ID lezione.
* - Controlla se ci sono posti disponibili.
* - Evita duplicati di prenotazione.
* - Genera un ID per la prenotazione.
* - Inserisce temporaneamente la prenotazione nella hashtable.
* - Salva i dettagli su file e rimuove la prenotazione.
*/
int prenota_lezione_test_1(Cliente c, list l, hashtable_p hp, char *id_lezione_scelta,int ncase)
{
    if(c == NULL)
    {
        return 0;
    }

    if(!abbonamento_valido(data_oggi(), get_data_scadenza(c)))
    {
        return 0;
    }

    list tmp = l;
    int trovate = 0;

    if(emptyList(l))
    {
        return 0;
    }

    // Cerca la lezione nella lista
    Lezione l_selezionata = cerca_lezione_per_id(l, id_lezione_scelta);
    if (l_selezionata == NULL)
    {
        return 0;
    }

    if (confronta_date(get_data_lezione(l_selezionata), get_data_scadenza(c)) > 0)
    {
        return 0;
    }
    else if (confronta_date(data_oggi(), get_data_lezione(l_selezionata)) > 0)
    {
        return 0;
    }

    int posti_occupati = get_posti_occupati(l_selezionata);
    int posti_max = get_posti_max(l_selezionata);
    int posti_disponibili = posti_max - posti_occupati;

    if (posti_disponibili <= 0)
    {
        return 0;
    }

    // Verifica se il cliente è già prenotato alla lezione
    Prenotazione* table = get_table_hash_p(hp);
    for (int i = 0; i < get_size_hash_p(hp); i++)
    {
        Prenotazione curr = table[i];
        while (curr != NULL)
        {
            if (strcmp(get_id_cliente_prenotazione(curr), get_id_cliente(c)) == 0 &&
            strcmp(get_id_lezione_prenotazione(curr), get_id_lezione(l_selezionata)) == 0)
            {
                return 0;
            }
            curr = get_next_prenotazione(curr);
        }
    }

    set_posti_occupati(l_selezionata, posti_occupati + 1);

    Data data_pre = data_oggi();

    char *id_prenotazione = genera_id_generico("P", "prenotazioni.txt",1);
    Prenotazione p = crea_prenotazione(id_prenotazione, get_id_cliente(c), get_id_lezione(l_selezionata), data_pre);
    free(id_prenotazione);

    insertHash_p(hp, p);

    int posti_occupati_nuovo = get_posti_occupati(l_selezionata);
    salva_prenotazione_test(p, posti_occupati_nuovo,ncase);

    set_posti_occupati(l_selezionata, posti_occupati_nuovo - 1);

    hashDelete_p(hp, get_id_prenotazione(p));

    return 1;
}

/*
* Funzione: confronta_file
* ----------------------------------------
* Confronta due file riga per riga per verificarne l’identicità.
*
* Parametri:
*   oracle: percorso del file oracle.
*   output: percorso del file output (generato).
*
* Pre-condizione:
*   I file devono esistere e contenere lo stesso numero di righe significative.
*
* Post-condizione:
*   Ritorna 1 se i file sono identici, altrimenti 0.
*
* Come funziona:
* - Apre entrambi i file in lettura.
* - Confronta ogni riga.
* - Se trova una differenza, ritorna 0.
* - Se tutti i confronti sono uguali, ritorna 1.
*/
int confronta_file(char *oracle, char *output)
{
    FILE *fp_oracle, *fp_output;
 
    if((fp_oracle = fopen(oracle, "r")) == NULL)
    {
        printf("Errore nell'apertura del file di oracle\n");
        exit(EXIT_FAILURE);
    }

    if((fp_output = fopen(output, "r")) == NULL)
    {
        printf("Errore nell'apertura del file di output\n");
        exit(EXIT_FAILURE);
    }

    char buffer[256], buffer2[256];
    while (fgets(buffer, sizeof(buffer), fp_oracle) && fgets(buffer2, sizeof(buffer2), fp_output))
    {
        if(strcmp(buffer,buffer2)!=0)
        {
            return 0;
        }
    }

    fclose(fp_oracle);
    fclose(fp_output);

    return 1;  
}

/*
* Funzione: test_prenotazione_1
* ----------------------------------------
* Esegue un caso di test sulla prenotazione delle lezioni.
*
* Parametri:
*   h: hashtable dei clienti.
*   l: lista delle lezioni.
*   hp: hashtable delle prenotazioni.
*   ncase: numero del caso di test da eseguire.
*
* Pre-condizione:
*   Le strutture h, l e hp devono essere inizializzate.
*   I file di input e oracle devono essere presenti.
*
* Post-condizione:
*   Esegue il test specificato e scrive PASS/FAIL in un file di result.
*
* Come funziona:
* - Costruisce i nomi dei file in base al numero di test.
* - Legge i dati di input dal file.
* - Esegue la funzione di prenotazione test.
* - Confronta l’output generato con l’oracle atteso.
* - Scrive il risultato nel file di test result.
*/
void test_prenotazione_1(hashtable h, list l, hashtable_p hp, int ncase)
{
    FILE *fp_input,*result,*fp_output;
    char input_fname[256],result_fname[256],output_fname[256],oracle_fname[256];
    sprintf(input_fname, "1_input_test_%d.txt", ncase);

    sprintf(result_fname, "1_result_test_%d.txt", ncase);

    sprintf(oracle_fname, "1_oracle_test_%d.txt", ncase);

    sprintf(output_fname, "1_output_test_%d.txt", ncase);


    if((fp_input = fopen(input_fname, "r")) == NULL)
    {
        printf("Errore nell'apertura del file di input\n");
        exit(EXIT_FAILURE);
    }

    if((result = fopen(result_fname, "w")) == NULL)
    {
        printf("Errore nell'apertura del file di result\n");
        exit(EXIT_FAILURE);
    }
    char buffer[256];
    char id_cliente[20], id_lezione[20];
    int k,z;

    while(fgets(buffer, sizeof(buffer), fp_input))
    {
        sscanf(buffer, "%s %s", id_cliente, id_lezione);
        Cliente c = hashSearch(h, id_cliente);

        k = prenota_lezione_test_1(c, l, hp, id_lezione, ncase);
    
        if(k==0)
            fp_output = fopen(output_fname, "w");  

        z = confronta_file(oracle_fname, output_fname);

        if(z == 1)
            fprintf(result,"PASS\n");
        else
            fprintf(result,"FAIL\n");   
    }

    fclose(result);
    fclose(fp_input);
    
}

/*
* Funzione: menu_testing
* ----------------------------------------
* Mostra un menu di test automatici per il sistema.
*
* Parametri:
*   h: hashtable dei clienti.
*   l: lista delle lezioni.
*   hp: hashtable delle prenotazioni.
*
* Pre-condizione:
*   Le strutture devono essere inizializzate.
*
* Post-condizione:
*   L’utente può selezionare un test da eseguire.
*
* Come funziona:
* - Mostra un menu.
* - L’utente seleziona un'opzione tra i test disponibili.
* - Esegue il test corrispondente.
* - Ritorna al menu finché l’utente non sceglie di uscire.
*/
void menu_testing(hashtable h, list l, hashtable_p hp)
{
    int sc, ncase;

    do
    {
        pulisci_schermo();
        printf("==============================================\n");
        printf("\tBENVENUTO NELLA AREA TESTING !\n");
        printf("==============================================\n");
        printf("1) Per verificare la corretta registrazione delle\n"
        "prenotazioni e dell'aggiornamento delle disponibilita'\n\n");
        printf("2) Per verificare la gestione degli abbonamenti e\n"
        "della loro validita'\n\n");
        printf("3) Per verificare che il report generato, contenga\n"
        "informazioni corrette\n\n");
        printf("0) Per uscire\n");
        printf("Scelta: ");
        sc = leggi_intero();

        switch (sc) 
        {
            case 1:
            {
                pulisci_schermo();
                printf("==============================================\n");
                printf("\t\tTEST NUMERO 1 !\n");
                printf("==============================================\n");
                printf("1) Testa un cliente che puo' prenotarsi\n");
                printf("2) Testa un cliente che gia' e' prenotato alla lezione\n");
                printf("3) Testa il caso in cui il cliente non esiste\n");
                do
                {   
                    printf("Scelta: ");
                    ncase = leggi_intero();
                } while(ncase<1 || ncase >3);

                test_prenotazione_1(h, l, hp,ncase);

                printf("\nTest generato, controlla il file result. Premi invio...");
                while (getchar() != '\n');
                break;
            }
            case 2:
            {
                pulisci_schermo();
                
                break;
            }
            case 3:
            {
                pulisci_schermo();
            
                break;
            }
            case 0:
            {
                
                break;
            }
            default:
                printf("Opzione non valida. Riprova.\n");
                printf("\nPremi INVIO...");
                while (getchar() != '\n');
                break;
        }
    } while (sc != 0);
}