//FILE uitls.h

#ifndef UTILS_H
#define UTILS_H

#include "data.h"
#include "hash.h"
#include "cliente.h"
#include "lista_lezioni.h"
#include "hash_prenotazioni.h"

// Dichiarazione delle funzioni per la gestione del contatore di id

char* genera_id_generico(const char* prefix_letter, const char* filename);
int carica_contatore_generico(const char* filename, const char* prefix);

void carica_clienti_da_file(hashtable h);

list carica_lezioni_da_file(list l);

void carica_prenotazioni_da_file(hashtable_p h);

void aggiorna_file_lezioni(list l);

void pulisci_schermo();

int calcola_durata_in_mesi(Data data_inizio, Data data_fine);

void riscrivi_file_clienti(hashtable h);

int leggi_intero(); //controlla se l'input sia effettivamente un numero intero.

int solo_lettere(char* s);

#endif /* UTILS_H */

