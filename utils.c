//FILE utils.c

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "data.h"
#include "hash.h"
#include "cliente.h"


// Funzione per salvare il contatore in un file
void salva_contatore(int counter)
{
    FILE* file = fopen("counter.txt", "w");  // Apre il file in modalità scrittura (sovrascrive)
    if (file == NULL)
    {
        printf("Errore nell'aprire il file per salvare il contatore.\n");
        exit(1);
    }

    fprintf(file, "%d", counter);  // Scrive il valore del contatore nel file
    fclose(file);  // Chiude il file
}

// Funzione per caricare il contatore da un file
int carica_contatore()
{
    FILE* file = fopen("counter.txt", "r");  // Apre il file in modalità lettura
    int counter = 0;

    if (file != NULL)
    {
        fscanf(file, "%d", &counter);  // Legge il valore del contatore dal file
        fclose(file);  // Chiude il file
    }
    else
    {
        printf("File 'counter.txt' non trovato. Inizializzo il contatore a 0.\n");
    }

    return counter;
}

void carica_clienti_da_file(hashtable h)
{
    FILE *fp = fopen("clienti.txt", "r");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file clienti.txt\n");
        return;
    }

    char id[20], nome[50], cognome[50];
    int durata, giorno, mese, anno, gs, ms, as;

    while (fscanf(fp, "ID: %s\n", id) == 1)
    {
        fscanf(fp, "Nome: %s\n", nome);
        fscanf(fp, "Cognome: %s\n", cognome);
        fscanf(fp, "Durata abbonamento: %d\n", &durata);
        fscanf(fp, "Data d'iscrizione: %d/%d/%d\n", &giorno, &mese, &anno);
        fscanf(fp, "Data scadenza: %d/%d/%d\n", &gs, &ms, &as);
        fscanf(fp, "-----------------------\n");

        Data data_is = crea_data(giorno, mese, anno);
        Data data_sca =  crea_data(gs, ms, as);

        Cliente c = crea_cliente(id, nome, cognome, durata, data_is);
        set_data_scadenza(c, data_sca);
        insertHash(h, c);
    }

    fclose(fp);
}

// Funzione per pulire lo schermo, portabile su Windows, Linux e macOS
void pulisci_schermo()
{
    #ifdef _WIN32  // Verifica se il sistema è Windows
        system("cls");
    #else  // Se il sistema è Linux o macOS
        system("clear");
    #endif
}

int calcola_durata_in_mesi(Data data_inizio, Data data_fine)
{
    int anni_diff = get_anno(data_fine) - get_anno(data_inizio);
    int mesi_diff = get_mese(data_fine) - get_mese(data_inizio);
    int giorni_diff = get_giorno(data_fine) - get_giorno(data_inizio);

    // Calcolare la durata in mesi
    int durata_in_mesi = anni_diff * 12 + mesi_diff;
    
    // Se la differenza nei giorni è negativa (ovvero la data di fine non è ancora arrivata a completare il mese), 
    // diminuisci un mese dalla durata
    if (giorni_diff < 0)
    {
        durata_in_mesi--;
    }

    return durata_in_mesi;
}

void riscrivi_file_clienti(hashtable h)
{
    FILE* fp = fopen("clienti.txt", "w"); // "w" per riscrivere il file ogni volta
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file per la scrittura.\n");
        return;
    }

    Cliente* table = get_table_hash(h); 

    for (int i = 0; i < get_size_hash(h); i++)  // Scorri ogni slot della tabella hash
    {
        Cliente curr = table[i];  // Ottieni la lista di clienti in questo slot

        while (curr != NULL)  // Scorri la lista concatenata
        {
            // Scrivi le informazioni del cliente nel file
            fprintf(fp, "ID: %s\n", get_id_cliente(curr));
            fprintf(fp, "Nome: %s\n", get_nome_cliente(curr));
            fprintf(fp, "Cognome: %s\n", get_cognome_cliente(curr));
            fprintf(fp, "Durata abbonamento: %d\n", get_durata_abbonamento(curr));
            
            // Scrivi le date (data iscrizione e scadenza)
            fprintf(fp, "Data d'iscrizione: %02d/%02d/%04d\n",
                get_giorno(get_data_iscrizione(curr)),
                get_mese(get_data_iscrizione(curr)),
                get_anno(get_data_iscrizione(curr)));
                
            fprintf(fp, "Data scadenza: %02d/%02d/%04d\n",
                get_giorno(get_data_scadenza(curr)),
                get_mese(get_data_scadenza(curr)),
                get_anno(get_data_scadenza(curr)));
                
            fprintf(fp, "-----------------------\n");  // Separazione tra i clienti

            curr = get_next_cliente(curr);  // Passa al prossimo cliente nella lista
        }
    }

    fclose(fp);  // Chiudi il file
}
