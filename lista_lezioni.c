//FILE lista_lezioni.c

#include <stdio.h>
#include"lista_lezioni.h"
#include <stdlib.h>

struct node
{
    Lezione lez;
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


list consList(Lezione le, list l)
{
    struct node *nuovo = malloc(sizeof(struct node));

    if (nuovo != NULL)
    {
        nuovo->lez = le;       // Assegna la lezione al nodo
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

Lezione getFirst(list l)
{
    if (l == NULL)
    {
        return NULL; 
    }
    return l->lez;  // Restituisce la lezione nella testa della lista
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

list reverseList(list l)
{
    list rev = newList();
    Lezione le;
    
    while(!emptyList(l))
    {
        le = getFirst(l);
        rev = consList(le, rev);
        l = tailList(l);
    }
    
    return rev;
}

void outputList(list l)
{
    int i = 0;
    Lezione le;

    printf("===================================\n");
    printf("         Elenco Lezioni\n");

    while (!emptyList(l))
    {
        le = getFirst(l); 
        visualizza_lezione(le);
        l = tailList(l);
        i++;
    }
}