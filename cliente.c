//FILE cliente.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"data.h"
#include"cliente.h"

// === Definizione della struttura ===

struct cliente
{
    char *id;                    // ID univoco cliente (dinamico)
    char nome[50];
    char cognome[50];
    int durata_abbonamento;      // in mesi
    Data data_iscrizione;
    Data data_scadenza;
    struct cliente *next;
};


Cliente crea_cliente(char* id, char* nome, char* cognome, int durata, Data data_is)
{
    Cliente c = malloc(sizeof(struct cliente));
    if (c == NULL)
    {
        printf("Errore di allocazione memoria per il cliente.\n");
        exit(1);
    }

    // Allocazione dinamica per ID
    c->id = malloc(strlen(id) + 1);
    if (c->id == NULL)
    {
        printf("Errore di allocazione memoria per l'ID del cliente.\n");
        free(c);
        exit(1);
    }
    strcpy(c->id, id);

    strcpy(c->nome, nome);
    strcpy(c->cognome, cognome);
    c->durata_abbonamento = durata;
    c->data_iscrizione = copia_data(data_is);
    c->data_scadenza = calcolo_scadenza_abbonamento(data_is, durata);
    c->next = NULL;

    return c;
}

void distruggi_cliente(Cliente c)
{
    if (c != NULL)
    {
        free(c->id);
        libera_data(c->data_iscrizione);
        libera_data(c->data_scadenza);
        free(c);
    }
}

void visualizza_cliente(Cliente c)
{
    if (c == NULL)
    {
        printf("Cliente non valido.\n");
        return;
    }

    printf("\nDettagli del Cliente:\n");
    printf("ID Cliente: %s\n", c->id);
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
    visualizza_data(c->data_iscrizione);
    printf("Data scadenza: ");
    visualizza_data(c->data_scadenza);
}

int confronta_clienti(Cliente c1, Cliente c2)
{
    if (c1 == NULL || c2 == NULL)
        return 0;  // Non uguali se uno dei due è NULL

    if (strcmp(c1->id, c2->id) == 0)
        return 1;  // Uguali se l'ID coincide
    else
        return 0;  // Altrimenti diversi
}

// === Getter ===

Data get_data_scadenza(Cliente c)
{
    return c->data_scadenza;
}

char* get_id_cliente(Cliente c)
{
    return c->id;
}

Cliente get_next_cliente(Cliente c)
{
    if (c == NULL)
    {
        return NULL;
    } 
    else
    {
        return c->next;
    }
}

char* get_nome_cliente(Cliente c)
{
    if (c == NULL) return NULL;
    return c->nome;
}

char* get_cognome_cliente(Cliente c)
{
    if (c == NULL) return NULL;
    return c->cognome;
}

int get_durata_abbonamento(Cliente c)
{
    if (c == NULL) return -1; // oppure un valore sentinella adeguato
    return c->durata_abbonamento;
}

Data get_data_iscrizione(Cliente c)
{
    if (c == NULL) return NULL;
    return c->data_iscrizione;
}

// === Setter ===

void set_next_cliente(Cliente c, Cliente next)
{
    if (c != NULL) {
        c->next = next;
    }
}

