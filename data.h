//FILE data.h

typedef struct data *Data;

Data creaData(int gg, int mm, int aa);
void visualizza_data(Data data);
Data calcolo_scadenza_abbonamento(Data data_inizio, int durata_abbonamento);

