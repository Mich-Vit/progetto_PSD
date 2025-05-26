//FILE report.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "report.h"
#include "data.h"
#include "prenotazione.h"
#include "lezione.h"
#include "utils.h"
#include "hash_prenotazioni.h"
#include "lista_lezioni.h"

/*
*   La struttura statistica e la funzione aggiorna_statistica servono proprio per 
*   tenere traccia del numero di prenotazioni effettuate per 
*   ciascuna lezione durante il mese corrente.
*/
struct statistica
{
    char id_lezione[20]; //id della lezione
    int conteggio; //numero di prenotazioni su quella lezione
};

/*
 * Funzione: aggiorna_statistica
 * -----------------------------
 * Aggiorna un array dinamico di statistiche. Se esiste già la lezione nell'array, 
 * incrementa il conteggio. Altrimenti, la aggiunge.
 *
 * Parametri:
 *   stats: puntatore al puntatore all'array di statistiche.
 *   n: puntatore al numero corrente di elementi nell'array.
 *   size_stats: puntatore alla dimensione attuale dell'array.
 *   id_lezione: ID della lezione da cercare o aggiungere.
 *
 * Pre-condizioni:
 *   - stats deve puntare a un array valido (eventualmente allocato via malloc).
 *   - n e size_stats devono essere inizializzati.
 *
 * Post-condizioni:
 *   - stats viene aggiornato con il nuovo conteggio o una nuova voce.
 *   - In caso di realloc, la dimensione dell’array viene raddoppiata.
 *
 * Come funziona:
 * - Cerca la lezione già presente.
 * - Se trovata, incrementa il conteggio.
 * - Se non trovata e lo spazio è insufficiente, raddoppia l’array con 
 *   realloc e inserisce la nuova voce.
 */
static int aggiorna_statistica(Stats* stats, int* n, int* size_stats, const char* id_lezione)
{
    for (int i = 0; i < *n; i++)
    {
        if (strcmp((*stats)[i].id_lezione, id_lezione) == 0)
        {
            (*stats)[i].conteggio++;
            return 1;
        }
    }

    //Controlla se c’è abbastanza spazio
    if (*n >= *size_stats)
    {
        //Se non c’è, raddoppia la capacità con realloc
        *size_stats = (*size_stats) * 2;
        *stats = realloc(*stats, (*size_stats) * sizeof(struct statistica));
        if (*stats == NULL)
        {
            printf("Errore di memoria in realloc.\n");
            exit(EXIT_FAILURE);
        }
    }

    strcpy((*stats)[*n].id_lezione, id_lezione);
    (*stats)[*n].conteggio = 1;
    (*n)++;
    return 1;
}

/*
 * Funzione: genera_report_mensile
 * -------------------------------
 * Genera un report testuale delle lezioni più prenotate nel mese corrente.
 *
 * Parametri:
 *   hp: tabella hash contenente tutte le prenotazioni.
 *   l: lista contenente tutte le lezioni disponibili (per risalire a nome/data/ora).
 *
 * Pre-condizioni:
 *   hp deve essere una tabella hash valida con prenotazioni.
 *   l deve essere una lista valida contenente le lezioni.
 *
 * Post-condizioni:
 *   - Viene generato un file di testo con il nome `report_<mese>_<anno>.txt`.
 *   - Il file contiene:
 *       1. Totale delle prenotazioni effettuate nel mese corrente (indipendentemente 
 *          dalla data della lezione).
 *       2. Le 3 lezioni più prenotate che si svolgono nel mese corrente (o meno 
 *          se ce ne sono meno).
 *
 * Come funziona:
 * 1. Ottiene la data odierna e filtra le prenotazioni effettuate nel mese corrente.
 * 2. Per ciascuna prenotazione del mese corrente, aggiorna un array di statistiche 
 *    sul numero di prenotazioni per ciascuna lezione.
 * 3. Ordina l’array di statistiche in ordine decrescente di prenotazioni.
 * 4. Tra queste, seleziona fino a 3 lezioni che si svolgono effettivamente nel 
 *    mese corrente.
 * 5. Crea un file `report_<mese>_<anno>.txt` e scrive:
 *    - il numero totale di prenotazioni del mese corrente,
 *    - fino a 3 lezioni più frequentate (che si svolgono nel mese).
 * 6. Stampa un messaggio di conferma a schermo.
 */
void genera_report_mensile(hashtable_p hp, list l)
{
    Data oggi = data_oggi();
    int mese_attuale = get_mese(oggi);
    int anno_attuale = get_anno(oggi);

    Prenotazione* table = get_table_hash_p(hp);
    int size = get_size_hash_p(hp);

    int size_stats = 10;
    int count_stats = 0;

    // Conta tutte le prenotazioni del mese corrente (indipendentemente dalla data della lezione)
    int totale = 0; 
    
    Stats stats = malloc(size_stats * sizeof(struct statistica));
    if (stats == NULL)
    {
        printf("Errore di memoria in malloc.\n");
        return;
    }

    // Scorri tutte le prenotazioni
    for (int i = 0; i < size; i++)
    {
        Prenotazione p = table[i];
        while (p != NULL)
        {
            Data data_pren = get_data_prenotazione(p);

            // Prenotazione fatta nel mese corrente
            if (get_mese(data_pren) == mese_attuale && get_anno(data_pren) == anno_attuale)
            {
                totale++;
                aggiorna_statistica(&stats, &count_stats, &size_stats, get_id_lezione_prenotazione(p));
            }

            p = get_next_prenotazione(p);
        }
    }

    // Ordina le lezioni in base al numero di prenotazioni
    for (int i = 0; i < count_stats - 1; i++)
    {
        for (int j = i + 1; j < count_stats; j++)
        {
            if (stats[j].conteggio > stats[i].conteggio)
            {
                struct statistica tmp = stats[i];
                stats[i] = stats[j];
                stats[j] = tmp;
            }
        }
    }

    // Scrive il file
    char nome_file[50];
    sprintf(nome_file, "report_%s_%04d.txt", nome_mese(mese_attuale), anno_attuale);
    FILE* fp = fopen(nome_file, "w");
    if (!fp)
    {
        printf("Errore nell'apertura del file report.\n");
        free(stats);
        return;
    }

    fprintf(fp, "REPORT MENSILE - %s %d\n\n", nome_mese(mese_attuale), anno_attuale);
    fprintf(fp, "Totale prenotazioni effettuate: %d\n\n", totale);
    fprintf(fp, "Lezioni più frequentate (solo quelle di %s):\n", nome_mese(mese_attuale));

    int stampati = 0;
    for (int i = 0; i < count_stats && stampati < 3; i++)
    {
        Lezione lez = cerca_lezione_per_id(l, stats[i].id_lezione);
        if (lez != NULL)
        {
            Data data_lez = get_data_lezione(lez);
            if (get_mese(data_lez) == mese_attuale && get_anno(data_lez) == anno_attuale)
            {
                // Stampa solo se la lezione si tiene nel mese corrente
                fprintf(fp, "%d. %s - %d prenotazioni\n", stampati + 1, get_nome_lezione(lez), stats[i].conteggio);
                fprintf(fp, "    Data: %02d/%02d/%04d | Ora: %02d:%02d\n",
                        get_giorno(data_lez),
                        get_mese(data_lez),
                        get_anno(data_lez),
                        get_ora(get_ora_lezione(lez)),
                        get_minuti(get_ora_lezione(lez)));

                stampati++;
            }
        }
    }

    if (stampati == 0)
    {
        fprintf(fp, "Nessuna lezione in questo mese tra quelle prenotate.\n");
    }

    fclose(fp);
    free(stats);
    printf("Report mensile generato correttamente in '%s'\n", nome_file);
}
