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


// === Getter ===

int get_ora(Orario o)
{
    return o->ore;
}

int get_minuti(Orario o)
{
    return o->minuti;
}

