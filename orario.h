//FILE orario.h

typedef struct orario *Orario;

Orario crea_orario(int ore, int min);
Orario copia_orario(Orario originale);
void stampa_orario(Orario o);

//Get per accedere
int get_ora(Orario o);
int get_minuti(Orario o);