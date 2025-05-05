#include <stdio.h>
#include "lista_clienti.h"


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
    struct node *new = malloc(sizeof(struct node));
    if(new == NULL)
    {
        printf("Errore\n");
        exit(0);
    }
    
    if(l == NULL)
    {
        return l;
    }
    
    new->c = cl;
    new->next = l;
    l->next = new;
    
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

Cliente getFirst (list l)
{
    Cliente cl;
    
    if(l == NULL)
    {
        return l;
    }
    
    cl = l->c;
    return cl;
}

int sizeList (list l)
{
    int n;
    
    if(l == NULL)
    {
        return l;
    }
    
    while(!emptyList(l))
    {
        n++;
        
        l = tailList(l);
    }
    
    return n;
}

int posCliente (list l, Cliente cl)
{
    int pos = 0;
    int found = 0;

    while (!emptyList(l) && !found)
    {
        if (confronta_clienti(getFirst(l), cl))
            found = 1;
        else
        {
            pos++;
            l = tailList(l);
        }
    }

    if (!found)
        pos = -1; // se non trovato restituiamo -1

    return pos;
}


Cliente getCliente (list l, int pos) //data una posizione, restituisce l'elemento in quella posizione
{
    Cliente cl;
    int i = 0;
    
    if(pos < 0)
    {
        return -1;
    }
    
    while (!emptyList(l) && i < pos)
    {
        i++;
        l=tailList(l);
    }
    
    if (!emptyList(l)) // se la lista ha almeno pos+1 elementi
        cl = getFirst(l); // elemento di posizione pos
    else
        cl = NULLCLIENTE;
    
    return cl;
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