//FILE prenotazione.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"prenotazione.h"
#include"cliente.h"
#include"lezione.h"
#include"data.h"
#include"utils.h"

struct prenotazione
{
    char *id_prenotazione;
    char *id_cliente;   
    char *id_lezione;   
    Data data_prenotazione;

    struct prenotazione *next;
};

/*
* Funzione: crea_prenotazione
* ----------------------------------------
* Alloca e inizializza una nuova prenotazione con i dati specificati.
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
*   Ritorna un nuovo oggetto Prenotazione con i dati copiati.
*
* Come funziona:
* - Alloca memoria per la struttura e per ciascuna stringa ID.
* - Copia i valori dei parametri nella struttura.
* - Imposta il campo `next` a NULL.
*/
Prenotazione crea_prenotazione(char *id_pren, char* id_cli, char* id_lez, Data data_prenotazione) 
{
    Prenotazione nuova_prenotazione = malloc(sizeof(struct prenotazione));
    if (nuova_prenotazione == NULL) 
    {
        printf("Errore nell'allocazione memoria per la prenotazione\n");
        exit(1);
    }

    nuova_prenotazione->id_prenotazione = malloc(strlen(id_pren) + 1);
    if (nuova_prenotazione->id_prenotazione == NULL)
    {
        printf("Errore nell'allocazione memoria per l'ID della prenotazione\n");
        free(nuova_prenotazione);
        exit(1);
    }
    strcpy(nuova_prenotazione->id_prenotazione, id_pren);

    nuova_prenotazione->id_cliente = malloc(strlen(id_cli) + 1);
    if (nuova_prenotazione->id_cliente == NULL)
    {
        printf("Errore nell'allocazione memoria per l'ID del cliente\n");
        free(nuova_prenotazione->id_prenotazione);
        free(nuova_prenotazione);
        exit(1);
    }
    strcpy(nuova_prenotazione->id_cliente, id_cli);

    nuova_prenotazione->id_lezione = malloc(strlen(id_lez) + 1);
    if (nuova_prenotazione->id_lezione == NULL)
    {
        printf("Errore nell'allocazione memoria per l'ID della lezione\n");
        free(nuova_prenotazione->id_cliente);
        free(nuova_prenotazione->id_prenotazione);
        free(nuova_prenotazione);
        exit(1);
    }
    strcpy(nuova_prenotazione->id_lezione, id_lez);

    nuova_prenotazione->data_prenotazione = copia_data(data_prenotazione);
    nuova_prenotazione->next = NULL;

    return nuova_prenotazione;
}

/*
* Funzione: salva_prenotazione_file
* ----------------------------------------
* Salva su file la prenotazione.
*
* Parametri:
*   p: puntatore alla prenotazione da salvare.
*
* Pre-condizione:
*   p deve essere un puntatore valido a una Prenotazione.
*
* Post-condizione:
*   I dati vengono scritti in fondo al file "prenotazioni.txt".
*
* Come funziona:
* - Apre il file in modalità append.
* - Scrive i campi della prenotazione.
* - Chiude il file.
*/
void salva_prenotazione_file(Prenotazione p)
{
    FILE *fp = fopen("prenotazioni.txt", "a");
    if (fp == NULL)
    {
        printf("Errore nell'aprire il file prenotazioni.txt.\n");
        return;
    }

    fprintf(fp, "ID: %s\n", p->id_prenotazione);
    fprintf(fp, "ID Cliente: %s\n", p->id_cliente);
    fprintf(fp, "ID Lezione: %s\n", p->id_lezione);
    fprintf(fp, "Data Prenotazione: %02d/%02d/%04d\n",
            get_giorno(p->data_prenotazione),
            get_mese(p->data_prenotazione),
            get_anno(p->data_prenotazione));

    fprintf(fp, "-----------------------\n");

    fclose(fp);
}

/*
* Funzione: visualizza_prenotazione
* ----------------------------------------
* Stampa tutte le informazioni di una prenotazione.
*
* Parametri:
*   p: puntatore alla prenotazione da stampare.
*
* Pre-condizione:
*   p non deve essere NULL.
*
* Post-condizione:
*   I dati della prenotazione vengono stampati a video.
*
* Come funziona:
* - Controlla se la prenotazione è NULL.
* - Se valida, stampa ID prenotazione, cliente, lezione e data.
*/
void visualizza_prenotazione(Prenotazione p)
{
    if (p == NULL)
    {
        printf("Prenotazione nulla.\n");
        return;
    }

    printf("ID Prenotazione: %s\n", p->id_prenotazione);
    printf("ID Cliente: %s\n", p->id_cliente);
    printf("ID Lezione: %s\n", p->id_lezione);
    printf("Data Prenotazione: ");
    visualizza_data(p->data_prenotazione);
    printf("\n");
}

/*
* Funzione: visualizza_essenziale_prenotazione
* ----------------------------------------
* Stampa solo i dati essenziali della prenotazione.
*
* Parametri:
*   p: puntatore alla prenotazione.
*
* Pre-condizione:
*   p non deve essere NULL.
*
* Post-condizione:
*   Vengono stampati ID prenotazione, ID cliente e ID lezione.
*
* Come funziona:
* - Verifica se p è NULL.
* - Stampa i tre ID.
*/
void visualizza_essenziale_prenotazione(Prenotazione p)
{
    if (p == NULL)
    {
        printf("Prenotazione nulla.\n");
        return;
    }

    printf("%-15s %-15s %-15s\n",
           p->id_prenotazione,
           p->id_cliente,
           p->id_lezione);
}

/*
* Funzione: libera_prenotazione
* ----------------------------------------
* Libera la memoria occupata da una prenotazione.
*
* Parametri:
*   p: puntatore alla prenotazione da liberare.
*
* Pre-condizione:
*   p può essere NULL o valido.
*
* Post-condizione:
*   Tutta la memoria allocata dinamicamente per la prenotazione viene liberata.
*
* Come funziona:
* - Libera ogni campo stringa e la data.
* - Infine libera la struttura stessa.
*/
void libera_prenotazione(Prenotazione p)
{
    if (p != NULL)
    {
        free(p->id_prenotazione); 
        free(p->id_cliente);
        free(p->id_lezione);
        libera_data(p->data_prenotazione);
        free(p);
    }
}

/*
* Funzione: get_id_prenotazione
* ----------------------------------------
* Restituisce l'ID della prenotazione.
*
* Parametri:
*   p: puntatore alla prenotazione.
*
* Pre-condizione:
*   p deve essere valido.
*
* Post-condizione:
*   Restituisce una stringa con l'ID della prenotazione, NULL se p non è valido.
*
* Come funziona:
* - Accede direttamente al campo `id_prenotazione`.
*/
char* get_id_prenotazione(Prenotazione p)
{
    if (p == NULL)
        return NULL;
    return p->id_prenotazione;
}

/*
* Funzione: get_id_cliente_prenotazione
* ----------------------------------------
* Restituisce l'ID del cliente associato alla prenotazione.
*
* Parametri:
*   p: prenotazione da cui estrarre l'informazione.
*
* Pre-condizione:
*   p deve essere valido.
*
* Post-condizione:
*   Restituisce una stringa con l'ID cliente, NULL se p non è valido.
*
* Come funziona:
* - Restituisce il campo `id_cliente` della struttura.
*/
char* get_id_cliente_prenotazione(Prenotazione p)
{
    if (p == NULL)
        return NULL;
    return p->id_cliente;
}

/*
* Funzione: get_id_lezione_prenotazione
* ----------------------------------------
* Restituisce l'ID della lezione associata.
*
* Parametri:
*   p: prenotazione da cui estrarre l’informazione.
*
* Pre-condizione:
*   p deve essere valido.
*
* Post-condizione:
*   Restituisce l’ID della lezione prenotata, NULL se p non è valido.
*
* Come funziona:
* - Restituisce il campo `id_lezione` della struttura.
*/
char* get_id_lezione_prenotazione(Prenotazione p)
{
    if (p == NULL)
        return NULL;
    return p->id_lezione;
}

/*
* Funzione: get_data_prenotazione
* ----------------------------------------
* Restituisce la data in cui è stata effettuata la prenotazione.
*
* Parametri:
*   p: prenotazione.
*
* Pre-condizione:
*   p deve essere valido.
*
* Post-condizione:
*   Ritorna un oggetto Data, NULL se p non è valido.
*
* Come funziona:
* - Ritorna il campo `data_prenotazione` della struttura.
*/
Data get_data_prenotazione(Prenotazione p)
{
    if (p == NULL)
        return NULL;
    return p->data_prenotazione;
}

/*
* Funzione: get_next_prenotazione
* ----------------------------------------
* Restituisce il puntatore alla prossima prenotazione (nella lista collegata).
*
* Parametri:
*   p: prenotazione corrente.
*
* Pre-condizione:
*   p può essere NULL o valido.
*
* Post-condizione:
*   Ritorna il campo `next`, o NULL se p non è valido.
*
* Come funziona:
* - Restituisce il campo `next`, cioè il nodo successivo nella lista.
*/
Prenotazione get_next_prenotazione(Prenotazione p)
{
    if (p == NULL)
        return NULL;
    return p->next;
}

/*
* Funzione: set_next_prenotazione
* ----------------------------------------
* Imposta il puntatore alla prossima prenotazione.
*
* Parametri:
*   p: prenotazione corrente.
*   next: prenotazione successiva da collegare.
*
* Pre-condizione:
*   p deve essere valido.
*
* Post-condizione:
*   Imposta il campo `next` della struttura.
*
* Come funziona:
* - Assegna a `p->next` il puntatore `next`.
*/
void set_next_prenotazione(Prenotazione p, Prenotazione next)
{
    if (p != NULL)
        p->next = next;
}
