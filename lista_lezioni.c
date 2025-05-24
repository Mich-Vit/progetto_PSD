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

/*
* Funzione: newList
* ----------------------------------------
* Crea una nuova lista vuota.
*
* Parametri:
*   Nessuno.
*
* Pre-condizione:
*   Nessuna.
*
* Post-condizione:
*   Restituisce una lista vuota (NULL).
*
* Come funziona:
* - Ritorna NULL per rappresentare una lista vuota.
*/
list newList(void)
{
    return NULL;
}

/*
* Funzione: emptyList
* ----------------------------------------
* Verifica se una lista è vuota.
*
* Parametri:
*   l: lista da controllare.
*
* Pre-condizione:
*   Lista valida.
*
* Post-condizione:
*   Ritorna 1 se la lista è vuota, 0 altrimenti.
*
* Come funziona:
* - Controlla se il puntatore alla lista è NULL.
*/
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

/*
* Funzione: consList
* ----------------------------------------
* Aggiunge una lezione all'inizio della lista.
*
* Parametri:
*   le: la Lezione da aggiungere.
*   l: la lista a cui aggiungere la lezione.
*
* Pre-condizione:
*   Lezione valida.
*
* Post-condizione:
*   Restituisce la nuova lista con la lezione in testa.
*
* Come funziona:
* - Alloca dinamicamente un nuovo nodo.
* - Assegna la lezione al nuovo nodo.
* - Collega il nuovo nodo alla lista esistente.
* - Ritorna il nuovo nodo come testa della lista.
*/
list consList(Lezione le, list l)
{
    struct node *nuovo = malloc(sizeof(struct node));

    if (nuovo != NULL)
    {
        nuovo->lez = le;      
        nuovo->next = l;  // Collega il nuovo nodo alla lista esistente
        l = nuovo;        // Il nuovo nodo diventa la testa della lista
    }
    else
    {
        printf("Errore allocazione memoria in consList\n");
        exit(1);
    }

    return l;
}

/*
* Funzione: tailList
* ----------------------------------------
* Restituisce la coda della lista, escludendo il primo elemento.
*
* Parametri:
*   l: lista originale.
*
* Pre-condizione:
*   Lista valida o NULL.
*
* Post-condizione:
*   Restituisce la lista senza la testa (primo nodo).
*
* Come funziona:
* - Se la lista è NULL, ritorna NULL.
* - Altrimenti, ritorna il campo "next" del primo nodo.
*/
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

/*
* Funzione: getFirst
* ----------------------------------------
* Ottiene la prima lezione nella lista.
*
* Parametri:
*   l: lista da cui prelevare la lezione.
*
* Pre-condizione:
*   Lista non NULL.
*
* Post-condizione:
*   Restituisce la lezione contenuta nel primo nodo della lista.
*
* Come funziona:
* - Se la lista è NULL, ritorna NULL.
* - Altrimenti, ritorna il campo "lez" del primo nodo.
*/
Lezione getFirst(list l)
{
    if (l == NULL)
    {
        return NULL; 
    }
    return l->lez; 
}

/*
* Funzione: sizeList
* ----------------------------------------
* Calcola la lunghezza della lista.
*
* Parametri:
*   l: lista da misurare.
*
* Pre-condizione:
*   Lista valida o NULL.
*
* Post-condizione:
*   Ritorna il numero di elementi nella lista.
*
* Come funziona:
* - Scorre tutta la lista contando i nodi uno ad uno.
* - Ritorna il conteggio.
*/
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

/*
* Funzione: reverseList
* ----------------------------------------
* Inverte l'ordine degli elementi nella lista.
*
* Parametri:
*   l: lista da invertire.
*
* Pre-condizione:
*   Lista valida.
*
* Post-condizione:
*   Ritorna una nuova lista con gli stessi elementi in ordine inverso.
*
* Come funziona:
* - Scorre la lista originale.
* - Aggiunge ciascun elemento in testa a una nuova lista.
* - Ritorna la nuova lista.
*/
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

/*
* Funzione: removeList
* ----------------------------------------
* Rimuove l'elemento in posizione specificata dalla lista.
*
* Parametri:
*   l: lista originale.
*   pos: posizione (zero-based) dell'elemento da rimuovere.
*
* Pre-condizione:
*   Lista valida, posizione >= 0.
*
* Post-condizione:
*   Ritorna la lista senza l’elemento alla posizione indicata.
*
* Come funziona:
* - Copia i nodi precedenti alla posizione in una lista temporanea.
* - Salta il nodo da rimuovere.
* - Ricompone la lista con i nodi rimanenti.
*/
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

/*
* Funzione: copiaLista
* ----------------------------------------
* Crea una copia della lista preservando l'ordine degli elementi.
*
* Parametri:
*   l: lista da copiare.
*
* Pre-condizione:
*   Lista valida.
*
* Post-condizione:
*   Ritorna una nuova lista con le stesse lezioni in ordine originale.
*
* Come funziona:
* - Inverte la lista copiandola in ordine inverso (temp).
* - Poi la reinverte per ottenere l’ordine originale (copia).
*/
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

/*
* Funzione: ordina_Lista
* ----------------------------------------
* Ordina la lista delle lezioni per data e orario crescenti.
*
* Parametri:
*   l: lista da ordinare.
*
* Pre-condizione:
*   Lista valida.
*
* Post-condizione:
*   Ritorna una nuova lista ordinata.
*
* Come funziona:
* - Copia la lista originale.
* - Seleziona ripetutamente la lezione "minima" (più imminente).
* - La rimuove dalla copia e la inserisce in una nuova lista ordinata.
* - Alla fine, inverte la lista ordinata per ottenere l'ordine corretto.
*/
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

/*
* Funzione: outputList
* ----------------------------------------
* Stampa tutte le lezioni della lista in formato completo.
*
* Parametri:
*   l: lista delle lezioni.
*
* Pre-condizione:
*   Lista valida.
*
* Post-condizione:
*   Le lezioni vengono stampate su console in ordine cronologico.
*
* Come funziona:
* - Ordina la lista.
* - Scorre la lista e stampa ogni lezione con la funzione `visualizza_lezione`.
*/
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

/*
* Funzione: stampaMinimaList
* ----------------------------------------
* Stampa le lezioni in formato compatto (essenziale).
*
* Parametri:
*   l: lista delle lezioni.
*
* Pre-condizione:
*   Lista valida.
*
* Post-condizione:
*   Le lezioni vengono stampate su console in formato tabellare ridotto.
*
* Come funziona:
* - Ordina la lista.
* - Scorre la lista e stampa ogni lezione con la funzione visualizza_essenziale_lezione.
*/
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



