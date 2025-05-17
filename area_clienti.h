//FILE area_clienti.h

#ifndef AREA_CLIENTI_H
#define AREA_CLIENTI_H

#include "cliente.h"
#include "lista_lezioni.h"
#include "hash.h"

Cliente login_cliente(hashtable h);

void rinnova_abbonamento(Cliente c);

void visualizza_lezioni(list l);

void menu_cliente(Cliente c, hashtable h, list l);

#endif /* AREA_CLIENTI_H */