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
* Funzione: newHashtable
* ----------------------------------------
* Crea una nuova tabella hash per i clienti.
*
* Parametri:
*   size: dimensione della tabella.
*
* Pre-condizione:
*   size > 0
*
* Post-condizione:
*   Viene restituito un nuovo oggetto hashtable correttamente allocato.
*
* Come funziona:
* - Alloca la struttura principale.
* - Se fallisce ritorna NULL.
* - Imposta la dimensione della tabella.
* - Inizializza la tabella con calloc (tutti NULL).
*/
hashtable newHashtable(int size) 
{
    hashtable h = (struct hash *) malloc(sizeof(struct hash));
    if (h == NULL) 
    {       
        return NULL;
    }

    h->size = size;  

    // Allocazione della memoria per gli elementi della tabella,
    // inizializzando tutti i puntatori a NULL utilizzando calloc.
    h->table = calloc(size, sizeof(Cliente));
    if (h->table == NULL) 
    { 
        free(h);
        return NULL;
    }

    return h;  
}

/*
* Funzione: insertHash
* ----------------------------------------
* Inserisce un nuovo cliente nella tabella hash.
*
* Parametri:
*   h: puntatore alla hash table.
*   c: cliente da inserire.
*
* Pre-condizione:
*   h e c devono essere validi;
*   c deve avere un ID diverso da quelle presenti.
*
* Post-condizione:
*   c viene inserita nella tabella.
*
* Come funziona:
* - Calcola l’indice della tabella hash usando hushFun
* - Inserisce in testa alla lista nella posizione hash.
* - Non permette duplicati (controllo su ID).
* - Ritorna: 1 se l'inserimento ha successo, 0 altrimenti.
*/
int insertHash(hashtable h, Cliente c) 
{
    
    int idx = hashFun(get_id_cliente(c), h->size);

    // Recupera la testa della lista nella posizione 'idx' della tabella
    Cliente head = h->table[idx];

    // Puntatore per scorrere la lista dei clienti già presenti in quella posizione
    Cliente curr = head;

    // Scorri la lista per verificare se esiste già un cliente con lo stesso ID
    while (curr) 
    {
        if (strcmp(get_id_cliente(curr), get_id_cliente(c)) == 0)
        {
            return 0; 
        }
        curr = get_next_cliente(curr);
    }

    set_next_cliente(c, head); 
    h->table[idx] = c;  

    return 1;
}

/*
* Funzione: hashDelete
* ----------------------------------------
* Elimina un cliente dalla tabella hash dato un ID.
*
* Parametri:
*   h: puntatore alla hash table.
*   id: identificativo del cliente da eliminare.
*
* Pre-condizione:
*   La tabella hash deve essere valida;
*   L'ID non deve essere NULL;
*   Il numero di elementi nella tabella deve essere > 0
*
* Post-condizione:
*   il cliente viene rimosso dalla tabella, se esiste.
*
* Come funziona:
* - Calcola l’indice della tabella hash usando hushFun
* - Gestisce eliminazione in testa e in mezzo alla lista.
* - Ritorna: il cliente rimosso, o NULL se non trovata.
*/
Cliente hashDelete(hashtable h, char *id) 
{
    int idx; 
    Cliente prev, curr, head;
    idx = hashFun(id, h->size);

    // Inizializza i puntatori:
    // 'head' punta alla testa della lista alla posizione 'idx'
    // 'curr' serve per scorrere la lista
    // 'prev' tiene traccia del nodo precedente a 'curr'
    head = h->table[idx];
    curr = head;
    prev = NULL;

    // Scorri la lista finché ci sono elementi
    while (curr) 
    {
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
            return curr;
        }
        prev = curr;
        curr = get_next_cliente(curr);
    }

    // Se non è stato trovato nessun cliente con quell'ID, ritorna NULL
    return NULL;
}

/*
* Funzione: hashSearch
* ----------------------------------------
* Cerca un cliente nella tabella tramite ID.
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
*   Il cliente trovata, o NULL se non esiste.
*
* Come funziona:
* - Calcola l’indice della tabella hash usando hushFun
* - Scorre la lista delle prenotazioni di quell'indice
* - Ritorna: la prenotazione ricercata, oppure NULL se non la trova
*/
Cliente hashSearch(hashtable h, char *id) 
{
    if (h == NULL || id == NULL) 
        return NULL; 
    
    int idx = hashFun(id, h->size);  
    Cliente curr = h->table[idx];    

    // Scorre la lista nella posizione 'idx' della tabella hash.
    while (curr != NULL)
    {
        if (strcmp(get_id_cliente(curr), id) == 0) 
        {
            return curr;
        }
        curr = get_next_cliente(curr);  
    }

    return NULL; 
}

/*
 * Funzione: deleteList
 * ----------------------------------------
 * Elimina una lista collegata di prenotazioni.
 *
 * Parametri:
 *   p: testa della lista da eliminare.
 *
 * Uso:
 * - Chiamata internamente da destroyHashtable_p.
 */
static void deleteList(Cliente p) 
{
    Cliente current = p;
    Cliente temp;

    while (current != NULL) 
    {
        temp = current;  
        current = get_next_cliente(current);
        free(temp);            
    }
    return;
}
  
/*
 * Funzione: destroyHashtable
 * ----------------------------------------
 * Libera tutta la memoria associata alla tabella hash.
 *
 * Parametri:
 *   h: tabella hash da distruggere.
 *
 * Come funziona:
 * - Elimina tutte le liste collegate chiamando la funzione deleteList.
 * - Libera l’array e la struttura.
 */
void destroyHashtable(hashtable h) 
{
    int i;

    for (i = 0; i < h->size; i++) 
    {
        deleteList(h->table[i]);
    }

    free(h->table);

    free(h);
}

/*
* Funzione: stampaHash_p
* ----------------------------------------
* Stampa tutti i clienti nella tabella.
*
* Parametri:
*   h: puntatore alla tabella hash.
*
* Pre-condizione:
*   Il puntatore h deve essere valido.
*
* Post-condizione:
*   Le informazioni di tutti clienti vengono stampate.
*
* Come funziona:
* - Scorre l’intera tabella e chiama la funzione visualizza_prenotazione.
* - Conta il numero totale di prenotazioni.
*/
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

/*
* Funzione: stampaMinimaHash
* ----------------------------------------
* Stampa una versione ridotta dei clienti.
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
* - Scorre l’intera tabella e chiama la funzione visualizza_essenziale_cliente.
*/
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


/*
 * Funzione: get_size_hash
 * ----------------------------------------
 * Restituisce la dimensione della tabella hash.
 *
 * Parametri:
 *   h: tabella hash.
 *
 * Valore di ritorno:
 *   Dimensione della tabella.
 */
int get_size_hash(hashtable h)
{
    return h->size;
}

/*
 * Funzione: get_table_hash
 * ----------------------------------------
 * Restituisce l’array delle liste di clienti.
 *
 * Parametri:
 *   h: tabella hash.
 *
 * Valore di ritorno:
 *   Puntatore all’array di clienti.
 */
Cliente* get_table_hash(hashtable h)
{
    return h->table;
}

