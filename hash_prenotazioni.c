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
 * Calcola l'hash FNV-1a a 32 bit per una stringa.
 * 
 * L'algoritmo FNV-1a è noto per la sua semplicità, velocità 
 * e buona distribuzione dei valori. Utilizza due costanti:
 * - offset basis (2166136261) come valore iniziale
 * - FNV prime (16777619) per la moltiplicazione
 *
 * Il processo:
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

// Funzione che calcola un valore hash limitato alla dimensione di una tabella
static int hashFun(const char *key, int size) 
{
    // Calcola l'hash a 32 bit della chiave usando FNV-1a
    // e lo riduce entro il range [0, size-1]
    return fnv1aHash32(key) % size;
}

hashtable_p newHashtable_p(int size) 
{
    int i;
    // Allocazione della memoria per la struttura hash.
    hashtable_p h = (struct hash_p *) malloc(sizeof(struct hash_p));
    if (h == NULL) 
    {       
        return NULL;
    }

    h->size = size;  // Imposta la dimensione della tabella.

    // Allocazione della memoria per gli elementi della tabella,
    // inizializzando tutti i puntatori a NULL utilizzando calloc.
    h->table = calloc(size, sizeof(Prenotazione));
    if (h->table == NULL) 
    { 
        free(h); // Libera la memoria allocata per la struttura hash se la tabella non può essere creata.
        return NULL;
    }

    return h;  // Ritorna il puntatore alla nuova tabella hash.
}


int insertHash_p(hashtable_p h, Prenotazione p) 
{
    // Calcola l’indice della tabella hash usando l’ID del cliente e la dimensione della tabella
    int idx = hashFun(get_id_prenotazione(p), h->size);

    // Recupera la testa della lista nella posizione 'idx' della tabella
    Prenotazione head = h->table[idx];

    // Puntatore per scorrere la lista dei clienti già presenti in quella posizione
    Prenotazione curr = head;

    // Scorri la lista per verificare se esiste già un cliente con lo stesso ID
    while (curr) 
    {
        // Controlla se esiste già una prenotazione con lo stesso id
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


// Funzione per eliminare un cliente dalla tabella hash basandosi sull'ID del cliente
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
  
// Funzione per distruggere (eliminare completamente) una tabella hash.
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
int get_size_hash_p(hashtable_p h)
{
    return h->size;
}

Prenotazione* get_table_hash_p(hashtable_p h)
{
    return h->table;
}

