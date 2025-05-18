// FILE cliente.h

#ifndef CLIENTE_H
#define CLIENTE_H

#include "data.h"

typedef struct cliente *Cliente;

Cliente crea_cliente(char* id, char* nome, char* cognome, int durata, Data data_iscrizione);
void distruggi_cliente(Cliente c);

void salva_cliente_file(Cliente c);

// Visualizzazione
void visualizza_cliente(Cliente c);
void visualizza_abbonamento_cliente(Cliente c);
void visualizza_essenziale_cliente(Cliente c);

// Confronto
int confronta_clienti(Cliente c1, Cliente c2);

// Funzioni di accesso (getter)
char* get_id_cliente(Cliente c);
char* get_nome_cliente(Cliente c);
char* get_cognome_cliente(Cliente c);
int get_durata_abbonamento(Cliente c);
Data get_data_iscrizione(Cliente c);
Data get_data_scadenza(Cliente c);
Cliente get_next_cliente(Cliente c);

// Funzioni di modifica (setter)
void set_next_cliente(Cliente c, Cliente next);
void set_data_scadenza(Cliente c, Data nuova_data);
void set_durata(Cliente c, int nuova_durata);

#endif /* CLIENTE_H */
