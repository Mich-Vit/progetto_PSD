//FILE uitls.h

#ifndef UTILS_H
#define UTILS_H

#include "data.h"
#include "hash.h"
#include "cliente.h"
#include "lista_lezioni.h"

// Dichiarazione delle funzioni per la gestione del contatore di id
int carica_contatore_clienti();

int carica_contatore_lezioni();

void carica_clienti_da_file(hashtable h);

list carica_lezioni_da_file(list l);

void pulisci_schermo();

int calcola_durata_in_mesi(Data data_inizio, Data data_fine);

void riscrivi_file_clienti(hashtable h);

#endif /* UTILS_H */

