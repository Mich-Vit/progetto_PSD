//FILE lista_clienti.h

#include"cliente.h"

typedef struct node *list;

list newList(void); //crea una nuova lista vuota

int emptyList(list l); //se la lista e' vuota restituisce 1, altrimenti 0

list consList(Cliente cl, list l); //aggiunge un nuovo nodo in testa alla lista

list tailList(list l); //restituisce la coda della lista (tutti i nodi dopo il primo)

Cliente getFirst (list l); //restituisce il primo nodo in testa

int sizeList (list l); //restituisce la lunghezza della lista

int posCliente (list l, Cliente cl); //restiruisce la posizione nella lista di un determinato ciente

Cliente getCliente (list l, int pos); //data una posizione, restituisce l'elemento in quella posizione

list reverseList (list l); //restituisce la lista invertia

void outputList (list l); //stampa tutti i nodi della lista