//FILE orario.h

typedef struct orario *Orario;

/*
* Funzione: crea_orario
* ---------------------
* Crea un nuovo oggetto Orario con ore e minuti specificati.
*
* Parametri:
*   ore: valore dell'ora (0–23).
*   min: valore dei minuti (0–59).
*
* Pre-condizione:
*   ore deve essere compreso tra 0 e 23;
*   min deve essere compreso tra 0 e 59.
*
* Post-condizione:
*   Viene restituito un nuovo oggetto Orario allocato in memoria.
*
* Ritorna:
*   Puntatore a un nuovo oggetto Orario.
*/
Orario crea_orario(int ore, int min);

/*
* Funzione: copia_orario
* ----------------------
* Crea una copia esatta dell'orario specificato.
*
* Parametri:
*   originale: orario da copiare.
*
* Pre-condizione:
*   Il puntatore originale deve essere valido.
*
* Post-condizione:
*   Nessuna modifica all'orario originale.
*
* Ritorna:
*   Puntatore a un nuovo oggetto Orario identico all'originale.
*/
Orario copia_orario(Orario originale);

/*
* Funzione: stampa_orario
* -----------------------
* Stampa a video l'orario nel formato HH:MM.
*
* Parametri:
*   o: puntatore all'orario da stampare.
*
* Pre-condizione:
*   Il puntatore o deve essere valido.
*
* Post-condizione:
*   L'orario viene stampato su standard output.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void stampa_orario(Orario o);

/*
* Funzione: confronta_orario
* --------------------------
* Confronta due orari in base a ore e minuti.
*
* Parametri:
*   o1: primo orario da confrontare.
*   o2: secondo orario da confrontare.
*
* Pre-condizione:
*   I puntatori o1 e o2 devono essere validi.
*
* Post-condizione:
*   Nessuna modifica agli orari.
*
* Ritorna:
*   - -1 se o1 è prima di o2,
*   -  0 se o1 è uguale a o2,
*   -  1 se o1 è successivo a o2.
*/
int confronta_orario(Orario o1, Orario o2);

/*
* Funzione: libera_orario
* -----------------------
* Dealloca la memoria occupata da un oggetto Orario.
*
* Parametri:
*   o: puntatore all'orario da deallocare.
*
* Pre-condizione:
*   Il puntatore o deve essere valido.
*
* Post-condizione:
*   La memoria occupata dall'orario viene liberata.
*
* Ritorna:
*   Nessun valore (funzione void).
*/
void libera_orario(Orario o);

/*
* Funzione: get_ora
* -----------------
* Restituisce il valore delle ore dell'orario.
*
* Parametri:
*   o: puntatore all'orario.
*
* Pre-condizione:
*   Il puntatore o deve essere valido.
*
* Post-condizione:
*   Nessuna modifica all'orario.
*
* Ritorna:
*   Valore intero corrispondente alle ore (0–23), -1 se o è non valido.
*/
int get_ora(Orario o);

/*
* Funzione: get_minuti
* --------------------
* Restituisce il valore dei minuti dell'orario.
*
* Parametri:
*   o: puntatore all'orario.
*
* Pre-condizione:
*   Il puntatore o deve essere valido.
*
* Post-condizione:
*   Nessuna modifica all'orario.
*
* Ritorna:
*   Valore intero corrispondente ai minuti (0–59), -1 se o è non valido.
*/
int get_minuti(Orario o);