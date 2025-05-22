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

// Funzione per trovare o aggiungere una lezione all'array di statistiche
// Cerca se esiste già una statistica per quella lezione. Se sì, incrementa. Se no, aggiunge.
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
*  1) Filtra tutte le prenotazioni del mese corrente
*  2) Conta quante prenotazioni ha ogni lezione
*  3) Ordina le lezioni in base al numero di prenotazioni
*/
void genera_report_mensile(hashtable_p hp, list l)
{
    Data oggi = data_oggi();
    int mese_attuale = get_mese(oggi);
    int anno_attuale = get_anno(oggi);

    Prenotazione* table = get_table_hash_p(hp);
    int size = get_size_hash_p(hp);

    int size_stats = 10; //Spazio disponibile nell’array stats
    int count_stats = 0; //Numero reale di lezioni diverse già contate nel report
    int totale = 0; //conteggio totale delle prenotazioni del mese.
    Stats stats = malloc(size_stats * sizeof(struct statistica));
    if (stats == NULL)
    {
        printf("Errore di memoria in malloc.\n");
        return;
    }

    // Scansiona la tabella hash
    for (int i = 0; i < size; i++)
    {
        Prenotazione p = table[i];
        while (p != NULL)
        {
            Data d = get_data_prenotazione(p);
            //Controlla se la prenotazione è del mese corrente
            if (get_mese(d) == mese_attuale && get_anno(d) == anno_attuale)
            {
                totale++;
                aggiorna_statistica(&stats, &count_stats, &size_stats, get_id_lezione_prenotazione(p));
            }
            p = get_next_prenotazione(p);
        }
    }

    // Ordina le statistiche per prenotazioni (in ordine decrescente)
    //Ordina il vettore stats in modo che le lezioni con più prenotazioni vengano prima
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
    fprintf(fp, "Lezioni più frequentate:\n");

    
    /*
    Se ci sono meno di 3 lezioni, stampa solo quelle esistenti.
    Altrimenti, stampa le prime 3.
    */
    int max;
    if (count_stats < 3)
    {
        max = count_stats;
    }
    else
    {
        max = 3;
    }

    for (int i = 0; i < max; i++)
    {
        Lezione lez = cerca_lezione_per_id(l, stats[i].id_lezione);
        const char* nome;

        if (lez != NULL)
        {
            nome = get_nome_lezione(lez);

            fprintf(fp, "%d. %s - %d prenotazioni\n", i + 1, nome, stats[i].conteggio);
            fprintf(fp, "    Data: %02d/%02d/%04d | Ora: %02d:%02d\n",
                    get_giorno(get_data_lezione(lez)),
                    get_mese(get_data_lezione(lez)),
                    get_anno(get_data_lezione(lez)),
                    get_ora(get_ora_lezione(lez)),
                    get_minuti(get_ora_lezione(lez)));
        }
        else
        {
            nome = "Nome non disponibile";
            fprintf(fp, "%d. %s - %d prenotazioni\n", i + 1, nome, stats[i].conteggio);
            fprintf(fp, "    Data: Data N/D | Ora: Ora N/D\n");
        }
    }

    fclose(fp);
    free(stats);
    printf("Report mensile generato correttamente in '%s'\n", nome_file);
}
