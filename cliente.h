//FILE cliente.h

#include"data.h"
#define NULLCLIENTE 0;

typedef struct cliente *Cliente;

Cliente crea_cliente(int id, char* nome, char* cognome, int durata, Data data_iscrizione);
// Alloca e inizializza un nuovo cliente, calcola la scadenza in base alla durata
void visualizza_cliente(Cliente c); //stampa i dati di un cliente
void visualizza_abbonamento_cliente(Cliente c); //funzione dedicata ai dettagli dell'abbonamento del cliente
int confronta_clienti(Cliente c1, Cliente c2); //ritorna 1 se i due id dei clienti sono uguali, 0 altrimenti
Data get_data_scadenza(Cliente c);


