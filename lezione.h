//FILE lezione.h

#ifndef LEZIONE_H
#define LEZIONE_H

#include"data.h"
#include "orario.h"

typedef struct lezione *Lezione;

/*
* Funzione: crea_lezione
* ----------------------
* Crea e inizializza una nuova lezione con i dati forniti.
*
* Parametri:
*   id: identificativo univoco della lezione.
*   nome: nome della lezione.
*   dat: data della lezione.
*   orario: orario della lezione.
*   posti_max: numero massimo di posti disponibili.
*
* Pre-condizione:
*   Tutti i parametri devono essere validi;
*   posti_max > 0.
*
* Post-condizione:
*   Viene restituito un nuovo oggetto Lezione correttamente allocato.
*
* Ritorna:
*   Puntatore alla nuova lezione.
*/
Lezione crea_lezione(const char* id, const char* nome, Data dat, Orario orario, int posti_max);

/*
* Funzione: salva_lezione_file
* ----------------------------
* Salva le informazioni di una lezione su file di testo.
*
* Parametri:
*   l: puntatore alla lezione da salvare.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Le informazioni della lezione vengono scritte su file di testo.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void salva_lezione_file(Lezione l);

/*
* Funzione: libera_lezione
* ------------------------
* Dealloca una lezione dalla memoria quando non serve più.
*
* Parametri:
*   l: puntatore alla lezione da deallocare.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   La memoria occupata dalla lezione viene liberata.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void libera_lezione(Lezione l); //dealloca una lezione dalla memoria quando non serve piu'

/*
* Funzione: visualizza_lezione
* ----------------------------
* Stampa tutte le informazioni di una lezione.
*
* Parametri:
*   l: puntatore alla lezione da visualizzare.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Le informazioni della lezione vengono stampate.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void visualizza_lezione(Lezione l);

/*
* Funzione: visualizza_essenziale_lezione
* ---------------------------------------
* Stampa le informazioni essenziali di una lezione.
*
* Parametri:
*   le: puntatore alla lezione.
*   posti_occupati: numero di posti attualmente occupati.
*   posti_max: numero massimo di posti disponibili.
*
* Pre-condizione:
*   Il puntatore le deve essere valido;
*   posti_occupati >= 0;
*   posti_max > 0.
*
* Post-condizione:
*   Le informazioni minime della lezione vengono stampate.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void visualizza_essenziale_lezione(Lezione le, int posti_occupati, int posti_max);

/*
* Funzione: confronta_lezioni
* ---------------------------
* Confronta due lezioni in base alla data e, in caso di parità, all'orario.
*
* Parametri:
*   l1: puntatore alla prima lezione.
*   l2: puntatore alla seconda lezione.
*
* Pre-condizione:
*   Entrambi i puntatori devono essere validi.
*
* Post-condizione:
*   Nessuna modifica alle lezioni.
*
* Ritorna:
*   Un valore intero che indica l'ordine tra le lezioni:
*     - valore negativo se l1 è prima di l2,
*     - 0 se le lezioni hanno stessa data e orario,
*     - valore positivo se l1 è dopo l2.
*/
int confronta_lezioni(Lezione l1, Lezione l2);

/*
* Funzione: get_id_lezione
* ------------------------
* Restituisce l'ID della lezione.
*
* Parametri:
*   l: puntatore alla lezione.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Stringa contenente l'ID della lezione, NULL se la lezione non è valida.
*/
char* get_id_lezione(Lezione l);

/*
* Funzione: get_posti_occupati
* ----------------------------
* Restituisce il numero di posti occupati nella lezione.
*
* Parametri:
*   l: puntatore alla lezione.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Numero intero dei posti occupati, -1 se la lezione non è valida.
*/
int get_posti_occupati(Lezione l);

/*
* Funzione: get_posti_max
* -----------------------
* Restituisce il numero massimo di posti della lezione.
*
* Parametri:
*   l: puntatore alla lezione.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Numero intero dei posti massimi disponibili, -1 se la lezione non è valida.
*/
int get_posti_max(Lezione l);

/*
* Funzione: get_nome_lezione
* --------------------------
* Restituisce il nome della lezione.
*
* Parametri:
*   l: puntatore alla lezione.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Stringa contenente il nome della lezione, NULL se la lezione non è valida.
*/
char* get_nome_lezione(Lezione l);

/*
* Funzione: get_data_lezione
* --------------------------
* Restituisce la data della lezione.
*
* Parametri:
*   l: puntatore alla lezione.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Oggetto Data relativo alla lezione, NULL se la lezione non è valida.
*/
Data get_data_lezione(Lezione l);

/*
* Funzione: get_ora_lezione
* -------------------------
* Restituisce l'orario della lezione.
*
* Parametri:
*   l: puntatore alla lezione.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Oggetto Orario relativo alla lezione, NULL se la lezione non è valida.
*/
Orario get_ora_lezione(Lezione l);

/*
* Funzione: set_posti_occupati
* ----------------------------
* Imposta il numero di posti occupati per una lezione.
*
* Parametri:
*   l: puntatore alla lezione.
*   pos_occupati: nuovo valore dei posti occupati.
*
* Pre-condizione:
*   Il puntatore l deve essere valido;
*   pos_occupati >= 0.
*
* Post-condizione:
*   Il numero di posti occupati nella lezione viene aggiornato.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void set_posti_occupati(Lezione l, int pos_occupati);

#endif /* LEZIONE_H */

