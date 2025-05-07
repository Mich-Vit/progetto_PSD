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

void visualizza_cliente(Cliente c)
{
    if (c == NULL)
    {
        printf("Cliente non valido.\n");
        return;
    }

    printf("\nDettagli del Cliente:\n");
    printf("ID Cliente: %d\n", c->id);
    printf("Nome: %s\n", c->nome);
    printf("Cognome: %s\n", c->cognome);
    visualizza_abbonamento_cliente(c);
}

void visualizza_abbonamento_cliente(Cliente c)
{
    if (c == NULL)
    {
        printf("Cliente non valido.\n");
        return;
    }

    printf("\nDettagli abbonamento del cliente:\n");
    printf("Durata abbonamento: %d mesi\n", c->durata_abbonamento);
    printf("Data iscrizione: ");
    visualizza_data(c->data_iscrizione);  // Usa la funzione che stampa la data
    printf("Data scadenza: ");
    visualizza_data(c->data_scadenza);    // Usa la funzione che stampa la data
}

int confronta_clienti(Cliente c1, Cliente c2)
{
    if (c1 == NULL || c2 == NULL)
        return 0;  // Non uguali se uno dei due è NULL

    if (c1->id == c2->id)
        return 1;  // Uguali se l'ID coincide
    else
        return 0;  // Altrimenti diversi
}

Data get_data_scadenza(Cliente c)
{
    return c->data_scadenza;
}