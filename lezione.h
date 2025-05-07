//FILE lezione.h

#include"data.h"

typedef struct lezione *Lezione;

Lezione crea_Lezione(int id, const char* nome, Data dat, const char* orario, int posti_max);
void libera_lezione(Lezione l); //dealloca una lezione dalla memoria quando non serve piu'
void visualizza_lezione(Lezione l);
int prenota_lezioe(Data oggi, Cliente c); //ritorna 1 se e' stata prenotata, 0 altrimenti