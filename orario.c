//FILE orario.c

#include<stdio.h>
#include<stdlib.h>
#include "orario.h"

struct orario
{
    int minuti; 
    int ore;
};

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

void stampa_orario(Orario o)
{
    if (o == NULL)
    {
        printf("Orario non valido");
        return;
    }
    printf("%02d:%02d", o->ore, o->minuti);
}

int confronta_orario(Orario o1, Orario o2) // -1 se o1< o2, 0 se o1== o2, 1 se o1> o2
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

void libera_orario(Orario o)
{
    if (o != NULL)
    {
        free(o);
    }
}


// === Getter ===

int get_ora(Orario o)
{
    return o->ore;
}

int get_minuti(Orario o)
{
    return o->minuti;
}

