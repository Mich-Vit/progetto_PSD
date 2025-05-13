//FILE area_gestore.h

#ifndef AREA_GESTORE_H
#define AREA_GESTORE_H

#include "cliente.h"
#include "lezione.h"
#include "lista_lezioni.h"
#include "hash.h"

void inserisci_cliente(hashtable h);

void menu_gestore(Cliente c, hashtable clienti, list lezioni);

#endif /* AREA_GESTORE_H */