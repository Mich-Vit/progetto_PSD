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

// Funzione per creare una nuova data
Data crea_data(int gg, int mm, int aa)
{
    Data nuova_data = malloc(sizeof(struct data));
    if(nuova_data == NULL) //controllo malloc
    {
        printf("Errore di allocazione memoria per la data.\n");
        exit(1);  // Uscita in caso di errore di memoria
    }

    nuova_data->giorno = gg;
    nuova_data->mese = mm;
    nuova_data->anno = aa;

    return nuova_data;
}

// Funzione per visualizzare una data
void visualizza_data(Data data) 
{
    if (data != NULL) 
    {
        printf("%02d/%02d/%04d\n", data->giorno, data->mese, data->anno);
    }
}

//calcola la data di scadenza dell'abbonamento
Data calcolo_scadenza_abbonamento(Data data_inizio, int durata_abbonamento) 
{
    Data data_scadenza = crea_data(data_inizio->giorno, data_inizio->mese, data_inizio->anno);

    // Aggiungi la durata (in mesi) alla nuova data di scadenza
    data_scadenza->mese += durata_abbonamento;

    // Gestisci l'overflow dei mesi (se i mesi superano 12, si passa all'anno successivo)
    while (data_scadenza->mese > 12) 
    {
        data_scadenza->mese -= 12;  // Reset mese a gennaio
        data_scadenza->anno += 1;   // Aggiungi un anno
    }

    return data_scadenza; 
}

int confronta_date(Data d1, Data d2)// -1 se d1 < d2, 0 se d1 == d2, 1 se d1 > d2
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

int abbonamento_valido(Data oggi, Data scadenza) // Ritorna 1 se e' valido, 0 altrimenti
{
    if (confronta_date(oggi, scadenza) == -1) 
    {
        // oggi è prima della scadenza
        return 1;
    } 
    else if (confronta_date(oggi, scadenza) == 0) 
    {
        // oggi è il giorno stesso della scadenza
        return 1;
    } 
    else 
    {
        // oggi è dopo la scadenza
        return 0;
    }
}

Data copia_data(Data originale) 
{
    // Alloca memoria per una nuova Data
    Data nuova_data = malloc(sizeof(struct data)); 
    if (nuova_data == NULL)
    {
        printf("Errore di allocazione memoria per la copia della data.\n");
        exit(1);
    }

    // Copia i dati dalla data originale alla nuova
    nuova_data->giorno = originale->giorno;
    nuova_data->mese = originale->mese;
    nuova_data->anno = originale->anno;

    return nuova_data;
}

// Funzione che libera la memoria allocata per una data
void libera_data(Data d) 
{
    if (d != NULL)
    {
        free(d);  // Libera la memoria allocata per la data
    }
}

Data data_oggi()
{
    time_t t = time(NULL);            // Ottieni tempo corrente
    //la struttura struct tm e' presente nella libreria <time.h>, così come la funzione localtime 
    struct tm *tm_info = localtime(&t);  // Converti in tempo locale

    int giorno = tm_info->tm_mday;
    int mese = tm_info->tm_mon + 1;     // tm_mon parte da 0 (gennaio)
    int anno = tm_info->tm_year + 1900; // tm_year parte da 1900

    return crea_data(giorno, mese, anno);
}

// === Getter ===

int get_giorno(Data d)
{
    return d->giorno;
}

int get_mese(Data d)
{
    return d->mese;
}

int get_anno(Data d)
{
    return d->anno;
}
