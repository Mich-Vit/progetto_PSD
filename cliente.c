//FILE cliente.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"data.h"
#include"cliente.h"
#include"utils.h"

// === Definizione della struttura ===

struct cliente
{
    char *id;           
    char nome[50];
    char cognome[50];
    int durata_abbonamento;      // in mesi
    Data data_iscrizione;
    Data data_scadenza;
    struct cliente *next;
};

/*
* Funzione: crea_cliente
* ----------------------------------------
* Crea un nuovo cliente allocando memoria dinamica per la struttura e per l'ID.
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
* Come funziona:
* - Alloca memoria per il cliente e per la stringa ID.
* - Copia i dati passati nella nuova struttura.
* - Calcola la data di scadenza dell'abbonamento con calcolo_scadenza_abbonamento.
* - Imposta il puntatore next a NULL.
*/
Cliente crea_cliente(char* id, char* nome, char* cognome, int durata, Data data_is)
{
    Cliente c = malloc(sizeof(struct cliente));
    if (c == NULL)
    {
        printf("Errore di allocazione memoria per il cliente.\n");
        exit(1);
    }

    c->id = malloc(strlen(id) + 1);
    if (c->id == NULL)
    {
        printf("Errore di allocazione memoria per l'ID del cliente.\n");
        free(c);
        exit(1);
    }
    strcpy(c->id, id);

    strcpy(c->nome, nome);
    strcpy(c->cognome, cognome);
    c->durata_abbonamento = durata;
    c->data_iscrizione = copia_data(data_is);
    c->data_scadenza = calcolo_scadenza_abbonamento(data_is, durata);
    c->next = NULL;

    return c;
}

/*
* Funzione: distruggi_cliente
* ----------------------------------------
* Libera la memoria allocata per un cliente e le date associate.
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
* Come funziona:
* - Se il cliente è NULL non fa nulla.
* - Libera memoria per ID, date e struttura cliente.
*/
void distruggi_cliente(Cliente c)
{
    if (c != NULL)
    {
        free(c->id);
        libera_data(c->data_iscrizione);
        libera_data(c->data_scadenza);
        free(c);
    }
}

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
* Come funziona:
* - Se c == NULL, stampa "Cliente non valido", altrimenti:
* - Stampa durata abbonamento, data iscrizione e data scadenza.
*/
void visualizza_abbonamento_cliente(Cliente c)
{
    if (c == NULL)
    {
        printf("Cliente non valido.\n");
        return;
    }

    printf("Durata abbonamento: %d mesi\n", c->durata_abbonamento);
    printf("Data iscrizione: ");
    visualizza_data(c->data_iscrizione);
    printf("Data scadenza: ");
    visualizza_data(c->data_scadenza);
}

/*
* Funzione: visualizza_cliente
* ----------------------------------------
* Visualizza tutte le informazioni di un cliente.
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
* Come funziona:
* - Se c == NULL, stampa "Cliente non valido", altrimenti:
* - Stampa ID, nome, cognome e chiama la funzione visualizza_abbonamento_cliente.
*/
void visualizza_cliente(Cliente c)
{
    if (c == NULL)
    {
        printf("Cliente non valido.\n");
        return;
    }

    printf("===================================\n");
    printf("ID: %s\n",c->id);
    printf("Nome: %s\n",c->nome);
    printf("Cognome: %s\n",c->cognome);
    visualizza_abbonamento_cliente(c);
}

/*
* Funzione: visualizza_essenziale_cliente
* ----------------------------------------
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
* Come funziona:
* - Se c == NULL, stampa "Cliente non valido", altrimenti:
* - Stampa ID, cognome e nome in formato tabellare.
*/
void visualizza_essenziale_cliente(Cliente c)
{
    if (c == NULL)
    {
        printf("Cliente non valido.\n");
        return;
    }

    printf("%-8s %-15s %-15s\n",c->id, c->cognome, c->nome);
}

/*
* Funzione: confronta_clienti
* ----------------------------------------
* Confronta due clienti in base al loro ID.
*
* Parametri:
*   c1: primo cliente
*   c2: secondo cliente
*
* Pre-condizione:
*   Entrambi i clienti devono essere validi.
*
* Post-condizione:
*   1 se i clienti sono uguali, 0 altrimenti.
*
* Come funziona:
* - Se uno dei due è NULL, restituisce 0.
* - Altrimenti confronta gli ID con strcmp.
*/
int confronta_clienti(Cliente c1, Cliente c2)
{
    if (c1 == NULL || c2 == NULL)
        return 0;  // Non uguali se uno dei due è NULL

    if (strcmp(c1->id, c2->id) == 0)
        return 1;
    else
        return 0; 
}

/*
* Funzione: salva_cliente_file
* ----------------------------------------
* Salva i dati del cliente su file di testo "clienti.txt" in modalità append.
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
* Come funziona:
* - Apre il file in append.
* - Scrive i dati nel file.
* - Chiude il file.
*/
void salva_cliente_file(Cliente c)
{
    FILE *fp = fopen("clienti.txt", "a");
    if (fp == NULL)
    {
        printf("Errore nell'aprire il file clienti.txt.\n");
        return;
    }

    fprintf(fp, "ID: %s\n", c->id); 
    fprintf(fp, "Nome: %s\n", c->nome);  
    fprintf(fp, "Cognome: %s\n", c->cognome);  
    fprintf(fp, "Durata abbonamento: %d\n", c->durata_abbonamento);

    fprintf(fp, "Data d'iscrizione: %02d/%02d/%04d\n", 
            get_giorno(c->data_iscrizione), get_mese(c->data_iscrizione), get_anno(c->data_iscrizione));
    fprintf(fp, "Data scadenza: %02d/%02d/%04d\n", 
            get_giorno(c->data_scadenza), get_mese(c->data_scadenza), get_anno(c->data_scadenza));

    // Aggiunge una riga di separazione per il prossimo cliente
    fprintf(fp, "-----------------------\n");

    fclose(fp);
}

// === Getter ===

/*
* Funzione: get_data_scadenza
* ----------------------------------------
* Restituisce la data di scadenza dell'abbonamento del cliente.
*
* Parametri:
*   c: cliente di cui ottenere la data.
*
* Valore di ritorno:
*   Data di scadenza o NULL.
*/
Data get_data_scadenza(Cliente c)
{
    if (c == NULL)
        return NULL;
    return c->data_scadenza;
}

/*
* Funzione: get_id_cliente
* ----------------------------------------
* Restituisce l'ID del cliente.
*
* Parametri:
*   c: cliente da cui ottenere l'ID.
*
* Valore di ritorno:
*   Stringa con ID o NULL.
*/
char* get_id_cliente(Cliente c)
{
    if (c == NULL)
        return NULL;
    return c->id;
}

/*
* Funzione: get_next_cliente
* ----------------------------------------
* Restituisce il cliente successivo nella lista collegata.
*
* Parametri:
*   c: cliente corrente.
*
* Valore di ritorno:
*   Puntatore al cliente successivo o NULL.
*/
Cliente get_next_cliente(Cliente c)
{
    if (c == NULL)
        return NULL;
    return c->next;
}

/*
* Funzione: get_nome_cliente
* ----------------------------------------
* Restituisce il nome del cliente.
*
* Parametri:
*   c: cliente.
*
* Valore di ritorno:
*  Stringa con il nome o NULL.
*/
char* get_nome_cliente(Cliente c)
{
    if (c == NULL)
        return NULL;
    return c->nome;
}

/*
* Funzione: get_cognome_cliente
* ----------------------------------------
* Restituisce il cognome del cliente.
*
* Parametri:
*   c: cliente.
*
* Valore di ritorno:
*   Puntatore alla stringa cognome o NULL.
*/
char* get_cognome_cliente(Cliente c)
{
    if (c == NULL)
        return NULL;
    return c->cognome;
}

/*
* Funzione: get_durata_abbonamento
* ----------------------------------------
* Restituisce la durata dell'abbonamento in mesi.
*
* Parametri:
*   c: cliente.
*
* Valore di ritorno:
*   Durata in mesi o -1 se cliente non valido.
*/
int get_durata_abbonamento(Cliente c)
{
    if (c == NULL)
        return -1; 
    return c->durata_abbonamento;
}

/*
* Funzione: get_data_iscrizione
* ----------------------------------------
* Restituisce la data di iscrizione del cliente.
*
* Parametri:
*   c: cliente.
*
* Valore di ritorno:
*   Data di iscrizione o NULL se cliente non valido.
*/
Data get_data_iscrizione(Cliente c)
{
    if (c == NULL)
        return NULL;
    return c->data_iscrizione;
}

// === Setter ===

/*
* Funzione: set_next_cliente
* --------------------------
* Imposta il cliente successivo nella lista.
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
*/
void set_next_cliente(Cliente c, Cliente next)
{
    if (c != NULL) {
        c->next = next;
    }
}

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
*/
void set_data_scadenza(Cliente c, Data nuova_data)
{
    if (c != NULL && nuova_data != NULL) 
    {
        // Aggiorna la data di scadenza del cliente
        c->data_scadenza = nuova_data;
    }
    else
    {
        printf("Errore: Cliente o data non valida.\n");
    }
}

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
*/
void set_durata(Cliente c, int nuova_durata)
{
    if (c != NULL)  // Verifica che il cliente esista (non sia NULL)
    {
        c->durata_abbonamento = nuova_durata;  // Imposta la nuova durata dell'abbonamento
    }
    else
    {
        printf("Errore: Cliente non valido.\n");
    }
}

