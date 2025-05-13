//FILE prenotazione.h

#include"cliente.h"
#include"data.h"
#include"lezione.h"

typedef struct prenotazione *Prenotazione;

Prenotazione crea_prenotazione(Cliente c, Lezione l, Data data_prenotazione);
void libera_prenotazione(Prenotazione p);  // Funzione che libera la memoria allocata per la prenotazione
