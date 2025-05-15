//FILE hash.h

#ifndef HASH_H
#define HASH_H

#include "cliente.h"

typedef struct hash *hashtable;

hashtable newHashtable(int size);

int insertHash(hashtable h, Cliente c);

Cliente hashSearch(hashtable h, char *id);

Cliente hashDelete(hashtable h, char *id);

void destroyHashtable(hashtable h); 

void stampaHash(hashtable h);

// Funzioni di accesso (getter)
int get_size_hash(hashtable h);
Cliente* get_table_hash(hashtable h);




#endif /* HASH_H */