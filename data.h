//FILE data.h

#ifndef DATA_H
#define DATA_H

typedef struct data *Data;

/*
* Funzione: crea_data
* -------------------
* Crea e inizializza un oggetto Data con giorno, mese e anno specificati.
*
* Parametri:
*   gg: giorno (1-31)
*   mm: mese (1-12)
*   aa: anno
*
* Pre-condizione:
*   I valori devono rappresentare una data valida.
*
* Post-condizione:
*   Viene creato un nuovo oggetto Data allocato dinamicamente.
*
* Ritorna:
*   Un puntatore alla struttura Data creata.
*/
Data crea_data(int gg, int mm, int aa);

/*
* Funzione: visualizza_data
* -------------------------
* Visualizza la data nel formato gg/mm/aaaa.
*
* Parametri:
*   data: puntatore alla struttura Data da visualizzare.
*
* Pre-condizione:
*   Il puntatore deve essere valido.
*
* Post-condizione:
*   La data viene stampata su output.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void visualizza_data(Data data);

/*
* Funzione: confronta_date
* ------------------------
* Confronta due date tra loro.
*
* Parametri:
*   d1: prima data
*   d2: seconda data
*
* Pre-condizione:
*   Entrambe le date devono essere valide.
*
* Post-condizione:
*   Nessuna modifica ai dati.
*
* Ritorna:
*   -1 se d1 < d2, 0 se d1 == d2, 1 se d1 > d2.
*/
int confronta_date(Data d1, Data d2);

/*
* Funzione: leggi_data
* --------------------
* Legge una data da input utente.
*
* Parametri:
*   Nessuno.
*
* Pre-condizione:
*   L'input deve essere fornito in modo corretto (gg/mm/aaaa);
*   La data inserita deve essere valida.
*
* Post-condizione:
*   Viene creata una nuova struttura Data contenente la data letta.
*
* Ritorna:
*   Un puntatore alla Data letta.
*/
Data leggi_data();

/*
* Funzione: copia_data
* --------------------
* Crea una copia indipendente di una data esistente.
*
* Parametri:
*   originale: puntatore alla data da copiare.
*
* Pre-condizione:
*   Il puntatore originale deve essere valido.
*
* Post-condizione:
*   Viene restituita una nuova struttura Data con gli stessi valori.
*
* Ritorna:
*   Puntatore alla copia della data.
*/
Data copia_data(Data originale);

/*
* Funzione: libera_data
* ---------------------
* Dealloca la memoria occupata da una struttura Data.
*
* Parametri:
*   d: puntatore alla data da liberare.
*
* Pre-condizione:
*   Il puntatore d deve essere valido.
*
* Post-condizione:
*   La memoria occupata dalla data è liberata.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void libera_data(Data d);

/*
* Funzione: data_oggi
* -------------------
* Restituisce la data odierna.
*
* Parametri:
*   Nessuno.
*
* Pre-condizione:
*   Nessuna.
*
* Post-condizione:
*   Viene restituito un oggetto Data contenente la data attuale.
*
* Ritorna:
*   Puntatore alla Data odierna.
*/
Data data_oggi();

/*
* Funzione: get_giorno
* --------------------
* Restituisce il giorno della data passata come parametro.
*
* Parametri:
*   d: puntatore alla struttura Data.
*
* Pre-condizione:
*   Il puntatore d deve essere valido.
*
* Post-condizione:
*   Nessuna modifica ai dati.
*
* Ritorna:
*   Un intero che rappresenta il giorno.
*/
int get_giorno(Data d);

/*
* Funzione: get_mese
* ------------------
* Restituisce il mese della data passata come parametro.
*
* Parametri:
*   d: puntatore alla struttura Data.
*
* Pre-condizione:
*   Il puntatore d deve essere valido.
*
* Post-condizione:
*   Nessuna modifica ai dati.
*
* Ritorna:
*   Un intero che rappresenta il mese.
*/
int get_mese(Data d);

/*
* Funzione: get_anno
* ------------------
* Restituisce l'anno della data passata come parametro.
*
* Parametri:
*   d: puntatore alla struttura Data.
*
* Pre-condizione:
*   Il puntatore d deve essere valido.
*
* Post-condizione:
*   Nessuna modifica ai dati.
*
* Ritorna:
*   Un intero che rappresenta l'anno.
*/
int get_anno(Data d);

#endif /* DATA_H */