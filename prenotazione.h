//FILE prenotazione.h

#ifndef PRENOTAZIONE_H
#define PRENOTAZIONE_H

#include"cliente.h"
#include"data.h"
#include"lezione.h"

typedef struct prenotazione *Prenotazione;

Prenotazione crea_prenotazione(char *id_pren, char* id_cli, char* id_lez, Data data_prenotazione);
void libera_prenotazione(Prenotazione p);  // Funzione che libera la memoria allocata per la prenotazione
void visualizza_prenotazione(Prenotazione p);
void visualizza_essenziale_prenotazione(Prenotazione p);
void salva_prenotazione_file(Prenotazione p);

char* get_id_prenotazione(Prenotazione p);
char* get_id_cliente_prenotazione(Prenotazione p);
char* get_id_lezione_prenotazione(Prenotazione p);
Data get_data_prenotazione(Prenotazione p);
Prenotazione get_next_prenotazione(Prenotazione p);
void set_next_prenotazione(Prenotazione p, Prenotazione next);

#endif /* PRENOTAZIONE_H */