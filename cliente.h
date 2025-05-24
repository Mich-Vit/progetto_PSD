// FILE cliente.h

#ifndef CLIENTE_H
#define CLIENTE_H

#include "data.h"

typedef struct cliente *Cliente;

/*
* Funzione: crea_cliente
* -----------------------
* Crea e inizializza un nuovo cliente con i dati specificati.
*
* Parametri:
*   id: identificativo univoco del cliente
*   nome: nome del cliente
*   cognome: cognome del cliente
*   durata: durata dell'abbonamento in mesi
*   data_iscrizione: data di iscrizione del cliente
*
* Pre-condizione:
*   I parametri devono essere validi e non NULL.
*
* Post-condizione:
*   Viene restituito un nuovo oggetto Cliente allocato dinamicamente.
*
* Ritorna:
*   Un puntatore alla struttura Cliente appena creata.
*/
Cliente crea_cliente(char* id, char* nome, char* cognome, int durata, Data data_iscrizione);

/*
* Funzione: distruggi_cliente
* ---------------------------
* Dealloca la memoria associata a un oggetto Cliente.
*
* Parametri:
*   c: puntatore al Cliente da deallocare
*
* Pre-condizione:
*   Il puntatore c deve essere valido.
*
* Post-condizione:
*   La memoria associata al Cliente è liberata.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void distruggi_cliente(Cliente c);

/*
* Funzione: salva_cliente_file
* ----------------------------
* Salva le informazioni del cliente su file di testo.
*
* Parametri:
*   c: puntatore al Cliente da salvare
*
* Pre-condizione:
*   Il cliente deve essere valido.
*
* Post-condizione:
*   I dati del cliente sono scritti su file di testo.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void salva_cliente_file(Cliente c);

/*
* Funzione: visualizza_cliente
* ----------------------------
* Visualizza tutte le informazioni relative a un cliente
*
* Parametri:
*   c: puntatore al Cliente di cui visualizzare le informazioni.
*
* Pre-condizione:
*   Il puntatore c deve essere valido (non NULL).
*
* Post-condizione:
*   Tutti i dati del cliente vengono stampati a video.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void visualizza_cliente(Cliente c);

/*
* Funzione: visualizza_abbonamento_cliente
* ----------------------------------------
* Visualizza i dettagli dell'abbonamento di un cliente.
*
* Parametri:
*   c: puntatore al Cliente di cui visualizzare l'abbonamento.
*
* Pre-condizione:
*   Il puntatore c deve essere valido (non NULL).
*
* Post-condizione:
*   Le informazioni relative all'abbonamento vengono stampate a video.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void visualizza_abbonamento_cliente(Cliente c);

/*
* Funzione: visualizza_essenziale_cliente
* ---------------------------------------
* Visualizza solo le informazioni essenziali del cliente (ID, nome e cognome).
*
* Parametri:
*   c: puntatore al Cliente di cui visualizzare i dati essenziali.
*
* Pre-condizione:
*   Il puntatore c deve essere valido (non NULL).
*
* Post-condizione:
*   Le informazioni principali del cliente vengono stampate a video.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void visualizza_essenziale_cliente(Cliente c);

/*
* Funzione: confronta_clienti
* ---------------------------
* Confronta due clienti in base al loro identificativo.
*
* Parametri:
*   c1: primo cliente
*   c2: secondo cliente
*
* Pre-condizione:
*   Entrambi i clienti devono essere validi.
*
* Post-condizione:
*   Nessuna modifica ai dati.
*
* Ritorna:
*   1 se i clienti sono uguali, 0 altrimenti.
*/
int confronta_clienti(Cliente c1, Cliente c2);

/*
* Funzione: get_id_cliente
* ------------------------
* Restituisce l'identificativo univoco del cliente.
*
* Parametri:
*   c: puntatore al Cliente da cui ottenere l'ID.
*
* Pre-condizione:
*   Il puntatore c deve essere valido (non NULL).
*
* Post-condizione:
*   Nessuna modifica ai dati.
*
* Ritorna:
*   Una stringa contenente l'ID del cliente.
*/
char* get_id_cliente(Cliente c);

/*
* Funzione: get_nome_cliente
* --------------------------
* Restituisce il nome del cliente.
*
* Parametri:
*   c: puntatore al Cliente da cui ottenere il nome.
*
* Pre-condizione:
*   Il puntatore c deve essere valido (non NULL).
*
* Post-condizione:
*   Nessuna modifica ai dati.
*
* Ritorna:
*   Una stringa contenente il nome del cliente.
*/
char* get_nome_cliente(Cliente c);

/*
* Funzione: get_cognome_cliente
* -----------------------------
* Restituisce il cognome del cliente.
*
* Parametri:
*   c: puntatore al Cliente da cui ottenere il cognome.
*
* Pre-condizione:
*   Il puntatore c deve essere valido (non NULL).
*
* Post-condizione:
*   Nessuna modifica ai dati.
*
* Ritorna:
*   Una stringa contenente il cognome del cliente.
*/
char* get_cognome_cliente(Cliente c);

/*
* Funzione: get_durata_abbonamento
* --------------------------------
* Restituisce la durata dell'abbonamento del cliente.
*
* Parametri:
*   c: puntatore al Cliente da cui ottenere la durata.
*
* Pre-condizione:
*   Il puntatore c deve essere valido (non NULL).
*
* Post-condizione:
*   Nessuna modifica ai dati.
*
* Ritorna:
*   Un intero che rappresenta la durata in mesi.
*/
int get_durata_abbonamento(Cliente c);

/*
* Funzione: get_data_iscrizione
* -----------------------------
* Restituisce la data di iscrizione del cliente.
*
* Parametri:
*   c: puntatore al Cliente da cui ottenere la data.
*
* Pre-condizione:
*   Il puntatore c deve essere valido (non NULL).
*
* Post-condizione:
*   Nessuna modifica ai dati.
*
* Ritorna:
*   Un oggetto di tipo Data rappresentante la data di iscrizione.
*/
Data get_data_iscrizione(Cliente c);

/*
* Funzione: get_data_scadenza
* ---------------------------
* Restituisce la data di scadenza dell'abbonamento del cliente.
*
* Parametri:
*   c: puntatore al Cliente da cui ottenere la data di scadenza.
*
* Pre-condizione:
*   Il puntatore c deve essere valido (non NULL).
*
* Post-condizione:
*   Nessuna modifica ai dati.
*
* Ritorna:
*   Un oggetto di tipo Data rappresentante la data di scadenza.
*/
Data get_data_scadenza(Cliente c);

/*
* Funzione: get_next_cliente
* --------------------------
* Restituisce il puntatore al cliente successivo nella struttura (tabella hash).
*
* Parametri:
*   c: puntatore al Cliente corrente.
*
* Pre-condizione:
*   Il puntatore c deve essere valido.
*
* Post-condizione:
*   Nessuna modifica ai dati.
*
* Ritorna:
*   Un puntatore al cliente successivo, o NULL se non esiste.
*/
Cliente get_next_cliente(Cliente c);

/*
* Funzione: set_next_cliente
* --------------------------
* Imposta il cliente successivo nella struttura dati (tabella hash).
*
* Parametri:
*   c: cliente corrente
*   next: puntatore al prossimo cliente
*
* Pre-condizione:
*   Entrambi i puntatori devono essere validi, o next può essere NULL.
*
* Post-condizione:
*   Il campo next del cliente viene aggiornato.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void set_next_cliente(Cliente c, Cliente next);

/*
* Funzione: set_data_scadenza
* ---------------------------
* Imposta una nuova data di scadenza per l'abbonamento del cliente.
*
* Parametri:
*   c: puntatore al Cliente
*   nuova_data: nuova data di scadenza
*
* Pre-condizione:
*   I parametri devono essere validi.
*
* Post-condizione:
*   La data di scadenza del cliente viene aggiornata.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void set_data_scadenza(Cliente c, Data nuova_data);

/*
* Funzione: set_durata
* --------------------
* Modifica la durata dell'abbonamento del cliente.
*
* Parametri:
*   c: puntatore al Cliente
*   nuova_durata: nuova durata dell'abbonamento (in mesi)
*
* Pre-condizione:
*   Il cliente deve essere valido. La nuova durata deve essere >= 0.
*
* Post-condizione:
*   La durata dell'abbonamento viene aggiornata.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void set_durata(Cliente c, int nuova_durata);

#endif /* CLIENTE_H */
