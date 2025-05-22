//FILE lista_lezioni.c

#include <stdio.h>
#include"lista_lezioni.h"
#include <stdlib.h>
#include <string.h>
#include "lezione.h"

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

list removeList(list l, int pos)
{
    int i = 0;
    list app = newList();
    list l2 = newList();
    Lezione x;

    if (emptyList(l))
    {
        return l;  // Se la lista è vuota, restituisci la lista vuota
    }

    while (!emptyList(l) && pos > i)
    {
        x = getFirst(l);
        app = consList(x, app);
        l = tailList(l);
        i++;
    }

    if (pos == i)
    {
        l2 = tailList(l);  // Prendi la parte successiva alla posizione da rimuovere
        while (!emptyList(app))
        {
            x = getFirst(app);
            l2 = consList(x, l2);  // Aggiungi i nodi precedenti alla lista successiva
            app = tailList(app);
        }
    }

    return l2;  // Restituisci la lista modificata
}

static list copiaLista(list l)
{
    list copia = newList();
    list temp = newList();

    // Copia l'originale in una lista temporanea (per mantenere ordine)
    while (!emptyList(l))
    {
        Lezione lezione = getFirst(l);
        temp = consList(lezione, temp);
        l = tailList(l);
    }

    // Inverti la temporanea per ottenere l'ordine originale
    while (!emptyList(temp))
    {
        Lezione lezione = getFirst(temp);
        copia = consList(lezione, copia);
        temp = tailList(temp);
    }

    return copia;
}

list ordina_Lista(list l)
{
    list copia = copiaLista(l); // Per non modificare l'originale
    list l_ord = newList();

    while (!emptyList(copia))
    {
        list tmp = copia;
        Lezione min = getFirst(copia);
        int pos = 0, i = 0;

        // Trova la lezione più imminente
        while (!emptyList(tmp))
        {
            Lezione curr = getFirst(tmp);
            if (confronta_lezioni(curr, min) < 0)
            {
                min = curr;
                pos = i;
            }
            tmp = tailList(tmp);
            i++;
        }

        l_ord = consList(min, l_ord);      // Aggiungi a lista ordinata
        copia = removeList(copia, pos);    // Rimuovi dalla copia
    }

    return reverseList(l_ord); // Riporta in ordine crescente
}

void outputList(list l)
{
    Lezione le;
    l = ordina_Lista(l);

    printf("===================================\n");
    printf("         Elenco Lezioni\n");

    while (!emptyList(l))
    {
        le = getFirst(l); 
        visualizza_lezione(le);
        l = tailList(l);
    }
}

void stampaMinimaList(list l)
{
    Lezione le;
    l = ordina_Lista(l);

    while (!emptyList(l))
    {
        le = getFirst(l); 
        visualizza_essenziale_lezione(le, get_posti_occupati(le), get_posti_max(le));
        l = tailList(l);
    }
}



