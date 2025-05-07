#include <stdio.h>
#include "lista_clienti.h"
#include <stdlib.h>

struct node
{
    Cliente c;
    struct node *next;
};


list newList(void)
{
    return NULL;
}

int emptyList(list l)
{
    if(l == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


list consList(Cliente cl, list l)
{
    struct node *nuovo = malloc(sizeof(struct node));

    if (nuovo != NULL)
    {
        nuovo->c = cl;       // Assegna il cliente al nodo
        nuovo->next = l;     // Collega il nuovo nodo alla lista esistente
        l = nuovo;           // Il nuovo nodo diventa la testa della lista
    }
    else
    {
        printf("Errore allocazione memoria in consList\n");
        exit(1);
    }

    return l;
}

list tailList(list l)
{
    list temp;
    if(l == NULL)
    {
        temp = NULL;
        return temp;
    }
    temp = l->next;
    
    return temp;
}

Cliente getFirst(list l)
{
    if (l == NULL)
    {
        return NULL; 
    }
    return l->c;  // Restituisce il cliente nella testa della lista
}


int sizeList(list l)
{
    int n = 0;  // Inizializza n a zero
    while (l != NULL)
    {
        n++;
        l = tailList(l);
    }
    return n;
}

int posCliente(list l, Cliente cl)
{
    int pos = 0;
    int found = 0;

    while (l != NULL && !found)
    {
        if (confronta_clienti(getFirst(l), cl))  //confronta_clienti restituisca 1 se sono uguali
            found = 1;
        else
        {
            pos++;
            l = tailList(l);
        }
    }

    if (!found)
        pos = -1;  // Se non trovato, restituisce -1

    return pos;
}

Cliente getCliente(list l, int pos)
{
    int i = 0;
    
    if (pos < 0)  // Se la posizione è negativa, restituisci NULL
    {
        return NULL; 
    }

    while (l != NULL && i < pos)
    {
        i++;
        l = l->next;
    }

    if (l != NULL)
    {
        return l->c;  // Restituisce il cliente nella posizione trovata
    }
    else
    {
        return NULL;  // Restituisci NULL se la posizione non esiste
    }
}


list reverseList (list l)
{
    list rev = newList();
    Cliente cl;
    
    while(!emptyList(l))
    {
        cl = getFirst(l);
        rev = consList(cl, rev);
        l = tailList(l);
    }
    
    return rev;
}

void outputList(list l)
{
    int i = 0;
    Cliente cl;

    while (!emptyList(l))
    {
        cl = getFirst(l); 
        printf("\nElemento di posizione %d:\n", i);
        visualizza_cliente(cl);  // Usa la funzione specifica per il cliente
         
        l = tailList(l);
        i++;
    }
}