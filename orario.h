//FILE orario.h

typedef struct orario *Orario;

Orario crea_orario(int ore, int min);
Orario copia_orario(Orario originale);
void stampa_orario(Orario o);
int confronta_orario(Orario o1, Orario o2); // -1 se o1< o2, 0 se o1== o2, 1 se o1> o2
void libera_orario(Orario o);

//Get per accedere
int get_ora(Orario o);
int get_minuti(Orario o);