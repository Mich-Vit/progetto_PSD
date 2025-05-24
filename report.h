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

#endif