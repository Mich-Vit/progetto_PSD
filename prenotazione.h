//FILE prenotazione.h

#ifndef PRENOTAZIONE_H
#define PRENOTAZIONE_H

#include"cliente.h"
#include"data.h"
#include"lezione.h"

typedef struct prenotazione *Prenotazione;

/*
* Funzione: crea_prenotazione
* ---------------------------
* Crea una nuova prenotazione con i dati specificati.
*
* Parametri:
*   id_pren: identificativo univoco della prenotazione.
*   id_cli: identificativo del cliente.
*   id_lez: identificativo della lezione.
*   data_prenotazione: data in cui viene effettuata la prenotazione.
*
* Pre-condizione:
*   Tutti i parametri devono essere validi e non NULL.
*
* Post-condizione:
*   Viene creato un nuovo oggetto Prenotazione.
*
* Ritorna:
*   Puntatore alla prenotazione appena creata.
*/
Prenotazione crea_prenotazione(char *id_pren, char* id_cli, char* id_lez, Data data_prenotazione);

/*
* Funzione: libera_prenotazione
* -----------------------------
* Dealloca la memoria occupata da una prenotazione.
*
* Parametri:
*   p: puntatore alla prenotazione da liberare.
*
* Pre-condizione:
*   Il puntatore p deve essere valido.
*
* Post-condizione:
*   La memoria della prenotazione viene rilasciata.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void libera_prenotazione(Prenotazione p);  

/*
* Funzione: visualizza_prenotazione
* ---------------------------------
* Stampa a video tutte le informazioni della prenotazione.
*
* Parametri:
*   p: puntatore alla prenotazione da visualizzare.
*
* Pre-condizione:
*   Il puntatore p deve essere valido.
*
* Post-condizione:
*   Le informazioni vengono stampate su standard output.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void visualizza_prenotazione(Prenotazione p);

/*
* Funzione: visualizza_essenziale_prenotazione
* --------------------------------------------
* Stampa le informazioni essenziali di una prenotazione.
*
* Parametri:
*   p: puntatore alla prenotazione da visualizzare.
*
* Pre-condizione:
*   Il puntatore p deve essere valido.
*
* Post-condizione:
*   Vengono stampate solo le informazioni principali.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void visualizza_essenziale_prenotazione(Prenotazione p);

/*
* Funzione: salva_prenotazione_file
* ---------------------------------
* Salva una prenotazione su file di testo.
*
* Parametri:
*   p: puntatore alla prenotazione da salvare.
*
* Pre-condizione:
*   Il puntatore p deve essere valido.
*
* Post-condizione:
*   Le informazioni della prenotazione vengono scritte su file di testo.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void salva_prenotazione_file(Prenotazione p);

/*
* Funzione: get_id_prenotazione
* -----------------------------
* Restituisce l'ID della prenotazione.
*
* Parametri:
*   p: puntatore alla prenotazione.
*
* Pre-condizione:
*   Il puntatore p deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Stringa contenente l'ID della prenotazione.
*/
char* get_id_prenotazione(Prenotazione p);

/*
* Funzione: get_id_cliente_prenotazione
* -------------------------------------
* Restituisce l'ID del cliente associato alla prenotazione.
*
* Parametri:
*   p: puntatore alla prenotazione.
*
* Pre-condizione:
*   Il puntatore p deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Stringa contenente l'ID del cliente associato alla prenotazione.
*/
char* get_id_cliente_prenotazione(Prenotazione p);

/*
* Funzione: get_id_lezione_prenotazione
* -------------------------------------
* Restituisce l'ID della lezione associata alla prenotazione.
*
* Parametri:
*   p: puntatore alla prenotazione.
*
* Pre-condizione:
*   Il puntatore p deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Stringa contenente l'ID della lezione alla prenotazione.
*/
char* get_id_lezione_prenotazione(Prenotazione p);

/*
* Funzione: get_data_prenotazione
* -------------------------------
* Restituisce la data in cui è stata effettuata la prenotazione.
*
* Parametri:
*   p: puntatore alla prenotazione.
*
* Pre-condizione:
*   Il puntatore p deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Oggetto Data associato alla prenotazione.
*/
Data get_data_prenotazione(Prenotazione p);

/*
* Funzione: get_next_prenotazione
* -------------------------------
* Restituisce il puntatore alla prossima prenotazione nella lista.
*
* Parametri:
*   p: puntatore alla prenotazione corrente.
*
* Pre-condizione:
*   Il puntatore p deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla struttura.
*
* Ritorna:
*   Puntatore alla prossima prenotazione, o NULL se non esiste.
*/
Prenotazione get_next_prenotazione(Prenotazione p);

/*
* Funzione: set_next_prenotazione
* -------------------------------
* Imposta il puntatore alla prossima prenotazione nella lista.
*
* Parametri:
*   p: puntatore alla prenotazione corrente.
*   next: puntatore alla prossima prenotazione da associare.
*
* Pre-condizione:
*   Entrambi i puntatori devono essere validi o NULL.
*
* Post-condizione:
*   Il campo next della prenotazione viene aggiornato.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void set_next_prenotazione(Prenotazione p, Prenotazione next);

#endif /* PRENOTAZIONE_H */