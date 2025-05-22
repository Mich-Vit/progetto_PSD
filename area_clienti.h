//FILE area_clienti.h

#ifndef AREA_CLIENTI_H
#define AREA_CLIENTI_H

#include "cliente.h"
#include "lista_lezioni.h"
#include "hash.h"
#include "hash_prenotazioni.h"

Cliente login_cliente(hashtable h);

void menu_cliente(Cliente c, hashtable h, list l, hashtable_p hp);

#endif /* AREA_CLIENTI_H */