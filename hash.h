//FILE hash.h

#ifndef HASH_H
#define HASH_H

#include "cliente.h"

typedef struct hash *hashtable;

/*
* Funzione: newHashtable
* ----------------------
* Crea e inizializza una nuova hash table per i clienti.
*
* Parametri:
*   size: dimensione desiderata della tabella hash.
*
* Pre-condizione:
*   size > 0
*
* Post-condizione:
*   Viene restituito un nuovo oggetto hashtable correttamente allocato.
*
* Ritorna:
*   Puntatore alla nuova tabella hash per clienti.
*/
hashtable newHashtable(int size);

/*
* Funzione: insertHash
* --------------------
* Inserisce un cliente all'interno della tabella hash.
*
* Parametri:
*   h: puntatore alla hash table.
*   c: cliente da inserire.
*
* Pre-condizione:
*   La tabella hash e il cliente devono essere validi;
*   c deve avere un ID diverso da quelli già presenti.
*
* Post-condizione:
*   Il cliente viene inserito nella tabella.
*
* Ritorna:
*   1 se l'inserimento ha successo, 0 altrimenti.
*/
int insertHash(hashtable h, Cliente c);

/*
* Funzione: hashSearch
* --------------------
* Cerca un cliente nella tabella hash tramite il suo ID.
*
* Parametri:
*   h: puntatore alla hash table.
*   id: identificativo del cliente da cercare.
*
* Pre-condizione:
*   La tabella hash deve essere valida;
*   L'ID non deve essere NULL;
*   Il numero di elementi nella tabella deve essere > 0.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Il cliente trovato, o NULL se non esiste.
*/
Cliente hashSearch(hashtable h, char *id);

/*
* Funzione: hashDelete
* --------------------
* Rimuove un cliente dalla tabella hash tramite ID.
*
* Parametri:
*   h: puntatore alla hash table.
*   id: identificativo del cliente da eliminare.
*
* Pre-condizione:
*   La tabella hash deve essere valida;
*   L'ID non deve essere NULL;
*   Il numero di elementi nella tabella deve essere > 0.
*
* Post-condizione:
*   Il cliente viene rimosso dalla tabella, se esiste.
*
* Ritorna:
*   Il cliente rimosso, o NULL se non trovato.
*/
Cliente hashDelete(hashtable h, char *id);

/*
* Funzione: destroyHashtable
* --------------------------
* Dealloca tutta la memoria associata alla tabella hash dei clienti.
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
void destroyHashtable(hashtable h);

/*
* Funzione: stampaHash
* --------------------
* Stampa a video tutte le informazioni dei clienti nella tabella hash.
*
* Parametri:
*   h: puntatore alla tabella hash.
*
* Pre-condizione:
*   Il puntatore h deve essere valido.
*
* Post-condizione:
*   Le informazioni di tutti i clienti vengono stampate.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void stampaHash(hashtable h);

/*
* Funzione: stampaMinimaHash
* --------------------------
* Stampa a video le informazioni essenziali dei clienti nella tabella.
*
* Parametri:
*   h: puntatore alla tabella hash.
*
* Pre-condizione:
*   Il puntatore h deve essere valido.
*
* Post-condizione:
*   Le informazioni minime dei clienti vengono stampate.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void stampaMinimaHash(hashtable h);

/*
* Funzione: get_size_hash
* -----------------------
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
int get_size_hash(hashtable h);

/*
* Funzione: get_table_hash
* ------------------------
* Restituisce il puntatore alla tabella interna dei clienti.
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
*   Puntatore all'array di clienti.
*/
Cliente* get_table_hash(hashtable h);

#endif /* HASH_H */