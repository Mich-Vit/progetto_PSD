//FILE data.h

typedef struct data *Data;

Data creaData(int gg, int mm, int aa);
void visualizza_data(Data data);
Data calcolo_scadenza_abbonamento(Data data_inizio, int durata_abbonamento);
int abbonamento_valido(Data oggi, Data scadenza); // Ritorna 1 se e' valido, 0 altrimenti
int confronta_date(Data d1, Data d2);// -1 se d1 < d2, 0 se d1 == d2, 1 se d1 > d2

