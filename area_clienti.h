//FILE area_clienti.h

#ifndef AREA_CLIENTI_H
#define AREA_CLIENTI_H

#include "cliente.h"
#include "lista_lezioni.h"
#include "hash.h"
#include "hash_prenotazioni.h"

/*
* Funzione: login_cliente
* -----------------------
* Gestisce la procedura di login per un cliente.
*
* Parametri:
*   h: hashtable contenente i dati dei clienti registrati.
*
* Pre-condizione:
*   La tabella hash 'h' deve essere inizializzata e contenere eventuali clienti.
*
* Post-condizione:
*   Viene restituito un oggetto Cliente corrispondente alle credenziali inserite,
*   se il login ha successo.
*
* Ritorna:
*   Il cliente autenticato (di tipo Cliente).
*/
Cliente login_cliente(hashtable h);

/*
* Funzione: prenota_lezione 
* -----------------------------------
* Permette a un cliente di prenotare una lezione.
*
* Parametri:
*   c: cliente autenticato che vuole prenotare.
*   l: lista delle lezioni disponibili.
*   h: hashtable delle prenotazioni.
*
* Pre-condizione:
*   Il cliente deve avere un abbonamento valido.
*   l e h devono essere valide.
*
* Post-condizione:
*   Viene creata una nuova prenotazione se possibile.
*   Vengono aggiornati i posti occupati della lezione 
*   e aggiorna il file di testo con tutte le lezioni aggiornate.
*/
void prenota_lezione(Cliente c, list l, hashtable_p h);

/*
* Funzione: menu_cliente
* ----------------------
* Mostra e gestisce il menu riservato a un cliente autenticato, consentendo
* l'interazione con il sistema (visualizzazione lezioni, prenotazioni, ecc.).
*
* Parametri:
*   c: struttura contenente i dati del cliente autenticato.
*   h: hashtable dei clienti.
*   l: lista delle lezioni disponibili.
*   hp: hashtable delle prenotazioni.
*
* Pre-condizione:
*   Il cliente deve essere correttamente autenticato.
*   Le strutture dati devono essere inizializzate correttamente.
*
* Post-condizione:
*   Il sistema aggiorna eventuali modifiche fatte dal cliente (es. prenotazioni).
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void menu_cliente(Cliente c, hashtable h, list l, hashtable_p hp);

#endif /* AREA_CLIENTI_H */