//FILE data.c

#include<stdio.h>
#include<stdlib.h>
#include"data.h"

struct data
{
    int giorno;   
    int mese;
    int anno;
};

// Funzione per creare una nuova data
Data creaData(int gg, int mm, int aa)
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

