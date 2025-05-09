//FILE lezione.h

#include"data.h"

typedef struct lezione *Lezione;

Lezione crea_lezione(const char* id, const char* nome, Data dat, const char* orario, int posti_max);
void libera_lezione(Lezione l); //dealloca una lezione dalla memoria quando non serve piu'
void visualizza_lezione(Lezione l);
int prenota_lezione(Lezione l, Cliente c); //ritorna 1 se e' stata prenotata, 0 altrimenti
char* get_id_lezione(Lezione l);

