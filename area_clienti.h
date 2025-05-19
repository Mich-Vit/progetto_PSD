//FILE area_clienti.h

#ifndef AREA_CLIENTI_H
#define AREA_CLIENTI_H

#include "cliente.h"
#include "lista_lezioni.h"
#include "hash.h"
#include "hash_prenotazioni.h"

Cliente login_cliente(hashtable h);

void rinnova_abbonamento(Cliente c);

void visualizza_lezioni(list l);

void disdici_prenotazione(Cliente c, hashtable_p hp, list l);

void menu_cliente(Cliente c, hashtable h, list l, hashtable_p hp);

#endif /* AREA_CLIENTI_H */