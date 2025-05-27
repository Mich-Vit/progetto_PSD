//FILE uitls.h

#ifndef UTILS_H
#define UTILS_H

#include "data.h"
#include "hash.h"
#include "cliente.h"
#include "lista_lezioni.h"
#include "hash_prenotazioni.h"

/*
* Funzione: rinnova_abbonamento
* -----------------------------
* Aggiorna l'abbonamento di un cliente nella tabella hash clienti.
*
* Parametri:
*   c: cliente di cui rinnovare l'abbonamento.
*   h: tabella hash contenente i clienti.
*
* Pre-condizione:
*   c e h devono essere validi.
*
* Post-condizione:
*   L'abbonamento del cliente viene aggiornato nella tabella.
*
* Ritorna:
*   Nessun valore (void).
*/
void rinnova_abbonamento(Cliente c, hashtable h);

/*
* Funzione: carica_contatore_generico
* ----------------------------------
* Legge da file il contatore massimo di ID che inizia con un certo prefisso.
* Questo serve per continuare la numerazione degli ID senza sovrapposizioni.
*
* Parametri:
*   filename: nome del file da cui leggere gli ID.
*   prefix: prefisso che identifica il tipo di ID (es. "ID: C").
*
* Pre-condizione:
*   filename e prefix devono essere validi e non NULL.
*
* Post-condizione:
*   Nessuna modifica esterna.
*
* Ritorna:
*   Il valore intero massimo trovato per il contatore degli ID con il prefisso dato,
*   o 0 se il file non esiste o non contiene ID validi.
*/
int carica_contatore_generico(const char* filename, const char* prefix);

/*
* Funzione: genera_id_generico
* ----------------------------
* Genera un nuovo ID univoco per clienti, lezioni o prenotazioni,
* basandosi su un prefisso e un file di contatori per mantenere la numerazione.
*
* Parametri:
*   prefix_letter: carattere che identifica il tipo di entità ('C' per clienti,
*                  'L' per lezioni, 'P' per prenotazioni).
*   filename: nome del file dove è salvato il contatore corrente.
*
* Pre-condizione:
*   prefix_letter deve essere uno tra 'C', 'L' o 'P'.
*   filename deve essere valido.
*
* Post-condizione:
*   Il contatore associato viene incrementato e salvato internamente.
*
* Ritorna:
*   Puntatore a una stringa dinamica contenente il nuovo ID generato.
*/
char* genera_id_generico(const char* prefix_letter, const char* filename, int reset);

/*
* Funzione: carica_clienti_da_file
* -------------------------------
* Carica i clienti letti da un file di testo nella tabella hash.
*
* Parametri:
*   h: tabella hash in cui inserire i clienti.
*
* Pre-condizione:
*   h deve essere valida.
*
* Post-condizione:
*   La tabella hash viene popolata con i clienti letti da file di testo.
*
* Ritorna:
*   Nessun valore (void).
*/
void carica_clienti_da_file(hashtable h);

/*
* Funzione: carica_lezioni_da_file
* -------------------------------
* Carica le lezioni lette da un file di testo in una lista.
*
* Parametri:
*   l: lista in cui inserire le lezioni caricate.
*
* Pre-condizione:
*   l deve essere valida (anche lista vuota).
*
* Post-condizione:
*   La lista viene popolata con le lezioni lette da file di testo.
*
* Ritorna:
*   La lista aggiornata con le lezioni caricate.
*/
list carica_lezioni_da_file(list l);

/*
* Funzione: carica_prenotazioni_da_file
* ------------------------------------
* Carica le prenotazioni lette da un file di testo nella tabella hash delle prenotazioni.
*
* Parametri:
*   h: tabella hash in cui inserire le prenotazioni.
*
* Pre-condizione:
*   h deve essere valida.
*
* Post-condizione:
*   La tabella hash viene popolata con le prenotazioni lette da file.
*
* Ritorna:
*   Nessun valore (void).
*/
void carica_prenotazioni_da_file(hashtable_p h);

/*
* Funzione: aggiorna_file_clienti
* -------------------------------
* Aggiorna il file di testo contenente i dati dei clienti con la tabella hash attuale.
*
* Parametri:
*   h: tabella hash contenente i clienti.
*
* Pre-condizione:
*   h deve essere valida.
*
* Post-condizione:
*   Il file di testo clienti viene sovrascritto con i dati aggiornati.
*
* Ritorna:
*   Nessun valore (void).
*/
void aggiorna_file_clienti(hashtable h);

/*
* Funzione: aggiorna_file_lezioni
* -------------------------------
* Aggiorna il file di testo contenente le lezioni con la lista attuale.
*
* Parametri:
*   l: lista delle lezioni.
*
* Pre-condizione:
*   l deve essere valida.
*
* Post-condizione:
*   Il file di testo lezioni viene sovrascritto con i dati aggiornati.
*
* Ritorna:
*   Nessun valore (void).
*/
void aggiorna_file_lezioni(list l);

/*
* Funzione: aggiorna_file_prenotazioni
* ------------------------------------
* Aggiorna il file di testo contenente le prenotazioni con la tabella hash attuale.
*
* Parametri:
*   h: tabella hash contenente le prenotazioni.
*
* Pre-condizione:
*   h deve essere valida.
*
* Post-condizione:
*   Il file di testo prenotazioni viene sovrascritto con i dati aggiornati.
*
* Ritorna:
*   Nessun valore (void).
*/
void aggiorna_file_prenotazioni(hashtable_p h);

/*
* Funzione: pulisci_schermo
* --------------------------
* Pulisce il terminale.
*
* Parametri:
*   Nessuno.
*
* Pre-condizione:
*   Nessuna.
*
* Post-condizione:
*   Lo schermo viene pulito.
*
* Ritorna:
*   Nessun valore (void).
*/
void pulisci_schermo();

/*
* Funzione: cerca_lezione_per_id
* ------------------------------
* Cerca una lezione nella lista tramite il suo ID.
*
* Parametri:
*   l: lista delle lezioni.
*   id_lezione: stringa contenente l'ID da cercare.
*
* Pre-condizione:
*   l e id_lezione devono essere validi.
*
* Post-condizione:
*   Nessuna modifica alla lista.
*
* Ritorna:
*   La lezione trovata o NULL se non esiste.
*/
Lezione cerca_lezione_per_id(list l, const char *id_lezione);

/*
* Funzione: stampa_lezioni_libere
* -------------------------------
* Stampa a video le lezioni che hanno posti disponibili.
*
* Parametri:
*   l: lista delle lezioni.
*
* Pre-condizione:
*   l deve essere valida.
*
* Post-condizione:
*   Vengono stampate le lezioni con posti liberi.
*
* Ritorna:
*   Nessun valore (void).
*/
void stampa_lezioni_libere(list l);

/*
* Funzione: calcola_durata_in_mesi
* --------------------------------
* Calcola la durata in mesi tra due date.
*
* Parametri:
*   data_inizio: data di inizio.
*   data_fine: data di fine.
*
* Pre-condizione:
*   Entrambe le date devono essere valide e data_fine >= data_inizio.
*
* Post-condizione:
*   Nessuna modifica alle date.
*
* Ritorna:
*   Numero intero che rappresenta la durata in mesi.
*/
int calcola_durata_in_mesi(Data data_inizio, Data data_fine);

/*
* Funzione: stampa_prenotazioni_cliente
* -------------------------------------
* Stampa tutte le prenotazioni di un cliente.
*
* Parametri:
*   c: cliente di cui stampare le prenotazioni.
*   hp: tabella hash delle prenotazioni.
*   l: lista delle lezioni.
*
* Pre-condizione:
*   c, hp e l devono essere validi.
*
* Post-condizione:
*   Vengono stampate tutte le prenotazioni relative al cliente.
*
* Ritorna:
*   Nessun valore (void).
*/
void stampa_prenotazioni_cliente(Cliente c, hashtable_p hp, list l);

/*
* Funzione: stampa_prenotazioni_lezione
* -------------------------------------
* Stampa tutte le prenotazioni associate a una specifica lezione.
*
* Parametri:
*   h: tabella hash clienti.
*   lezione: lezione di cui stampare le prenotazioni.
*   hp: tabella hash delle prenotazioni.
*
* Pre-condizione:
*   h, lezione e hp devono essere validi.
*
* Post-condizione:
*   Vengono stampate tutte le prenotazioni per la lezione specificata.
*
* Ritorna:
*   Nessun valore (void).
*/
void stampa_prenotazioni_lezione(hashtable h, Lezione lezione, hashtable_p hp);


/*
* Funzione: abbonamento_valido
* ----------------------------
* Verifica se un abbonamento è ancora valido rispetto alla data odierna.
*
* Parametri:
*   oggi: data attuale.
*   scadenza: data di scadenza dell'abbonamento.
*
* Pre-condizione:
*   Le date devono essere valide.
*
* Post-condizione:
*   Nessuna modifica alle date.
*
* Ritorna:
*   1 se l'abbonamento è valido, 0 altrimenti.
*/
int abbonamento_valido(Data oggi, Data scadenza);

/*
* Funzione: calcolo_scadenza_abbonamento
* --------------------------------------
* Calcola la data di scadenza di un abbonamento a partire
* da una data di inizio e una durata in mesi.
*
* Parametri:
*   data_inizio: data di inizio abbonamento.
*   durata_abbonamento: durata in mesi.
*
* Pre-condizione:
*   data_inizio deve essere valida e durata_abbonamento >= 0.
*
* Post-condizione:
*   Nessuna modifica alle date.
*
* Ritorna:
*   Data corrispondente alla scadenza dell'abbonamento.
*/
Data calcolo_scadenza_abbonamento(Data data_inizio, int durata_abbonamento);;

/*
* Funzione: leggi_intero
* ----------------------
* Legge un numero intero da input e verifica se sia effettivamente un numero intero.
*
* Parametri:
*   Nessuno.
*
* Pre-condizione:
*   Nessuna.
*
* Post-condizione:
*   Nessun effetto collaterale.
*
* Ritorna:
*   Intero letto dall'input.
*/
int leggi_intero();

/*
* Funzione: solo_lettere
* ----------------------
* Controlla se una stringa contiene solo lettere alfabetiche.
*
* Parametri:
*   s: stringa da controllare.
*
* Pre-condizione:
*   s deve essere valida (non NULL).
*
* Post-condizione:
*   Nessuna modifica alla stringa.
*
* Ritorna:
*   1 se la stringa contiene solo lettere, 0 altrimenti.
*/
int solo_lettere(char* s);

/*
* Funzione: nome_mese
* -------------------
* Restituisce il nome del mese corrispondente a un numero da 1 a 12.
*
* Parametri:
*   mese: numero intero del mese (1-12).
*
* Pre-condizione:
*   mese deve essere compreso tra 1 e 12.
*
* Post-condizione:
*   Nessuna.
*
* Ritorna:
*   Stringa con il nome del mese.
*/
const char* nome_mese(int mese);

#endif /* UTILS_H */

