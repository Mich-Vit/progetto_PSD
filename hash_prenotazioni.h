// FILE hash_prenotazioni.h

#ifndef HASH_PRENOTAZIONI_H
#define HASH_PRENOTAZIONI_H

#include "prenotazione.h" 

typedef struct hash_p *hashtable_p;

hashtable_p newHashtable_p(int size);

int insertHash_p(hashtable_p h, Prenotazione p);

Prenotazione hashSearch_p(hashtable_p h, char *id);

Prenotazione hashDelete_p(hashtable_p h, char *id);

void destroyHashtable_p(hashtable_p h);

void stampaHash_p(hashtable_p h);

void stampaMinimaHash_p(hashtable_p h);

// Funzioni getter
int get_size_hash_p(hashtable_p h);
Prenotazione* get_table_hash_p(hashtable_p h);

#endif /* HASH_PRENOTAZIONI_H */

