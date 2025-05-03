//FILE cliente.h

typedef struct cliente *Cliente;

Cliente crea_cliente(int id, char* nome, char* cognome, int durata, Data data_iscrizione);
// Alloca e inizializza un nuovo cliente, calcola la scadenza in base alla durata



