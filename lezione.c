//FILE lezione.c

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"data.h"
#include"lezione.h"
#include "orario.h"

struct lezione
{
    char* id;                   // ID lezione (ora è una stringa dinamica)
    char nome[50];              // es. "Zumba", "Pilates"
    Data data;                  // La data della lezione (giorno, mese, anno)
    Orario ora_lezione;            // es. "18:30"
    int posti_max;              // capacità totale
    int posti_occupati;         // quanti sono già prenotati
};

/*
* Funzione: crea_lezione
* ----------------------------------------
* Crea una nuova lezione con i dati forniti, allocando memoria dinamicamente.
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
*   Restituisce un puntatore a una struttura Lezione inizializzata.
*
* Come funziona:
* - Alloca memoria per una nuova struttura lezione.
* - Copia dinamicamente l'ID.
* - Copia nome, data e orario usando funzioni dedicate.
* - Imposta i posti occupati inizialmente a zero.
*/
Lezione crea_lezione(const char* id, const char* nome, Data dat, Orario orario, int posti_max)
{
    Lezione nuova_lezione = malloc(sizeof(struct lezione));
    if (nuova_lezione == NULL) 
    {
        printf("Errore di allocazione memoria per la lezione.\n");
        exit(1); 
    }

    nuova_lezione->id = malloc(strlen(id) + 1);
    if (nuova_lezione->id == NULL)
    {
        printf("Errore di allocazione memoria per l'ID della lezione.\n");
        free(nuova_lezione);
        exit(1);
    }
    strcpy(nuova_lezione->id, id);

    strcpy(nuova_lezione->nome, nome);
    nuova_lezione->data = copia_data(dat);
    nuova_lezione->ora_lezione = copia_orario(orario);
    nuova_lezione->posti_max = posti_max;
    nuova_lezione->posti_occupati = 0;

    return nuova_lezione;
}

/*
* Funzione: salva_lezione_file
* ----------------------------------------
* Salva i dati di una lezione su file di testo "lezioni.txt" in modalità append.
*
* Parametri:
*   l: puntatore alla lezione da salvare.
*
* Pre-condizione:
*   Il puntatore l deve essere valido.
*
* Post-condizione:
*   I dati della lezione sono scritti nel file "lezioni.txt".
*
* Come funziona:
* - Apre il file in modalità append.
* - Scrive ID, nome, data, orario, posti max e posti occupati.
* - Chiude il file.
*/
void salva_lezione_file(Lezione l)
{
    FILE *fp = fopen("lezioni.txt", "a");
    if (fp == NULL)
    {
        printf("Errore nell'aprire il file lezioni.txt.\n");
        return;
    }

    if(l == NULL)
    {
        return;
    }

    fprintf(fp, "ID: %s\n", l->id);
    fprintf(fp, "Nome: %s\n", l->nome);
    fprintf(fp, "Data: %02d/%02d/%04d\n",
            get_giorno(l->data), get_mese(l->data), get_anno(l->data));

    fprintf(fp, "Orario: %02d:%02d\n", get_ora(l->ora_lezione), get_minuti(l->ora_lezione));

    fprintf(fp, "Posti massimi: %d\n", l->posti_max);
    fprintf(fp, "Posti occupati: %d\n", l->posti_occupati);
    fprintf(fp, "-----------------------\n");

    fclose(fp);
}

/*
* Funzione: libera_lezione
* ----------------------------------------
* Libera la memoria associata a una lezione.
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
* Come funziona:
* - Libera l'ID.
* - Libera data e orario con funzioni specifiche.
* - Libera la struttura stessa.
*/
void libera_lezione(Lezione l) 
{
    if (l != NULL) 
    {
        free(l->id);              
        libera_data(l->data);      
        libera_orario(l->ora_lezione); 
        free(l);                
    }
}

/*
* Funzione: visualizza_lezione
* ----------------------------------------
* Visualizza tutte le informazioni di una lezione in formato leggibile.
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
* Come funziona:
* - Stampa ID, nome, data, orario, posti occupati e posti disponibili.
* - Indica se la lezione è al completo.
*/
void visualizza_lezione(Lezione l) 
{
    if (l == NULL) 
    {
        printf("Lezione non valida.\n");
        return;
    }

    printf("===================================\n");
    printf("ID Lezione: %s\n", l->id);
    printf("Nome: %s\n", l->nome);
    printf("Data: ");
    visualizza_data(l->data);
    printf("Orario: ");
    stampa_orario(l->ora_lezione);
    printf("\nPosti occupati: %d/%d\n", l->posti_occupati, l->posti_max);

    if (l->posti_occupati < l->posti_max)
    {
        printf("Posti disponibili: %d\n", l->posti_max - l->posti_occupati);
    }
    else
    {
        printf("Lezione al completo.\n");
    }
}

/*
* Funzione: confronta_lezioni
* ----------------------------------------
* Confronta due lezioni in base a data e orario.
*
* Parametri:
*   l1, l2: puntatori alle due lezioni da confrontare
*
* Pre-condizione:
*   Entrambe le lezioni devono essere valide.
*
* Post-condizione:
*   Ritorna:
*     - valore negativo se l1 è prima di l2,
*     - 0 se le lezioni hanno stessa data e orario,
*     - valore positivo se l1 è dopo l2.
*
* Come funziona:
* - Confronta prima la data.
* - Se le date sono uguali, confronta l'orario.
*/
int confronta_lezioni(Lezione l1, Lezione l2)
{
    int cmp_data = confronta_date(l1->data, l2->data);
    if (cmp_data != 0) //non sono uguali le date
        return cmp_data;

    //se le date sono uguali confronta l'orario
    return confronta_orario(l1->ora_lezione, l2->ora_lezione);
}

/*
* Funzione: visualizza_essenziale_lezione
* ----------------------------------------
* Visualizza una riga riassuntiva della lezione.
*
* Parametri:
*   le: lezione da visualizzare
*   posti_occupati: numero di partecipanti attualmente iscritti
*   posti_max: numero massimo di partecipanti consentiti
*
* Pre-condizione:
*   La lezione deve essere valida.
*
* Post-condizione:
*   Le informazioni minime della lezione vengono stampate.
*
* Come funziona:
* - Estrae orario e data in formato stringa.
* - Stampa ID, nome, orario, data, posti occupati/posti max.
*/
void visualizza_essenziale_lezione(Lezione le, int posti_occupati, int posti_max)
{
    char buffer_ora[6]; 
    char buffer_data[11]; 

    snprintf(buffer_ora, sizeof(buffer_ora), "%02d:%02d", get_ora(le->ora_lezione), get_minuti(le->ora_lezione));
    snprintf(buffer_data, sizeof(buffer_data), "%02d/%02d/%04d", get_giorno(le->data), get_mese(le->data), get_anno(le->data));

    // Stampa allineata con posti disponibili
    printf("%-8s %-15s %-10s %-12s %d/%d\n", le->id, le->nome, buffer_ora, buffer_data, posti_occupati, posti_max);
}

/*
* Funzione: get_id_lezione
* ----------------------------------------
* Restituisce l'ID di una lezione.
*
* Parametri:
*   l: puntatore alla lezione
*
* Pre-condizione:
*   La lezione deve essere valida.
*
* Post-condizione:
*   Ritorna una stringa con l'ID, oppure NULL.
*
* Come funziona:
* - Ritorna direttamente il campo `id`, oppure NULL.
*/
char* get_id_lezione(Lezione l)
{
    if (l == NULL)
        return NULL;
    return l->id; 
}

/*
* Funzione: get_posti_occupati
* ----------------------------------------
* Restituisce il numero di posti occupati nella lezione.
*
* Parametri:
*   l: puntatore alla lezione
*
* Pre-condizione:
*   La lezione deve essere valida.
*
* Post-condizione:
*   Ritorna il numero intero dei posti occupati, o -1 se la lezione non è valida.
*
* Come funziona:
* - Legge il campo `posti_occupati`.
*/
int get_posti_occupati(Lezione l)
{
    if (l == NULL)
        return -1;
    return l->posti_occupati; 
}

/*
* Funzione: get_posti_max
* ----------------------------------------
* Restituisce la capacità massima della lezione.
*
* Parametri:
*   l: puntatore alla lezione
*
* Pre-condizione:
*   La lezione deve essere valida.
*
* Post-condizione:
*   ritorna numero intero dei posti massimi disponibili, o -1 se la lezione non è valida.
*
* Come funziona:
* - Legge il campo `posti_max`.
*/
int get_posti_max(Lezione l)
{
    if (l == NULL)
        return -1;
    return l->posti_max; 
}

/*
* Funzione: get_nome_lezione
* ----------------------------------------
* Restituisce il nome della lezione.
*
* Parametri:
*   l: puntatore alla lezione
*
* Pre-condizione:
*   La lezione deve essere valida.
*
* Post-condizione:
*   ritorna la stringa contenente il nome, o NULL se la lezione non è valida.
*
* Come funziona:
* - Ritorna direttamente il campo `nome`, o NULL se la lezione non è valida.
*/
char* get_nome_lezione(Lezione l)
{
    if (l == NULL)
        return NULL;
    return l->nome;
}

/*
* Funzione: get_data_lezione
* ----------------------------------------
* Restituisce la data della lezione.
*
* Parametri:
*   l: puntatore alla lezione
*
* Pre-condizione:
*   La lezione deve essere valida.
*
* Post-condizione:
*   Ritorna un oggetto di tipo Data, o NULL.
*
* Come funziona:
* - Ritorna il campo `data`, o NULL.
*/
Data get_data_lezione(Lezione l)
{
    if (l == NULL)
        return NULL;
    return l->data;
}

/*
* Funzione: get_ora_lezione
* ----------------------------------------
* Restituisce l’orario della lezione.
*
* Parametri:
*   l: puntatore alla lezione
*
* Pre-condizione:
*   La lezione deve essere valida.
*
* Post-condizione:
*   Ritorna un oggetto di tipo Orario, o NULL.
*
* Come funziona:
* - Ritorna il campo `ora_lezione`, o NULL.
*/
Orario get_ora_lezione(Lezione l)
{
    if (l == NULL)
        return NULL;
    return l->ora_lezione;
}

/*
* Funzione: set_posti_occupati
* ----------------------------------------
* Imposta il numero di posti occupati della lezione.
*
* Parametri:
*   l: puntatore alla lezione
*   pos_occupati: numero di posti da impostare
*
* Pre-condizione:
*   Il valore deve essere compreso tra 0 e posti_max.
*
* Post-condizione:
*   Il campo `posti_occupati` viene aggiornato se valido.
*
* Come funziona:
* - Controlla che il valore sia nel range consentito.
* - Aggiorna `posti_occupati` o stampa errore.
*/
void set_posti_occupati(Lezione l, int pos_occupati)
{
    if (l == NULL)
        return;

    if (pos_occupati >= 0 && pos_occupati <= l->posti_max)
    {
        l->posti_occupati = pos_occupati;
    }
    else
    {
        printf("Errore: posti occupati non validi (deve essere tra 0 e %d).\n", l->posti_max);
    }
}
