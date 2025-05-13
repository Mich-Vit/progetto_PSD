//FILE area_clienti.h

#include "cliente.h"
#include "lista_lezioni.h"
#include "hash.h"

Cliente login_cliente(hashtable h);

void rinnova_abbonamento(Cliente c);

void menu_cliente(Cliente c, hashtable clienti, list lezioni);