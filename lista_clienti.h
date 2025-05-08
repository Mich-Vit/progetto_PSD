//FILE lista_clienti.h

#include"cliente.h"

typedef struct node *list;

list newList_c(void); //crea una nuova lista vuota

int emptyList_c(list l); //se la lista e' vuota restituisce 1, altrimenti 0

list consList_c(Cliente cl, list l); //aggiunge un nuovo nodo in testa alla lista

list tailList_c(list l); //restituisce la coda della lista (tutti i nodi dopo il primo)

Cliente getFirst_c(list l); //restituisce il primo nodo in testa

int sizeList_c(list l); //restituisce la lunghezza della lista

int posCliente(list l, Cliente cl); //restiruisce la posizione nella lista di un determinato ciente

Cliente getCliente(list l, int pos); //data una posizione, restituisce l'elemento in quella posizione

list reverseList_c(list l); //restituisce la lista invertia

void outputList_c(list l); //stampa tutti i nodi della lista