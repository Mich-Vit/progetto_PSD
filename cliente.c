//FILE cliente.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"data.h"
#include"cliente.h"


struct cliente
{
    int id;                      // ID univoco cliente
    char nome[50];
    char cognome[50];
    int durata_abbonamento;     // in mesi
    Data data_iscrizione;       // Inizio iscrizione
    Data data_scadenza;         // Validità dell’abbonamento
};

Cliente crea_cliente(int id, char* nome, char* cognome, int durata, Data data_is)
{
    Cliente c = malloc(sizeof(struct cliente));
    if(c == NULL) //controllo malloc
    {
        printf("Errore di allocazione memoria per la data.\n");
        exit(1);  // Uscita in caso di errore di memoria
    }

    c->id = id;
    strcpy(c->nome, nome);
    strcpy(c->cognome, cognome);
    c->durata_abbonamento = durata;

    // Usa la funzione creaData per allocare e inizializzare la data_iscrizione
    c->data_iscrizione = copia_data(data_is);

    // Calcola data di scadenza in base alla durata
    c->data_scadenza = calcolo_scadenza_abbonamento(data_is, durata);

    return c;
}