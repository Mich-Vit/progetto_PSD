//FILE uitls.h

#ifndef UTILS_H
#define UTILS_H

#include "data.h"
#include "hash.h"
#include "cliente.h"

// Dichiarazione delle funzioni per la gestione del contatore di id
void salva_contatore(int counter);
int carica_contatore();

void carica_clienti_da_file(hashtable h);

void pulisci_schermo();

#endif /* UTILS_H */

