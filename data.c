//FILE data.c

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"data.h"

struct data
{
    int giorno;   
    int mese;
    int anno;
};

/*
* Funzione: crea_data
* ----------------------------------------
* Crea una nuova data allocando memoria dinamica per la struttura data.
*
* Parametri:
*   gg: giorno (1-31)
*   mm: mese (1-12)
*   aa: anno
*
* Pre-condizione:
*   I valori devono rappresentare una data valida.
*
* Post-condizione:
*   Viene creato un nuovo oggetto Data allocato dinamicamente.
*
* Come funziona:
* - Alloca memoria.
* - Se malloc fallisce, stampa errore e termina il programma.
* - Inizializza giorno, mese e anno.
* - Ritorna la nuova data creata
*/
Data crea_data(int gg, int mm, int aa)
{
    Data nuova_data = malloc(sizeof(struct data));
    if(nuova_data == NULL) 
    {
        printf("Errore di allocazione memoria per la data.\n");
        exit(1);
    }

    nuova_data->giorno = gg;
    nuova_data->mese = mm;
    nuova_data->anno = aa;

    return nuova_data;
}

/*
* Funzione: visualizza_data
* ----------------------------------------
* Stampa la data nel formato gg/mm/aaaa.
*
* Parametri:
*   data: puntatore alla struttura Data da visualizzare.
*
* Pre-condizione:
*   data può essere NULL, in tal caso non stampa nulla.
*
* Post-condizione:
*   La data viene stampata su output.
*/
void visualizza_data(Data data) 
{
    if (data != NULL) 
    {
        printf("%02d/%02d/%04d\n", data->giorno, data->mese, data->anno);
    }
}

/*
* Funzione: confronta_date
* ----------------------------------------
* Confronta due date.
*
* Parametri:
*   d1: prima data
*   d2: seconda data
*
* Pre-condizione:
*   Entrambe le date devono essere valide.
*
* Post-condizione:
*   -1 se d1 < d2,
*    0 se d1 == d2,
*    1 se d1 > d2.
*
* Come funziona:
* - Confronta anno, poi mese, poi giorno in ordine.
*/
int confronta_date(Data d1, Data d2)
{
    if (d1->anno > d2->anno)
        return 1;
    if (d1->anno < d2->anno)
        return -1;

    // Anno uguale, confronto i mesi
    if (d1->mese > d2->mese)
        return 1;
    if (d1->mese < d2->mese)
        return -1;

    // Mese uguale, confronto i giorni
    if (d1->giorno > d2->giorno)
        return 1;
    if (d1->giorno < d2->giorno)
        return -1;

    // Tutti uguali
    return 0;
}

/*
* Funzione: data_valida
* ----------------------------------------
* Verifica se una data è valida (giorno, mese, anno).
*
* Parametri:
*   giorno, mese, anno: valori interi.
*
* Valore di ritorno:
*   1 se la data è valida, 0 altrimenti.
*
* Come funziona:
* - Controlla intervalli di anno, mese e giorno.
* - Tiene conto degli anni bisestili per febbraio.
*/
static int data_valida(int giorno, int mese, int anno)
{
    if (anno < 1900 || mese < 1 || mese > 12 || giorno < 1)
        return 0;

    int giorni_per_mese[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Anno bisestile
    if ((anno % 4 == 0 && anno % 100 != 0) || (anno % 400 == 0))
        giorni_per_mese[1] = 29; //febbraio avra' 29 giorni

    //Controlla che il giorno non superi il numero massimo di giorni per quel mese.
    if (giorno > giorni_per_mese[mese - 1])
        return 0;

    return 1;
}

/*
* Funzione: leggi_data
* ----------------------------------------
* Legge da input una data valida successiva ad oggi.
*
* Parametri:
*   Nessuno.
*
* Pre-condizione:
*   L'input deve essere fornito in modo corretto (gg/mm/aaaa);
*   La data inserita deve essere valida.
*
* Post-condizione:
*   Viene creata una nuova struttura Data contenente la data letta.
*
* Come funziona:
* - Richiede input in formato gg/mm/aaaa.
* - Controlla che la data sia valida e successiva ad oggi.
* - Ripete la richiesta in caso di errore.
*/
Data leggi_data()
{
    int giorno, mese, anno;

    while (1)
    {
        printf("Inserisci la data (GG/MM/AAAA): ");
        if (scanf("%d/%d/%d", &giorno, &mese, &anno) != 3)
        {
            printf("Input non valido. Riprova.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (!data_valida(giorno, mese, anno))
        {
            printf("Data non valida. Verifica il giorno, mese e anno.\n");
            continue;
        }

        Data inserita = crea_data(giorno, mese, anno);

        if (inserita == NULL)
        {
            printf("Errore nella creazione della data.\n");
            continue;
        }

        if (confronta_date(inserita, data_oggi()) <= 0)
        {
            printf("Errore: la data non puo' essere precedente o uguale ad oggi.\n");
            libera_data(inserita);
            continue;
        }

        return inserita;
    }
}

/*
* Funzione: copia_data
* ----------------------------------------
* Crea una copia di una data esistente.
*
* Parametri:
*   originale: puntatore alla data da copiare.
*
* Pre-condizione:
*   Il puntatore originale deve essere valido.
*
* Post-condizione:
*   Viene restituita una nuova struttura Data con gli stessi valori.
*
* Come funziona:
* - Alloca nuova struttura.
* - Termina il programma in caso di errore di allocazione.
* - Copia i campi giorno, mese, anno.
* - Ritorna la nuova data
*/
Data copia_data(Data originale) 
{
    Data nuova_data = malloc(sizeof(struct data)); 
    if (nuova_data == NULL)
    {
        printf("Errore di allocazione memoria per la copia della data.\n");
        exit(1);
    }

    nuova_data->giorno = originale->giorno;
    nuova_data->mese = originale->mese;
    nuova_data->anno = originale->anno;

    return nuova_data;
}

/*
* Funzione: libera_data
* ----------------------------------------
* Libera la memoria allocata per una data.
*
* Parametri:
*   d: puntatore alla data da liberare.
*
* Pre-condizione:
*   Il puntatore d deve essere valido.
*
* Post-condizione:
*   La memoria occupata dalla data è liberata.
*
* Come funziona:
* - Se d è NULL non fa nulla.
* - Altrimenti libera la memoria.
*/
void libera_data(Data d) 
{
    if (d != NULL)
    {
        free(d);
    }
}

/*
* Funzione: data_oggi
* ----------------------------------------
* Restituisce la data odierna.
*
* Post-condizione:
*   Viene restituito un oggetto Data contenente la data attuale.
*
* Come funziona:
* - Usa funzioni della libreria time.h per ottenere la data corrente.
* - Restituisce una Data allocata dinamicamente.
*/
Data data_oggi()
{
    time_t t = time(NULL); // Ottieni tempo corrente
    struct tm *tm_info = localtime(&t);  // Converti in tempo locale

    int giorno = tm_info->tm_mday;
    int mese = tm_info->tm_mon + 1;     // tm_mon parte da 0 (gennaio)
    int anno = tm_info->tm_year + 1900; // tm_year parte da 1900

    return crea_data(giorno, mese, anno);
}

// === Getter ===

/*
* Funzione: get_giorno
* ----------------------------------------
* Restituisce il giorno di una Data.
*
* Parametri:
*   d: Data.
*
* Pre-condizione:
*   Il puntatore d deve essere valido.
*
* Valore di ritorno:
*   Intero rappresentante il giorno, oppure -1 .
*/
int get_giorno(Data d)
{
    if(d == NULL)
        return -1;
    return d->giorno;
}

/*
* Funzione: get_mese
* ----------------------------------------
* Restituisce il mese di una Data.
*
* Parametri:
*   d: Data.
*
* Pre-condizione:
*   Il puntatore d deve essere valido.
*
* Valore di ritorno:
*   Intero rappresentante il mese, oppure -1.
*/
int get_mese(Data d)
{
    if(d == NULL)
        return -1;
    return d->mese;
}

/*
* Funzione: get_anno
* ----------------------------------------
* Restituisce l'anno di una Data.
*
* Parametri:
*   d: Data.
*
* Pre-condizione:
*   Il puntatore d deve essere valido.
*
* Valore di ritorno:
*   Intero rappresentante l'anno, oppure -1.
*/
int get_anno(Data d)
{
    if(d == NULL)
        return -1;
    return d->anno;
}
