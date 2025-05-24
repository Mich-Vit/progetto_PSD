//FILE area_gestore.h

#ifndef AREA_GESTORE_H
#define AREA_GESTORE_H

#include "lista_lezioni.h"
#include "hash.h"
#include "hash_prenotazioni.h"

/*
* Funzione: menu_gestore
* -----------------------
* Mostra e gestisce il menu riservato al gestore del sistema, permettendo
* operazioni come la gestione delle lezioni, dei clienti e delle prenotazioni.
*
* Parametri:
*   h: hashtable contenente i dati dei clienti registrati.
*   l: lista delle lezioni disponibili nel sistema.
*   hp: hashtable contenente le prenotazioni effettuate.
*
* Pre-condizione:
*   Le strutture dati (hashtable dei clienti, lista lezioni, hashtable prenotazioni)
*   devono essere correttamente inizializzate.
*
* Post-condizione:
*   Il gestore può effettuare modifiche alle lezioni, ai clienti e alle prenotazioni;
*   il sistema aggiorna i dati di conseguenza.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void menu_gestore(hashtable h, list l, hashtable_p hp);

#endif /* AREA_GESTORE_H */