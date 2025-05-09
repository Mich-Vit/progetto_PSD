//FILE cliente.h

#include"data.h"
#define NULLCLIENTE NULL

typedef struct cliente *Cliente;

Cliente crea_cliente(char* id, char* nome, char* cognome, int durata, Data data_iscrizione);
void visualizza_cliente(Cliente c);
void visualizza_abbonamento_cliente(Cliente c);
int confronta_clienti(Cliente c1, Cliente c2);
Data get_data_scadenza(Cliente c);
char* get_id_cliente(Cliente c);
void distruggi_cliente(Cliente c);