//FILE hash_prenotazioni.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_prenotazioni.h"
#include "prenotazione.h" 

//libreria per FNV-1a
#include <stdint.h>

// Definizione della struttura della tabella hash.
struct hash_p 
{
    int size;              // Dimensione della tabella hash.
    Prenotazione *table;        // Vettore di puntatori a Cliente.
};

 /*
* Funzione: fnv1aHash32
* ----------------------------------------
* Calcola l'hash FNV-1a a 32 bit per una stringa.
*
* Parametri:
*   key: la stringa da trasformare in hash.
*
* Valore di ritorno:
*   Valore hash a 32 bit generato dalla stringa.
*
* Dettagli:
* - L'algoritmo FNV-1a è rapido e produce buone distribuzioni.
* - Utilizza costanti predefinite per inizializzazione e moltiplicazione.
* 
* Come funziona:
* 1. Inizializza il valore hash con l'offset basis
* 2. Per ogni carattere nella stringa:
*    - Applica un XOR tra l'hash e il carattere corrente
*    - Moltiplica il risultato per il FNV prime
* 3. Restituisce l'hash risultante
*/
static uint32_t fnv1aHash32(const char *key)
{
    uint32_t hash = 2166136261u;
    while (*key)
    {
        hash ^= (unsigned char)(*key);
        hash *= 16777619u;
        key++;
    }
    return hash;
}

/*
* Funzione: hashFun
* ----------------------------------------
* Calcola un indice valido per una tabella hash a partire da una chiave stringa.
*
* Parametri:
*   key: chiave stringa da convertire.
*   size: dimensione della tabella hash.
*
* Valore di ritorno:
*   Intero nell'intervallo [0, size-1].
*
* Uso:
* - Garantisce che l'indice sia all'interno dei limiti della tabella.
*/
static int hashFun(const char *key, int size) 
{
    // Calcola l'hash a 32 bit della chiave usando FNV-1a
    // e lo riduce entro il range [0, size-1]
    return fnv1aHash32(key) % size;
}

/*
* Funzione: newHashtable_p
* ----------------------------------------
* Crea una nuova tabella hash per le prenotazioni.
*
* Parametri:
*   size: dimensione della tabella.
*
* Pre-condizione:
*   size > 0
*
* Post-condizione:
*   Viene restituito un nuovo oggetto hashtable_p correttamente allocato.
*
* Come funziona:
* - Alloca la struttura principale.
* - Se fallisce ritorna NULL.
* - Imposta la dimensione della tabella.
* - Inizializza la tabella con calloc (tutti NULL).
*/
hashtable_p newHashtable_p(int size) 
{
    hashtable_p h = (struct hash_p *) malloc(sizeof(struct hash_p));
    if (h == NULL) 
    {       
        return NULL;
    }

    h->size = size;

    h->table = calloc(size, sizeof(Prenotazione));
    if (h->table == NULL) 
    { 
        free(h); 
        return NULL;
    }

    return h;
}

/*
* Funzione: insertHash_p
* ----------------------------------------
* Inserisce una nuova prenotazione nella tabella hash.
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
* Come funziona:
* - Calcola l’indice della tabella hash usando hushFun
* - Inserisce in testa alla lista nella posizione hash.
* - Non permette duplicati (controllo su ID).
* - Ritorna: 1 se l'inserimento ha successo, 0 altrimenti.
*/
int insertHash_p(hashtable_p h, Prenotazione p) 
{
    int idx = hashFun(get_id_prenotazione(p), h->size);

    Prenotazione head = h->table[idx];
    Prenotazione curr = head;

    // Scorri la lista per verificare se esiste già una prenotazione con lo stesso ID
    while (curr) 
    {
        if (strcmp(get_id_prenotazione(curr), get_id_prenotazione(p)) == 0)
        {
            return 0;  // già presente, non inserisco
        }
        curr = get_next_prenotazione(curr);
    }

    // Inserisci in testa alla lista
    set_next_prenotazione(p, head);
    h->table[idx] = p;
    return 1;
}

/*
* Funzione: hashDelete_p
* ----------------------------------------
* Elimina una prenotazione dalla tabella hash dato un ID.
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
* Come funziona:
* - Calcola l’indice della tabella hash usando hushFun
* - Gestisce eliminazione in testa e in mezzo alla lista.
* - Ritorna: la prenotazione rimossa, o NULL se non trovata.
*/
Prenotazione hashDelete_p(hashtable_p h, char *id) 
{
    if (h == NULL || id == NULL)
        return NULL;

    int idx = hashFun(id, h->size);
    Prenotazione prev = NULL;
    Prenotazione curr = h->table[idx];

    while (curr) 
    {
        if (strcmp(get_id_prenotazione(curr), id) == 0) 
        {
            if (prev == NULL) 
            {
                // eliminazione in testa
                h->table[idx] = get_next_prenotazione(curr);
            } 
            else 
            {
                set_next_prenotazione(prev, get_next_prenotazione(curr));
            }
            return curr;  // restituisco la prenotazione eliminata
        }
        prev = curr;
        curr = get_next_prenotazione(curr);
    }
    return NULL;
}

/*
* Funzione: hashSearch_p
* ----------------------------------------
* Cerca una prenotazione nella tabella tramite ID.
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
* Ritorna:
*   La prenotazione trovata, o NULL se non esiste.
*
* Come funziona:
* - Calcola l’indice della tabella hash usando hushFun
* - Scorre la lista delle prenotazioni di quell'indice
* - Ritorna: la prenotazione ricercata, oppure NULL se non la trova
*/
Prenotazione hashSearch_p(hashtable_p h, char *id) 
{
    if (h == NULL || id == NULL)
        return NULL;

    int idx = hashFun(id, h->size);
    Prenotazione curr = h->table[idx];

    while (curr) 
    {
        if (strcmp(get_id_prenotazione(curr), id) == 0) 
            return curr;
        curr = get_next_prenotazione(curr);
    }
    return NULL;
}

/*
 * Funzione: deleteList_p
 * ----------------------------------------
 * Elimina ricorsivamente una lista collegata di prenotazioni.
 *
 * Parametri:
 *   p: testa della lista da eliminare.
 *
 * Uso:
 * - Chiamata internamente da destroyHashtable_p.
 */
static void deleteList_p(Prenotazione p) 
{
    Prenotazione current = p;
    Prenotazione temp;

    while (current != NULL) 
    {
        temp = current;
        current = get_next_prenotazione(current);
        free(temp);
    }
}
  
/*
 * Funzione: destroyHashtable_p
 * ----------------------------------------
 * Libera tutta la memoria associata alla tabella hash.
 *
 * Parametri:
 *   h: tabella hash da distruggere.
 *
 * Come funziona:
 * - Elimina tutte le liste collegate chiamando la funzione deleteList_p.
 * - Libera l’array e la struttura.
 */
void destroyHashtable_p(hashtable_p h) 
{
    if (h == NULL)
        return;

    for (int i = 0; i < h->size; i++) 
    {
        deleteList_p(h->table[i]);
    }

    free(h->table);
    free(h);
}

/*
* Funzione: stampaHash_p
* ----------------------------------------
* Stampa tutte le prenotazioni nella tabella.
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
* Come funziona:
* - Scorre l’intera tabella e chiama la funzione visualizza_prenotazione.
* - Conta il numero totale di prenotazioni.
*/
void stampaHash_p(hashtable_p h)
{
    if (h == NULL) 
    {
        printf("Tabella non inizializzata.\n");
        return;
    }

    printf("===================================\n");
    printf("         Elenco Prenotazioni\n");

    int count = 0;
    for (int i = 0; i < h->size; i++) 
    {
        Prenotazione curr = h->table[i];
        while (curr) 
        {
            visualizza_prenotazione(curr);
            curr = get_next_prenotazione(curr);
            count++;
        }
    }

    if (count == 0)
        printf("Nessuna prenotazione presente.\n");
    else
        printf("-----------------------------------\n");
        printf("Totale prenotazioni: %d\n", count);
}

/*
* Funzione: stampaMinimaHash_p
* ----------------------------------------
* Stampa una versione ridotta delle prenotazioni.
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
* Come funziona:
* - Scorre l’intera tabella e chiama la funzione visualizza_essenziale_prenotazione.
*/
void stampaMinimaHash_p(hashtable_p h)
{
    if (h == NULL)
        return;

    for (int i = 0; i < h->size; i++) 
    {
        Prenotazione curr = h->table[i];
        while (curr) 
        {
            visualizza_essenziale_prenotazione(curr);
            curr = get_next_prenotazione(curr);
        }
    }
}

// === Getter ===

/*
 * Funzione: get_size_hash_p
 * ----------------------------------------
 * Restituisce la dimensione della tabella hash.
 *
 * Parametri:
 *   h: tabella hash.
 *
 * Valore di ritorno:
 *   Dimensione della tabella.
 */
int get_size_hash_p(hashtable_p h)
{
    return h->size;
}

/*
 * Funzione: get_table_hash_p
 * ----------------------------------------
 * Restituisce l’array delle liste di prenotazioni.
 *
 * Parametri:
 *   h: tabella hash.
 *
 * Valore di ritorno:
 *   Puntatore all’array di prenotazioni.
 */
Prenotazione* get_table_hash_p(hashtable_p h)
{
    return h->table;
}

