//FILE hash.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "cliente.h"

//libreria per FNV-1a
#include <stdint.h>

// Definizione della struttura della tabella hash.
struct hash 
{
    int size;              // Dimensione della tabella hash.
    Cliente *table;        // Vettore di puntatori a Cliente.
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

hashtable newHashtable(int size) 
{
    // Allocazione della memoria per la struttura hash.
    hashtable h = (struct hash *) malloc(sizeof(struct hash));
    if (h == NULL) 
    {       
        return NULL;
    }

    h->size = size;  // Imposta la dimensione della tabella.

    // Allocazione della memoria per gli elementi della tabella,
    // inizializzando tutti i puntatori a NULL utilizzando calloc.
    h->table = calloc(size, sizeof(Cliente));
    if (h->table == NULL) 
    { 
        free(h); // Libera la memoria allocata per la struttura hash se la tabella non può essere creata.
        return NULL;
    }

    return h;  // Ritorna il puntatore alla nuova tabella hash.
}


int insertHash(hashtable h, Cliente c) 
{
    // Calcola l’indice della tabella hash usando l’ID del cliente e la dimensione della tabella
    int idx = hashFun(get_id_cliente(c), h->size);

    // Recupera la testa della lista nella posizione 'idx' della tabella
    Cliente head = h->table[idx];

    // Puntatore per scorrere la lista dei clienti già presenti in quella posizione
    Cliente curr = head;

    // Scorri la lista per verificare se esiste già un cliente con lo stesso ID
    while (curr) 
    {
        // Confronta l’ID del cliente corrente con quello del cliente da inserire
        if (strcmp(get_id_cliente(curr), get_id_cliente(c)) == 0)
        {
            return 0;  // Cliente già presente: non si inserisce
        }

        // Passa al prossimo nodo della lista
        curr = get_next_cliente(curr);
    }

    // Inserisci il nuovo cliente in testa alla lista
    set_next_cliente(c, head);   // Collega il nuovo cliente alla lista esistente
    h->table[idx] = c;           // Aggiorna la testa della lista nella tabella

    return 1;  // Inserimento riuscito
}


// Funzione per eliminare un cliente dalla tabella hash basandosi sull'ID del cliente
Cliente hashDelete(hashtable h, char *id) 
{
    int idx;               // Indice calcolato dalla funzione hash
    Cliente prev, curr, head;

    // Calcola l’indice nella tabella hash usando l’ID
    idx = hashFun(id, h->size);

    // Inizializza i puntatori:
    // 'head' punta alla testa della lista alla posizione 'idx'
    // 'curr' serve per scorrere la lista
    // 'prev' tiene traccia del nodo precedente a 'curr'
    prev = curr = head = h->table[idx];

    // Scorri la lista finché ci sono elementi
    while (curr) 
    {
        // Confronta l’ID dell’elemento corrente con quello da cercare
        if (strcmp(get_id_cliente(curr), id) == 0) 
        {
            // Caso 1: l’elemento da eliminare è il primo della lista
            if (curr == head) 
            {
                // Aggiorna il puntatore nella tabella: nuova testa è il nodo successivo
                h->table[idx] = get_next_cliente(curr);
            } 
            else 
            {
                // Caso 2: l’elemento non è in testa alla lista
                // Rimuove 'curr' collegando 'prev' direttamente al nodo successivo
                set_next_cliente(prev, get_next_cliente(curr));
            }

            // Restituisce il puntatore all’elemento rimosso (utile se vuoi liberarne la memoria dopo)
            return curr;
        }

        // Avanza nella lista: 'prev' diventa 'curr', e 'curr' avanza al nodo successivo
        prev = curr;
        curr = get_next_cliente(curr);
    }

    // Se non è stato trovato nessun cliente con quell'ID, ritorna NULL
    return NULL;
}


Cliente hashSearch(hashtable h, char *id) 
{
    if (h == NULL || id == NULL) 
        return NULL;  // Se la tabella hash o la chiave sono NULL, restituisce NULL.
    
    int idx = hashFun(id, h->size);  // Calcola l'indice nella tabella hash.
    Cliente curr = h->table[idx];     // Inizia dalla testa della lista nella posizione hash.

    // Scorre la lista nella posizione 'idx' della tabella hash.
    while (curr != NULL)
    {
        // Confronta l'ID del cliente corrente con quello cercato.
        if (strcmp(get_id_cliente(curr), id) == 0) 
        {
            return curr;  // Cliente trovato! Restituisce il puntatore al cliente.
        }
        curr = get_next_cliente(curr);  // Passa al prossimo cliente nella lista.
    }

    return NULL;  // Se il cliente non è trovato, restituisce NULL.
}

// Versione iterativa di deleteList per evitare l'overflow dello stack su liste lunghe.
static void deleteList(Cliente p) 
{
    Cliente current = p;
    Cliente temp;

    // Itera attraverso tutta la lista e libera ogni nodo
    while (current != NULL) 
    {
        temp = current;               // Salva il nodo corrente
        current = get_next_cliente(current); // Passa al prossimo nodo
        free(temp);                   // Libera il nodo corrente
    }
    return;
}
  
// Funzione per distruggere (eliminare completamente) una tabella hash.
void destroyHashtable(hashtable h) 
{
    int i;

    // Scorre ogni posizione dell'array (cioè ogni "slot" della tabella hash)
    for (i = 0; i < h->size; i++) 
    {
        // Elimina (libera) la lista concatenata di clienti presente in quella posizione, se esiste.
        deleteList(h->table[i]);
    }

    // Libera la memoria dell'array di puntatori (cioè h->table),
    // che rappresenta la tabella hash vera e propria.
    free(h->table);

    // Libera la memoria della struttura `hashtable` stessa.
    free(h);
}

// Funzione per stampare tutti i clienti nella tabella hash
void stampaHash(hashtable h)
{
    if (h == NULL)
    {
        printf("Tabella non inizializzata.\n");
        return;
    }

    printf("===================================\n");
    printf("         Elenco Clienti\n");

    int i, clienti_trovati = 0;

    for (i = 0; i < h->size; i++)
    {
        Cliente curr = h->table[i];
        while (curr != NULL)
        {
            visualizza_cliente(curr);
            curr = get_next_cliente(curr);
            clienti_trovati++;
        }
    }

    if (clienti_trovati == 0)
    {
        printf("Nessun cliente presente.\n");
    }
    else
    {
        printf("-----------------------------------\n");
        printf("Totale clienti presenti: %d\n", clienti_trovati);
    }
}

void stampaMinimaHash(hashtable h)
{
    for (int i = 0; i < h->size; i++)
    {
        Cliente curr = h->table[i];
        while (curr != NULL)
        {
            visualizza_essenziale_cliente(curr);
            curr = get_next_cliente(curr);
        }
    }
}


// === Getter ===
int get_size_hash(hashtable h)
{
    return h->size;
}

Cliente* get_table_hash(hashtable h)
{
    return h->table;
}

