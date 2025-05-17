//FILE lista_lezioni.h

#ifndef LISTA_LEZIONI_H
#define LISTA_LEZIONI_H

#include"lezione.h"

typedef struct node *list;

list newList(void); //crea una nuova lista vuota

int emptyList(list l); //se la lista e' vuota restituisce 1, altrimenti 0

list consList(Lezione le, list l); //aggiunge un nuovo nodo in testa alla lista

list tailList(list l); //restituisce la coda della lista (tutti i nodi dopo il primo)

Lezione getFirst(list l); //restituisce il primo nodo in testa

int sizeList(list l); //restituisce la lunghezza della lista

list reverseList(list l); //restituisce la lista invertia

list removeList(list l, int pos);

void outputList(list l); //stampa tutti i nodi della lista
void stampaMinimaList(list l);

list ordina_Lista(list l);

#endif /* LISTA_LEZIONI_H */