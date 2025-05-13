//FILE data.h

typedef struct data *Data;

Data crea_data(int gg, int mm, int aa);
void visualizza_data(Data data);
Data calcolo_scadenza_abbonamento(Data data_inizio, int durata_abbonamento);
int abbonamento_valido(Data oggi, Data scadenza); // Ritorna 1 se e' valido, 0 altrimenti
int confronta_date(Data d1, Data d2);// -1 se d1 < d2, 0 se d1 == d2, 1 se d1 > d2
Data copia_data(Data originale);
void libera_data(Data d); //dealloca una data che non serve piu'
Data data_oggi(); //da la vera data odierna

// Funzioni di accesso (getter)
int get_giorno(Data d);
int get_mese(Data d);
int get_anno(Data d);