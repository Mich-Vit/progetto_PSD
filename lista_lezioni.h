//FILE lista_lezioni.h

#ifndef LISTA_LEZIONI_H
#define LISTA_LEZIONI_H

#include"lezione.h"

typedef struct node *list;

/*
* Funzione: newList
* -----------------
* Crea e restituisce una nuova lista vuota di lezioni.
*
* Parametri:
*   Nessuno.
*
* Pre-condizione:
*   Nessuna.
*
* Post-condizione:
*   La lista è inizializzata e vuota (l = NULL).
*
* Ritorna:
*   Una lista vuota (NULL).
*/
list newList(void);

/*
* Funzione: emptyList
* -------------------
* Verifica se una lista è vuota.
*
* Parametri:
*   l: puntatore alla lista.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   se l = NULL allora restituisce 1, altrimenti 0
*
* Ritorna:
*   1 se la lista è vuota, 0 altrimenti.
*/
int emptyList(list l);

/*
* Funzione: consList
* ------------------
* Aggiunge una nuova lezione all'inizio della lista.
*
* Parametri:
*   le: lezione da inserire.
*   l: lista a cui aggiungere la lezione.
*
* Pre-condizione:
*   Il puntatore alla lezione deve essere valido.
*
* Post-condizione:
*   La lezione viene inserita in testa alla lista.
*
* Ritorna:
*   La nuova lista con la lezione aggiunta.
*/
list consList(Lezione le, list l);

/*
* Funzione: tailList
* ------------------
* Restituisce la coda della lista, ovvero tutti gli elementi escluso il primo.
*
* Parametri:
*   l: puntatore alla lista.
*
* Pre-condizione:
*   Il puntatore l deve essere valido e la lista non deve essere vuota:
*   l = <a1, a2, …, an>,  n>0 .
*
* Post-condizione:
*   Nessuna modifica alla lista originale, ma restituisce una nuova lista l':
*   l’ = <a2, …, an>
*
* Ritorna:
*   Una lista senza il primo elemento.
*/
list tailList(list l);

/*
* Funzione: getFirst
* ------------------
* Restituisce la prima lezione della lista.
*
* Parametri:
*   l: puntatore alla lista.
*
* Pre-condizione:
*   Il puntatore l deve essere valido e la lista non deve essere vuota:
*   l = <a1, a2, …, an>, n>0 
*
* Post-condizione:
*   Nessuna modifica alla lista, ma inizializza la nuova lezione = a1 
*
* Ritorna:
*   La prima lezione nella lista.
*/
Lezione getFirst(list l);

/*
* Funzione: sizeList
* ------------------
* Calcola il numero di elementi presenti nella lista.
*
* Parametri:
*   l: puntatore alla lista.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla lista.
*
* Ritorna:
*   Numero di elementi nella lista.
*/
int sizeList(list l);

/*
* Funzione: reverseList
* ---------------------
* Restituisce una nuova lista con gli elementi in ordine inverso.
*
* Parametri:
*   l: puntatore alla lista originale.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Nessuna modifica alla lista originale.
*
* Ritorna:
*   Una nuova lista contenente gli stessi elementi in ordine inverso.
*/
list reverseList(list l); 

/*
* Funzione: removeList
* --------------------
* Rimuove l'elemento in posizione specificata dalla lista.
*
* Parametri:
*   l: puntatore alla lista.
*   pos: posizione dell'elemento da rimuovere (0-based).
*
* Pre-condizione:
*   Il puntatore l deve essere valido;
*   pos deve essere >= 0 e < sizeList(l).
*
* Post-condizione:
*   L'elemento alla posizione indicata viene rimosso dalla lista.
*
* Ritorna:
*   La lista aggiornata dopo la rimozione.
*/
list removeList(list l, int pos);

/*
* Funzione: outputList
* --------------------
* Stampa a video tutte le lezioni contenute nella lista.
*
* Parametri:
*   l: puntatore alla lista.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Le informazioni di tutte le lezioni vengono stampate a video.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void outputList(list l);

/*
* Funzione: stampaMinimaList
* --------------------------
* Stampa le informazioni essenziali delle lezioni nella lista.
*
* Parametri:
*   l: puntatore alla lista.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Le informazioni minime di ciascuna lezione vengono stampate.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void stampaMinimaList(list l);

/*
* Funzione: ordina_Lista
* ----------------------
* Ordina le lezioni presenti nella lista per data e orario.
*
* Parametri:
*   l: puntatore alla lista.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   Gli elementi della lista vengono riordinati per data e ora.
*
* Ritorna:
*   Una nuova lista ordinata.
*/
list ordina_Lista(list l);

#endif /* LISTA_LEZIONI_H */