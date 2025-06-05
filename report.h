//FILE report.h

#ifndef REPORT_H
#define REPORT_H

#include "hash_prenotazioni.h"
#include "lista_lezioni.h"

typedef struct statistica *Stats;

/*
* Funzione: genera_report_mensile
* -------------------------------
* Genera e stampa un report mensile (file di testo) sulle prenotazioni 
* e sulle tre lezioni più frequentate.
*
* Parametri:
*   hp: puntatore alla tabella hash contenente tutte le prenotazioni.
*   l: lista contenente tutte le lezioni.
*
* Pre-condizione:
*   hp deve essere una tabella hash valida e inizializzata;
*   l deve essere una lista valida (può anche essere vuota).
*
* Post-condizione:
*   Viene generato un file di testo riepilogativo con le statistiche relative
*   al numero di prenotazioni totali del mese
*   e le tre lezioni più prenotate/frequentate del mese.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void genera_report_mensile(hashtable_p hp, list l);

/*
* Funzione: genera_report_test
* ----------------------------
* Genera e stampa un report di test (file di testo) sulle prenotazioni attualmente 
* presenti nella tabella hash temporanea, utile per la verifica automatizzata dei test.
*
* Parametri:
*   hp: puntatore alla tabella hash contenente le prenotazioni temporanee create 
*       durante il test.
*   l: lista contenente tutte le lezioni (necessaria per ottenere dettagli delle lezioni).
*   output_fname: nome del file di testo in cui salvare il report generato.
*
* Pre-condizione:
*   hp deve essere una tabella hash valida e inizializzata, contenente solo le prenotazioni 
*   create nel test corrente.
*   l deve essere una lista valida (può anche essere vuota).
*   output_fname deve essere un percorso valido per la creazione del file di output.
*
* Post-condizione:
*   Viene generato un file di testo contenente il report delle prenotazioni presenti in hp,
*   formattato secondo le specifiche di test.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void genera_report_test(hashtable_p hp, list l, const char *output_fname);

#endif