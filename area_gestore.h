//FILE area_gestore.h

#ifndef AREA_GESTORE_H
#define AREA_GESTORE_H

#include "cliente.h"
#include "lezione.h"
#include "lista_lezioni.h"
#include "hash.h"

void inserisci_cliente(hashtable h);

list inserisci_lezione(list l);

void rimuovi_cliente(hashtable h);

list rimuovi_lezione(list l);

void ricerca_cliente(hashtable h);

void menu_gestore(hashtable h, list l);


#endif /* AREA_GESTORE_H */