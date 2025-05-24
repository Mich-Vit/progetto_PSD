//FILE orario.c

#include<stdio.h>
#include<stdlib.h>
#include "orario.h"

struct orario
{
    int minuti; 
    int ore;
};

/*
* Funzione: crea_orario
* ----------------------------------------
* Crea un nuovo oggetto Orario con valori validi.
*
* Parametri:
*   ore: valore dell’ora (0-23).
*   min: valore dei minuti (0-59).
*
* Pre-condizione:
*   ore deve essere tra 0 e 23, min tra 0 e 59.
*
* Post-condizione:
*   Restituisce un puntatore a un nuovo Orario, oppure NULL se i valori sono invalidi.
*
* Come funziona:
* - Verifica che ore e minuti siano nei range validi.
* - Alloca memoria per un nuovo Orario.
* - Assegna i valori e restituisce il puntatore.
*/
Orario crea_orario(int ore, int min)
{
    if (ore < 0 || ore > 23 || min < 0 || min > 59)
    {
        printf("Errore: orario non valido (%02d:%02d).\n", ore, min);
        return NULL;
    }

    Orario o = malloc(sizeof(struct orario));
    if (o == NULL){
        printf("Errore di allocazione memoria per Orario.\n");
        exit(1);
    }

    o->ore = ore;
    o->minuti = min;
    return o;
}

/*
* Funzione: copia_orario
* ----------------------------------------
* Crea una copia indipendente di un oggetto Orario.
*
* Parametri:
*   originale: puntatore all'orario da copiare.
*
* Pre-condizione:
*   originale non deve essere NULL.
*
* Post-condizione:
*   Restituisce un nuovo Orario con gli stessi valori dell’originale.
*
* Come funziona:
* - Alloca nuova memoria.
* - Copia ore e minuti dal parametro originale.
* - Restituisce la nuova copia.
*/
Orario copia_orario(Orario originale)
{
    if (originale == NULL)
        return NULL;

    Orario copia = malloc(sizeof(struct orario));
    if (copia == NULL)
    {
        printf("Errore di allocazione memoria per copia orario.\n");
        exit(1);
    }

    copia->ore = originale->ore;
    copia->minuti = originale->minuti;
    return copia;
}

/*
* Funzione: stampa_orario
* ----------------------------------------
* Stampa un oggetto Orario nel formato HH:MM.
*
* Parametri:
*   o: puntatore all’orario da stampare.
*
* Pre-condizione:
*   o deve essere valido (non NULL).
*
* Post-condizione:
*   L'orario viene stampato su standard output.
*
* Come funziona:
* - Se l’orario è NULL, stampa messaggio di errore.
* - Altrimenti, stampa ore e minuti con formato %02d.
*/
void stampa_orario(Orario o)
{
    if (o == NULL)
    {
        printf("Orario non valido");
        return;
    }
    printf("%02d:%02d", o->ore, o->minuti);
}

/*
* Funzione: confronta_orario
* ----------------------------------------
* Confronta due orari.
*
* Parametri:
*   o1, o2: puntatori ai due orari da confrontare.
*
* Pre-condizione:
*   Entrambi gli orari devono essere validi.
*
* Post-condizione:
*   Restituisce:
*   - -1 se o1 è prima di o2,
*   -  0 se o1 è uguale a o2,
*   -  1 se o1 è successivo a o2.
*
* Come funziona:
* - Confronta prima le ore.
* - Se le ore sono uguali, confronta i minuti.
* - Restituisce il risultato della comparazione.
*/
int confronta_orario(Orario o1, Orario o2)
{
    if (o1 == NULL || o2 == NULL)
    {
        printf("Errore: uno dei due orari è NULL.\n");
        return 0;
    }

    // Confronta le ore
    if (o1->ore < o2->ore)
    {
        return -1;  // o1 è prima
    }
    else if (o1->ore > o2->ore)
    {
        return 1;   // o1 è dopo
    }
    else
    {
        // Ore uguali, confronto minuti
        if (o1->minuti < o2->minuti)
        {
            return -1; // o1 è prima
        }
        else if (o1->minuti > o2->minuti)
        {
            return 1;  // o1 è dopo
        }
        else
        {
            return 0;  // orari identici
        }
    }
}

/*
* Funzione: libera_orario
* ----------------------------------------
* Libera la memoria allocata per un orario.
*
* Parametri:
*   o: puntatore all’orario da liberare.
*
* Pre-condizione:
*   o può essere NULL o valido.
*
* Post-condizione:
*   Se o è valido, la memoria viene liberata.
*
* Come funziona:
* - Se il puntatore è diverso da NULL, chiama free(o).
*/
void libera_orario(Orario o)
{
    if (o != NULL)
    {
        free(o);
    }
}

/*
* Funzione: get_ora
* ----------------------------------------
* Restituisce le ore di un oggetto Orario.
*
* Parametri:
*   o: puntatore all’orario.
*
* Pre-condizione:
*   o deve essere valido.
*
* Post-condizione:
*   Ritorna il valore delle ore, -1 se o non è valido.
*
* Come funziona:
* - Ritorna valore intero corrispondente alle ore (0–23), -1 se o è non valido.
*/
int get_ora(Orario o)
{
    if(o == NULL)
        return -1;
    return o->ore;
}

/*
* Funzione: get_minuti
* ----------------------------------------
* Restituisce i minuti di un oggetto Orario.
*
* Parametri:
*   o: puntatore all’orario.
*
* Pre-condizione:
*   o deve essere valido.
*
* Post-condizione:
*   Ritorna il valore dei minuti.
*
* Come funziona:
* - Ritorna valore intero corrispondente ai minuti (0–59), -1 se o è non valido.
*/
int get_minuti(Orario o)
{
    if(o == NULL)
        return -1;
    return o->minuti;
}

