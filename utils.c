//FILE utils.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "data.h"
#include "hash.h"
#include "cliente.h"
#include "lezione.h"
#include "lista_lezioni.h"
#include "orario.h"
#include "hash_prenotazioni.h"
#include "prenotazione.h"

/*
* Funzione: rinnova_abbonamento
* ----------------------------------------
* Rinnova l’abbonamento di un cliente per un numero specificato di mesi.
*
* Parametri:
*   c: puntatore al cliente da rinnovare.
*   h: hashtable contenente i clienti.
*
* Pre-condizione:
*   c e h devono essere diversi da NULL.
*
* Post-condizione:
*   L'abbonamento del cliente viene esteso, aggiornato nei dati e salvato su file.
*
* Come funziona:
* - Controlla la validità dei parametri.
* - Chiede all’utente la durata del rinnovo in mesi.
* - Calcola la nuova data di scadenza e aggiorna il cliente.
* - Ricalcola la durata dell'abbonamento.
* - Aggiorna il file con i dati modificati.
*/
void rinnova_abbonamento(Cliente c, hashtable h)
{
    if (c == NULL || h == NULL)
    {
        printf("Cliente o tabella non valida.\n");
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

    Data nuova_data_scadenza = calcolo_scadenza_abbonamento(data_scadenza_attuale, durata);
    set_data_scadenza(c, nuova_data_scadenza);

    // Calcola la durata effettiva dell'abbonamento (differenza tra data di scadenza e data di iscrizione)
    int durata_effettiva = calcola_durata_in_mesi(get_data_iscrizione(c), get_data_scadenza(c));    
    set_durata(c, durata_effettiva);

    aggiorna_file_clienti(h);

    printf("Il tuo abbonamento e' stato rinnovato fino al ");
    visualizza_data(nuova_data_scadenza);
    printf("\n");
}

/*
* Funzione: carica_contatore_generico
* ----------------------------------------
* Carica il valore massimo di un ID generico da un file, basandosi su un prefisso.
*
* Parametri:
*   filename: nome del file da cui leggere gli ID.
*   prefix: prefisso che identifica il tipo di ID (es. "ID: C").
*
* Pre-condizione:
*   filename e prefix devono essere validi e non NULL.
*
* Post-condizione:
*   Restituisce il valore massimo trovato per il prefisso specificato.
*
* Come funziona:
* - Apre il file e cerca righe che iniziano con il prefisso.
* - Estrae la parte numerica e aggiorna il contatore massimo.
* - Ritorna il valore massimo trovato.
*/
int carica_contatore_generico(const char* filename, const char* prefix)
{
    FILE* file = fopen(filename, "r");
    int max_id = 0;

    if (file == NULL)
    {
        printf("File '%s' non trovato. Inizializzo il contatore a 0.\n", filename);
        return 0;
    }

    char line[256];
    int prefix_len = strlen(prefix);
    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, prefix, prefix_len) == 0)
        {
            int current_id = atoi(line + prefix_len);
            if (current_id > max_id)
                max_id = current_id;
        }
    }

    fclose(file);
    return max_id;
}

/*
* Funzione: genera_id_generico
* ----------------------------------------
* Genera un nuovo ID univoco basato su un prefisso e un file di contatori.
*
* Parametri:
*   prefix_letter: carattere identificativo dell'entità (es. "C" per Cliente).
*   filename: nome del file da cui leggere il contatore esistente.
*
* Pre-condizione:
*   prefix_letter deve essere "C", "L" o "P".
*
* Post-condizione:
*   Ritorna una nuova stringa ID allocata dinamicamente.
*
* Come funziona:
* - Identifica quale contatore usare in base al prefisso.
* - Se non inizializzato, lo carica da file.
* - Incrementa il contatore e genera l’ID nel formato 'Prefisso + numero'.
*/
char* genera_id_generico(const char* prefix_letter, const char* filename)
{
    static int contatore_clienti = -1;
    static int contatore_lezioni = -1;
    static int contatore_prenotazioni = -1;
    int* counter = NULL;

    if (strcmp(prefix_letter, "C") == 0)
        counter = &contatore_clienti;
    else if (strcmp(prefix_letter, "L") == 0)
        counter = &contatore_lezioni;
    else if (strcmp(prefix_letter, "P") == 0)
        counter = &contatore_prenotazioni;
    else
    {
        printf("Prefisso non riconosciuto.\n");
        exit(1);
    }

    char prefix[10];
    sprintf(prefix, "ID: %s", prefix_letter);

    if (*counter == -1)
        *counter = carica_contatore_generico(filename, prefix);

    (*counter)++;

    char* id = malloc(10 * sizeof(char));
    if (id == NULL)
    {
        printf("Errore di allocazione memoria per l'ID.\n");
        exit(1);
    }

    sprintf(id, "%s%03d", prefix_letter, *counter);
    return id;
}

/*
* Funzione: carica_clienti_da_file
* ----------------------------------------
* Carica i dati dei clienti da un file e li inserisce nella hashtable.
*
* Parametri:
*   h: hashtable in cui inserire i clienti caricati.
*
* Pre-condizione:
*   Il file "clienti.txt" deve essere presente e formattato correttamente.
*
* Post-condizione:
*   Tutti i clienti vengono caricati e inseriti nella hashtable.
*
* Come funziona:
* - Apre il file e legge i dati dei clienti.
* - Crea le strutture cliente e le inserisce nella tabella hash.
*/
void carica_clienti_da_file(hashtable h)
{
    FILE *fp = fopen("clienti.txt", "r");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file clienti.txt\n");
        return;
    }

    char id[20], nome[50], cognome[50];
    int durata, giorno, mese, anno, gs, ms, as;

    while (fscanf(fp, "ID: %s\n", id) == 1)
    {
        fscanf(fp, "Nome: %s\n", nome);
        fscanf(fp, "Cognome: %s\n", cognome);
        fscanf(fp, "Durata abbonamento: %d\n", &durata);
        fscanf(fp, "Data d'iscrizione: %d/%d/%d\n", &giorno, &mese, &anno);
        fscanf(fp, "Data scadenza: %d/%d/%d\n", &gs, &ms, &as);
        fscanf(fp, "-----------------------\n");

        Data data_is = crea_data(giorno, mese, anno);
        Data data_sca =  crea_data(gs, ms, as);

        Cliente c = crea_cliente(id, nome, cognome, durata, data_is);
        set_data_scadenza(c, data_sca);
        insertHash(h, c);
    }

    fclose(fp);
}

/*
* Funzione: carica_prenotazioni_da_file
* ----------------------------------------
* Carica tutte le prenotazioni dal file e le inserisce nella hashtable delle prenotazioni.
*
* Parametri:
*   h: hashtable delle prenotazioni.
*
* Pre-condizione:
*   Il file "prenotazioni.txt" deve essere presente e formattato correttamente.
*
* Post-condizione:
*   Tutte le prenotazioni vengono inserite nella tabella.
*
* Come funziona:
* - Apre il file e legge le informazioni delle prenotazioni.
* - Crea le strutture prenotazione e le inserisce nella hashtable.
*/
void carica_prenotazioni_da_file(hashtable_p h)
{
    FILE *fp = fopen("prenotazioni.txt", "r");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file prenotazioni.txt\n");
        return;
    }

    char id_pren[20], id_cli[50], id_lez[50];
    int giorno, mese, anno;

    while (fscanf(fp, "ID: %s\n", id_pren) == 1)
    {
        fscanf(fp, "ID Cliente: %s\n", id_cli);
        fscanf(fp, "ID Lezione: %s\n", id_lez);
        fscanf(fp, "Data Prenotazione: %d/%d/%d\n", &giorno, &mese, &anno);
        fscanf(fp, "-----------------------\n");

        Data data_pre = crea_data(giorno, mese, anno);

        Prenotazione p = crea_prenotazione(id_pren, id_cli, id_lez, data_pre);

        insertHash_p(h, p);
    }

    fclose(fp);
}

/*
* Funzione: aggiorna_file_clienti
* ----------------------------------------
* Salva tutti i clienti dalla hashtable su file.
*
* Parametri:
*   h: hashtable dei clienti.
*
* Pre-condizione:
*   h deve contenere una tabella valida con clienti.
*
* Post-condizione:
*   I dati dei clienti vengono scritti nel file "clienti.txt".
*
* Come funziona:
* - Apre il file in modalità scrittura.
* - Itera su ogni cliente della tabella hash e ne scrive i dati su file.
*/
void aggiorna_file_clienti(hashtable h)
{
    FILE *fp = fopen("clienti.txt", "w");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file clienti.txt.\n");
        return;
    }

    Cliente *table = get_table_hash(h); 
    int size = get_size_hash(h);       

    for (int i = 0; i < size; i++)
    {
        Cliente curr = table[i];
        while (curr != NULL)
        {
            fprintf(fp, "ID: %s\n", get_id_cliente(curr));
            fprintf(fp, "Nome: %s\n", get_nome_cliente(curr));
            fprintf(fp, "Cognome: %s\n", get_cognome_cliente(curr));
            fprintf(fp, "Durata abbonamento: %d\n", get_durata_abbonamento(curr));
            fprintf(fp, "Data d'iscrizione: %02d/%02d/%04d\n",
                    get_giorno(get_data_iscrizione(curr)),
                    get_mese(get_data_iscrizione(curr)),
                    get_anno(get_data_iscrizione(curr)));
            fprintf(fp, "Data scadenza: %02d/%02d/%04d\n",
                    get_giorno(get_data_scadenza(curr)),
                    get_mese(get_data_scadenza(curr)),
                    get_anno(get_data_scadenza(curr)));
            fprintf(fp, "-----------------------\n");

            curr = get_next_cliente(curr);
        }
    }

    fclose(fp);
}

/*
* Funzione: aggiorna_file_lezioni
* ----------------------------------------
* Salva la lista delle lezioni su file.
*
* Parametri:
*   l: lista delle lezioni.
*
* Pre-condizione:
*   l deve contenere lezioni valide.
*
* Post-condizione:
*   I dati delle lezioni vengono scritti nel file "lezioni.txt".
*
* Come funziona:
* - Apre il file in modalità "w".
* - Itera sulla lista e scrive le informazioni di ogni lezione.
*/
void aggiorna_file_lezioni(list l)
{
    FILE *fp = fopen("lezioni.txt", "w");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file lezioni.txt.\n");
        return;
    }

    while (!emptyList(l))
    {
        Lezione le = getFirst(l);

        fprintf(fp, "ID: %s\n", get_id_lezione(le));
        fprintf(fp, "Nome: %s\n", get_nome_lezione(le));
        fprintf(fp, "Data: %02d/%02d/%04d\n",
                get_giorno(get_data_lezione(le)), get_mese(get_data_lezione(le)), get_anno(get_data_lezione(le)));
        fprintf(fp, "Orario: %02d:%02d\n", get_ora(get_ora_lezione(le)), get_minuti(get_ora_lezione(le)));
        fprintf(fp, "Posti massimi: %d\n", get_posti_max(le));
        fprintf(fp, "Posti occupati: %d\n", get_posti_occupati(le));
        fprintf(fp, "-----------------------\n");

        l = tailList(l);
    }

    fclose(fp);
}

/*
* Funzione: aggiorna_file_prenotazioni
* ----------------------------------------
* Scrive tutte le prenotazioni presenti nella hashtable su file.
*
* Parametri:
*   h: hashtable delle prenotazioni.
*
* Pre-condizione:
*   h deve essere non NULL e contenere dati validi.
*
* Post-condizione:
*   Il file "prenotazioni.txt" viene aggiornato con tutte le prenotazioni.
*
* Come funziona:
* - Apre il file e itera su ogni prenotazione della tabella hash.
* - Scrive i dettagli su file.
*/
void aggiorna_file_prenotazioni(hashtable_p h)
{
    if (h == NULL)
        return;

    int size = get_size_hash_p(h);
    Prenotazione* table = get_table_hash_p(h);

    FILE* fp_out = fopen("prenotazioni.txt", "w");
    if (fp_out != NULL)
    {
        for (int i = 0; i < size; i++)
        {
            Prenotazione curr = table[i];
            while (curr != NULL)
            {
                Data d = get_data_prenotazione(curr);
                fprintf(fp_out, "ID: %s\n", get_id_prenotazione(curr));
                fprintf(fp_out, "ID Cliente: %s\n", get_id_cliente_prenotazione(curr));
                fprintf(fp_out, "ID Lezione: %s\n", get_id_lezione_prenotazione(curr));
                fprintf(fp_out, "Data Prenotazione: %02d/%02d/%04d\n",
                        get_giorno(d), get_mese(d), get_anno(d));
                fprintf(fp_out, "-----------------------\n");
                curr = get_next_prenotazione(curr);
            }
        }
        fclose(fp_out);
    }
    else
    {
        printf("Errore durante il salvataggio delle prenotazioni aggiornate.\n");
    }
}

/*
* Funzione: pulisci_schermo
* ----------------------------------------
* Pulisce lo schermo della console.
*
* Parametri:
*   Nessuno.
*
* Pre-condizione:
*   Nessuna.
*
* Post-condizione:
*   La console viene ripulita.
*
* Come funziona:
* - Utilizza "cls" su Windows e "clear" su altri sistemi tramite `system()`.
*/
void pulisci_schermo()
{
    #ifdef _WIN32  // Verifica se il sistema è Windows
        system("cls");
    #else  // Se il sistema è Linux o macOS
        system("clear");
    #endif
}

/*
* Funzione: carica_lezioni_da_file
* ----------------------------------------
* Carica le lezioni da file e le inserisce in una lista.
*
* Parametri:
*   l: lista iniziale (può essere NULL).
*
* Pre-condizione:
*   Il file "lezioni.txt" deve esistere e avere il formato corretto.
*
* Post-condizione:
*   La lista l viene popolata con le lezioni lette da file.
*
* Come funziona:
* - Apre il file e legge le informazioni delle lezioni.
* - Converte le stringhe lette in strutture dati e aggiorna la lista.
*/
list carica_lezioni_da_file(list l)
{
    FILE *fp = fopen("lezioni.txt", "r");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file lezioni.txt\n");
        return NULL;
    }

    char buffer[256];
    char id[20], nome[50];
    char orario_str[10];
    int ore, minuti;
    int giorno, mese, anno;
    int posti_max, posti_occupati;

    while (1)
    {
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "ID: %s", id) != 1)
            break;

        // Leggi Nome (anche con spazi)
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Nome: %[^\n]", nome) != 1)
            break;

        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Data: %d/%d/%d", &giorno, &mese, &anno) != 3)
            break;

        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Orario: %s", orario_str) != 1)
            break;

        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Posti massimi: %d", &posti_max) != 1)
            break;

        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Posti occupati: %d", &posti_occupati) != 1)
            break;

        // Leggi la linea separatrice
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;

        // Converto orario da stringa HH:MM a interi ore e minuti
        if (sscanf(orario_str, "%d:%d", &ore, &minuti) != 2)
        {
            printf("Formato orario non valido per ID %s\n", id);
            continue;
        }

        Data data = crea_data(giorno, mese, anno);
        Orario ora_lez = crea_orario(ore, minuti);

        Lezione lez = crea_lezione(id, nome, data, ora_lez, posti_max);
        set_posti_occupati(lez, posti_occupati);

        l = consList(lez, l); 
    }

    fclose(fp);
    return l;
}

/*
* Funzione: calcola_durata_in_mesi
* ----------------------------------------
* Calcola la durata in mesi tra due date.
*
* Parametri:
*   data_inizio: data iniziale.
*   data_fine: data finale.
*
* Pre-condizione:
*   Le due date devono essere valide e data_fine >= data_inizio.
*
* Post-condizione:
*   Ritorna la differenza in mesi interi tra le due date.
*
* Come funziona:
* - Calcola la differenza in anni e mesi.
* - Se la data finale è prima nel giorno rispetto a quella iniziale, sottrae un mese.
*/
int calcola_durata_in_mesi(Data data_inizio, Data data_fine)
{
    int anni_diff = get_anno(data_fine) - get_anno(data_inizio);
    int mesi_diff = get_mese(data_fine) - get_mese(data_inizio);
    int giorni_diff = get_giorno(data_fine) - get_giorno(data_inizio);

    // Calcolare la durata in mesi
    int durata_in_mesi = anni_diff * 12 + mesi_diff;
    
    // Se la differenza nei giorni è negativa (ovvero la data di fine non è ancora arrivata a completare il mese), 
    // diminuisci un mese dalla durata
    if (giorni_diff < 0)
    {
        durata_in_mesi--;
    }

    return durata_in_mesi;
}

/*
* Funzione: stampa_prenotazioni_cliente
* ----------------------------------------
* Stampa tutte le prenotazioni associate a un cliente.
*
* Parametri:
*   c: cliente di cui stampare le prenotazioni.
*   hp: hashtable delle prenotazioni.
*   l: lista delle lezioni per trovare i dettagli delle lezioni prenotate.
*
* Pre-condizione:
*   c deve essere valido, hp deve contenere prenotazioni.
*
* Post-condizione:
*   Vengono stampate le prenotazioni del cliente, se presenti.
*
* Come funziona:
* - Scorre la tabella delle prenotazioni cercando quelle con l’ID del cliente.
* - Per ogni prenotazione trovata, cerca nella lista la lezione corrispondente.
* - Stampa ID, nome della lezione, data e orario.
*/
void stampa_prenotazioni_cliente(Cliente c, hashtable_p hp, list l)
{
    printf("======================================================\n");
    printf("LISTA DELLE PRENOTAZIONI DI: %s %s!\n", get_nome_cliente(c), get_cognome_cliente(c));
    printf("======================================================\n");

    if (hp == NULL)
    {
        printf("Nessuna tabella di prenotazioni trovata.\n");
        return;
    }

    char* id_cliente = get_id_cliente(c);
    Prenotazione* table = get_table_hash_p(hp);
    int size = get_size_hash_p(hp);
    int trovata = 0;

    printf("%-10s %-20s %-16s\n", "ID", "LEZIONE", "DATA\t    ORA");
    

    for (int i = 0; i < size; i++)
    {
        Prenotazione curr = table[i];
        while (curr != NULL)
        {
            if (strcmp(get_id_cliente_prenotazione(curr), id_cliente) == 0)
            {
                trovata = 1;

                char* id_lezione = get_id_lezione_prenotazione(curr);
                char nome_lezione[50] = "Lezione non trovata";
                int giorno = 0, mese = 0, anno = 0, ore = 0, minuti = 0;

                // Cerca la lezione nella lista
                list temp = l;
                while (!emptyList(temp))
                {
                    Lezione le = getFirst(temp);
                    if (strcmp(get_id_lezione(le), id_lezione) == 0)
                    {
                        strcpy(nome_lezione, get_nome_lezione(le));
                        giorno = get_giorno(get_data_lezione(le));
                        mese = get_mese(get_data_lezione(le));
                        anno = get_anno(get_data_lezione(le));
                        ore = get_ora(get_ora_lezione(le));
                        minuti = get_minuti(get_ora_lezione(le));
                        break;
                    }
                    temp = tailList(temp);
                }

                printf("%-10s %-20s %02d/%02d/%04d %02d:%02d\n",
                       get_id_prenotazione(curr),
                       nome_lezione,
                       giorno, mese, anno,
                       ore, minuti);
            }
            curr = get_next_prenotazione(curr);
        }
    }

    if (!trovata)
        printf("Nessuna prenotazione trovata per questo cliente.\n");
}

/*
* Funzione: stampa_prenotazioni_lezione
* ----------------------------------------
* Stampa tutte le prenotazioni associate a una specifica lezione.
*
* Parametri:
*   h: hashtable dei clienti.
*   lezione: lezione di cui stampare le prenotazioni.
*   hp: hashtable delle prenotazioni.
*
* Pre-condizione:
*   h, lezione e hp devono essere validi e non NULL.
*
* Post-condizione:
*   Vengono stampate tutte le prenotazioni della lezione specificata, se presenti.
*
* Come funziona:
* - Ottiene l’ID, nome, data e orario della lezione.
* - Scorre tutta la tabella delle prenotazioni.
* - Per ogni prenotazione che corrisponde all’ID della lezione, recupera il cliente.
* - Stampa ID prenotazione, nome e cognome cliente.
* - Se non ci sono prenotazioni per la lezione, stampa un messaggio di assenza.
*/
void stampa_prenotazioni_lezione(hashtable h, Lezione lezione, hashtable_p hp)
{
    if (lezione == NULL || hp == NULL || h == NULL) {
        printf("Errore: dati mancanti.\n");
        return;
    }

    // Ottieni gli attributi della lezione tramite getter
    char* id_lez = get_id_lezione(lezione);
    char* nome_lez = get_nome_lezione(lezione);
    Data data = get_data_lezione(lezione);
    Orario orario = get_ora_lezione(lezione);

    // Estrai giorno, mese, anno da Data
    int giorno = get_giorno(data);
    int mese = get_mese(data);
    int anno = get_anno(data);

    // Estrai ora e minuti da Orario
    int ore = get_ora(orario);
    int minuti = get_minuti(orario);

    printf("======================================================\n");
    printf("LISTA DELLE PRENOTAZIONI DELLA LEZIONE DI: %s!\n", nome_lez);
    printf("DATA: %02d/%02d/%04d    ORARIO: %02d:%02d\n", giorno, mese, anno, ore, minuti);
    printf("======================================================\n");

    printf("%-10s %-15s %-15s\n", "ID", "CLIENTE", "COGNOME");

    Prenotazione* tabella = get_table_hash_p(hp);
    int size = get_size_hash_p(hp);

    int trovata = 0;

    // Scorri tutta la tabella delle prenotazioni
    for (int i = 0; i < size; i++) 
    {
        Prenotazione p = tabella[i];
        while (p != NULL) 
        {
            if (strcmp(get_id_lezione_prenotazione(p), id_lez) == 0) 
            {
                trovata = 1;
                char* id_cli = get_id_cliente_prenotazione(p);

                // Cerca il cliente associato
                Cliente cliente = hashSearch(h, id_cli);
                if (cliente != NULL) 
                {
                    printf("%-10s %-15s %-15s\n",
                           get_id_prenotazione(p),
                           get_nome_cliente(cliente),
                           get_cognome_cliente(cliente));
                } 
                else 
                {
                    printf("%-10s %-15s %-15s\n",
                           get_id_prenotazione(p), "Cliente", "Non Trovato");
                }
            }
            p = get_next_prenotazione(p);
        }
    }

    if (!trovata) 
    {
        printf("Nessuna prenotazione trovata per questa lezione.\n");
    }
}

/*
* Funzione: cerca_lezione_per_id
* ----------------------------------------
* Cerca una lezione nella lista in base al suo ID.
*
* Parametri:
*   l: lista di lezioni.
*   id_lezione: stringa contenente l’ID della lezione da cercare.
*
* Pre-condizione:
*   La lista deve essere valida.
*
* Post-condizione:
*   Restituisce la lezione con l’ID specificato, oppure NULL se non trovata.
*
* Come funziona:
* - Scorre la lista iterativamente.
* - Confronta l’ID della lezione corrente con quello cercato.
* - Restituisce la lezione appena l'ha trovata.
*/
Lezione cerca_lezione_per_id(list l, const char *id_lezione)
{
    while (!emptyList(l))
    {
        Lezione le = getFirst(l);
        if (le != NULL && strcmp(get_id_lezione(le), id_lezione) == 0)
        {
            return le;
        }
        l = tailList(l);
    }
    return NULL;
}

/*
* Funzione: stampa_lezioni_libere
* ----------------------------------------
* Stampa la lista delle lezioni che hanno posti disponibili e che non sono passate.
*
* Parametri:
*   l: lista di lezioni.
*
* Pre-condizione:
*   La lista deve essere valida.
*
* Post-condizione:
*   Vengono stampate tutte le lezioni con posti liberi e data odierna o futura.
*
* Come funziona:
* - Ordina la lista delle lezioni.
* - Per ogni lezione verifica se ci sono posti disponibili e la data è valida.
* - Stampa i dettagli essenziali della lezione.
*/
void stampa_lezioni_libere(list l)
{
    Lezione le;
    l = ordina_Lista(l);
    Data oggi = data_oggi();

    printf("%-8s %-15s %-10s %-12s %s\n", "ID", "NOME", "ORA", "DATA", "POSTI");
    printf("======================================================\n");

    while (!emptyList(l))
    {
        le = getFirst(l);

        int posti_occupati = get_posti_occupati(le);
        int posti_max = get_posti_max(le);
        int posti_disponibili = posti_max - posti_occupati;

        if (posti_disponibili > 0 && confronta_date(get_data_lezione(le), oggi) >= 0)
        {
            visualizza_essenziale_lezione(le, posti_occupati, posti_max);
        }

        l = tailList(l);
    }

    libera_data(oggi); 
}

/*
* Funzione: calcolo_scadenza_abbonamento
* ----------------------------------------
* Calcola la data di scadenza dell’abbonamento dato l’inizio e la durata in mesi.
*
* Parametri:
*   data_inizio: data di inizio abbonamento.
*   durata_abbonamento: durata in mesi dell’abbonamento.
*
* Pre-condizione:
*   data_inizio deve essere valida.
*
* Post-condizione:
*   Restituisce la data di scadenza calcolata.
*
* Come funziona:
* - Estrae giorno, mese e anno dalla data di inizio.
* - Somma i mesi della durata, gestendo il superamento dell’anno.
* - Crea e restituisce la data di scadenza.
*/
Data calcolo_scadenza_abbonamento(Data data_inizio, int durata_abbonamento) 
{
    int giorno = get_giorno(data_inizio);
    int mese = get_mese(data_inizio);
    int anno = get_anno(data_inizio);

    mese += durata_abbonamento;

    // Gestione overflow mesi
    while (mese > 12) 
    {
        mese -= 12;
        anno += 1;
    }

    Data data_scadenza = crea_data(giorno, mese, anno);

    return data_scadenza;
}

/*
* Funzione: abbonamento_valido
* ----------------------------------------
* Verifica se un abbonamento è valido
*
* Parametri:
*   oggi: data odierna.
*   scadenza: data di scadenza dell’abbonamento.
*
* Pre-condizione:
*   Le date devono essere valide.
*
* Post-condizione:
*   Ritorna 1 se l’abbonamento è valido (oggi <= scadenza), 0 altrimenti.
*
* Come funziona:
* - Confronta le due date.
* - Restituisce 1 se oggi è precedente o uguale alla scadenza.
* - Altrimenti restituisce 0.
*/
int abbonamento_valido(Data oggi, Data scadenza) 
{
    if (confronta_date(oggi, scadenza) == -1) 
    {
        // oggi è prima della scadenza
        return 1;
    } 
    else if (confronta_date(oggi, scadenza) == 0) 
    {
        // oggi è il giorno stesso della scadenza
        return 1;
    } 
    else 
    {
        // oggi è dopo la scadenza
        return 0;
    }
}

/*
* Funzione: leggi_intero
* ----------------------------------------
* Legge un intero da input, gestendo errori di input non valido.
*
* Parametri:
*   Nessuno.
*
* Pre-condizione:
*   L’input deve essere da stdin.
*
* Post-condizione:
*   Restituisce il valore intero letto dall’utente.
*
* Come funziona:
* - Legge una riga da stdin.
* - Tenta di convertire in intero.
* - Se fallisce, chiede nuovamente l’input.
*/
int leggi_intero()
{
    char buffer[64];
    int valore;

    while (1)
    {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            continue;

        if (sscanf(buffer, "%d", &valore) == 1)
            return valore;

        printf("Input non valido. Inserisci un numero intero: ");
    }
}

/*
* Funzione: solo_lettere
* ----------------------------------------
* Verifica se una stringa contiene solo lettere (maiuscole o minuscole) e spazi.
*
* Parametri:
*   s: stringa da verificare.
*
* Pre-condizione:
*   s deve essere una stringa valida terminata da '\0'.
*
* Post-condizione:
*   Ritorna 1 se la stringa contiene almeno una lettera e nessun carattere non ammesso.
*   Ritorna 0 altrimenti.
*
* Come funziona:
* - Scorre la stringa carattere per carattere.
* - Controlla che ogni carattere sia una lettera o uno spazio.
* - Se trova numeri o caratteri non ammessi, ritorna 0.
* - Se almeno una lettera è presente e nessun carattere non ammesso, ritorna 1.
*/
int solo_lettere(char* s)
{
    int contiene_lettera = 0;

    for (int i = 0; s[i] != '\0'; i++)
    {
        if ((s[i] >= '0' && s[i] <= '9') ||        // Cifre non ammesse
            !( (s[i] >= 'A' && s[i] <= 'Z') ||     // Lettere maiuscole
               (s[i] >= 'a' && s[i] <= 'z') ||     // Lettere minuscole
               s[i] == ' '))                      // Solo spazi ammessi
        {
            return 0; // Carattere non valido
        }

        if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z'))
        {
            contiene_lettera = 1;
        }
    }

    return contiene_lettera;
}

/*
* Funzione: nome_mese
* ----------------------------------------
* Converte un numero di mese in una stringa con il nome del mese.
*
* Parametri:
*   mese: intero da 1 a 12 rappresentante il mese.
*
* Pre-condizione:
*   mese deve essere compreso tra 1 e 12.
*
* Post-condizione:
*   Restituisce il nome del mese come stringa.
*   Se il mese è fuori range, restituisce "Mese sconosciuto".
*
* Come funziona:
* - Usa un array statico di stringhe con i nomi dei mesi.
* - Ritorna il nome corrispondente all’indice.
*/
const char* nome_mese(int mese)
{
    const char* mesi[] = {"Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno",
                          "Luglio", "Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre"};
    if (mese >= 1 && mese <= 12)
        return mesi[mese - 1];
    return "Mese sconosciuto";
}