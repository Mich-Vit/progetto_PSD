//FILE uitls.h

#ifndef UTILS_H
#define UTILS_H

#include "data.h"
#include "hash.h"
#include "cliente.h"
#include "lista_lezioni.h"
#include "hash_prenotazioni.h"

void rinnova_abbonamento(Cliente c, hashtable h);

char* genera_id_generico(const char* prefix_letter, const char* filename);
int carica_contatore_generico(const char* filename, const char* prefix);

void carica_clienti_da_file(hashtable h);

list carica_lezioni_da_file(list l);

void carica_prenotazioni_da_file(hashtable_p h);

void aggiorna_file_clienti(hashtable h);
void aggiorna_file_lezioni(list l);
void aggiorna_file_prenotazioni(hashtable_p h);

void pulisci_schermo();

Lezione cerca_lezione_per_id(list l, const char *id_lezione);

void stampa_lezioni_libere(list l);

int calcola_durata_in_mesi(Data data_inizio, Data data_fine);

void stampa_prenotazioni_cliente(Cliente c, hashtable_p hp, list l);
void stampa_prenotazioni_lezione(hashtable h, Lezione lezione, hashtable_p hp);

int abbonamento_valido(Data oggi, Data scadenza);
Data calcolo_scadenza_abbonamento(Data data_inizio, int durata_abbonamento);;

int leggi_intero(); //controlla se l'input sia effettivamente un numero intero.

int solo_lettere(char* s);

const char* nome_mese(int mese);

#endif /* UTILS_H */

