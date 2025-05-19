//FILE area_gestore.h

#ifndef AREA_GESTORE_H
#define AREA_GESTORE_H

#include "cliente.h"
#include "lezione.h"
#include "lista_lezioni.h"
#include "hash.h"
#include "hash_prenotazioni.h"

void inserisci_cliente(hashtable h);

list inserisci_lezione(list l);

void rimuovi_cliente(hashtable h, hashtable_p hp);

list rimuovi_lezione(list l, hashtable_p hp);

void ricerca_cliente(hashtable h);

void menu_gestore(hashtable h, list l, hashtable_p hp);


#endif /* AREA_GESTORE_H */