//FILE cliente.c

#include<stdio.h>
#include<stdlib.h>
#include"cliente.h"
#include"data.h"

struct cliente
{
    int id;                      // ID univoco cliente
    char nome[50];
    char cognome[50];
    int durata_abbonamento;     // in mesi
    Data data_iscrizione;       // Inizio iscrizione
    Data data_scadenza;         // Validità dell’abbonamento
};
