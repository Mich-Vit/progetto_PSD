// FILE hash_prenotazioni.h

#ifndef HASH_PRENOTAZIONI_H
#define HASH_PRENOTAZIONI_H

#include "prenotazione.h" 

typedef struct hash_p *hashtable_p;

/*
* Funzione: newHashtable_p
* ------------------------
* Crea e inizializza una nuova hash table per le prenotazioni.
*
* Parametri:
*   size: dimensione desiderata della tabella hash.
*
* Pre-condizione:
*   size > 0
*
* Post-condizione:
*   Viene restituito un nuovo oggetto hashtable_p correttamente allocato.
*
* Ritorna:
*   Puntatore alla nuova tabella hash per prenotazioni.
*/
hashtable_p newHashtable_p(int size);

/*
* Funzione: insertHash_p
* ----------------------
* Inserisce una prenotazione all'interno della tabella hash.
*
* Parametri:
*   h: puntatore alla hash table.
*   p: prenotazione da inserire.
*
* Pre-condizione:
*   La tabella hash e la prenotazione devono essere validi;
*   p deve avere un ID diverso da quelle presenti.
*
* Post-condizione:
*   La prenotazione viene inserita nella tabella.
*
* Ritorna:
*   1 se l'inserimento ha successo, 0 altrimenti.
*/
int insertHash_p(hashtable_p h, Prenotazione p);

/*
* Funzione: hashSearch_p
* ----------------------
* Cerca una prenotazione nella tabella hash tramite il suo ID.
*
* Parametri:
*   h: puntatore alla hash table.
*   id: identificativo della prenotazione da cercare.
*
* Pre-condizione:
*   La tabella hash deve essere valida;
*   L'ID non deve essere NULL;
*   Il numero di elementi nella tabella deve essere > 0
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   La prenotazione trovata, o NULL se non esiste.
*/
Prenotazione hashSearch_p(hashtable_p h, char *id);

/*
* Funzione: hashDelete_p
* ----------------------
* Rimuove una prenotazione dalla tabella hash tramite ID.
*
* Parametri:
*   h: puntatore alla hash table.
*   id: identificativo della prenotazione da eliminare.
*
* Pre-condizione:
*   La tabella hash deve essere valida;
*   L'ID non deve essere NULL;
*   Il numero di elementi nella tabella deve essere > 0
*
* Post-condizione:
*   La prenotazione viene rimossa dalla tabella, se esiste.
*
* Ritorna:
*   La prenotazione rimossa, o NULL se non trovata.
*/
Prenotazione hashDelete_p(hashtable_p h, char *id);

/*
* Funzione: destroyHashtable_p
* ----------------------------
* Dealloca tutta la memoria associata alla tabella hash delle prenotazioni.
*
* Parametri:
*   h: puntatore alla tabella hash da distruggere.
*
* Pre-condizione:
*   Il puntatore h deve essere valido.
*
* Post-condizione:
*   Tutti gli elementi e la tabella stessa vengono deallocati.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void destroyHashtable_p(hashtable_p h);

/*
* Funzione: stampaHash_p
* ----------------------
* Stampa a video tutte le prenotazioni contenute nella tabella hash.
*
* Parametri:
*   h: puntatore alla tabella hash.
*
* Pre-condizione:
*   Il puntatore h deve essere valido.
*
* Post-condizione:
*   Le informazioni di tutte le prenotazioni vengono stampate.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void stampaHash_p(hashtable_p h);

/*
* Funzione: stampaMinimaHash_p
* ----------------------------
* Stampa a video le informazioni essenziali delle prenotazioni nella tabella.
*
* Parametri:
*   h: puntatore alla tabella hash.
*
* Pre-condizione:
*   Il puntatore h deve essere valido.
*
* Post-condizione:
*   Le informazioni minime delle prenotazioni vengono stampate.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void stampaMinimaHash_p(hashtable_p h);

/*
* Funzione: get_size_hash_p
* -------------------------
* Restituisce la dimensione della tabella hash.
*
* Parametri:
*   h: puntatore alla tabella hash.
*
* Pre-condizione:
*   Il puntatore h deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Intero rappresentante la dimensione della tabella.
*/
int get_size_hash_p(hashtable_p h);

/*
* Funzione: get_table_hash_p
* --------------------------
* Restituisce il puntatore alla tabella interna delle prenotazioni.
*
* Parametri:
*   h: puntatore alla hash table.
*
* Pre-condizione:
*   Il puntatore h deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Puntatore all'array di prenotazioni.
*/
Prenotazione* get_table_hash_p(hashtable_p h);

#endif /* HASH_PRENOTAZIONI_H */

