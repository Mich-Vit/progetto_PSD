//FILE lezione.h

typedef struct lezione *Lezione;

Lezione crea_Lezione(int id, const char* nome, Data dat, const char* orario, int posti_max);
void libera_lezione(Lezione l); //dealloca una lezione dalla memoria quando non serve più