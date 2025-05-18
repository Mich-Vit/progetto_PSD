//FILE lezione.h

#ifndef LEZIONE_H
#define LEZIONE_H

#include"data.h"
#include "orario.h"

typedef struct lezione *Lezione;

Lezione crea_lezione(const char* id, const char* nome, Data dat, Orario orario, int posti_max);

void salva_lezione_file(Lezione l);

void libera_lezione(Lezione l); //dealloca una lezione dalla memoria quando non serve piu'

void visualizza_lezione(Lezione l);
void visualizza_essenziale_lezione(Lezione le, int posti_occupati, int posti_max);

int confronta_lezioni(Lezione l1, Lezione l2);

char* get_id_lezione(Lezione l);
int get_posti_occupati(Lezione l);
int get_posti_max(Lezione l);
char* get_nome_lezione(Lezione l);
Data get_data_lezione(Lezione l);
Orario get_ora_lezione(Lezione l);

void set_posti_occupati(Lezione l, int pos_occupati);

#endif /* LEZIONE_H */

