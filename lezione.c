//FILE lezione.c

#include<stdio.h>
#include<stdlib.h>
#include"lezione.h"

struct lezione
{
    int id;                     // ID lezione
    char nome[50];             // es. "Zumba", "Pilates"
    char giorno[10];           // es. "Lun", "Mar", "Mer"
    char orario[6];            // es. "18:30"
    int posti_max;             // capacità totale
    int posti_occupati;        // quanti sono già prenotati
};
