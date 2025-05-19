//FILE utils.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "data.h"
#include "hash.h"
#include "cliente.h"
#include "lezione.h"
#include "lista_lezioni.h"
#include "orario.h"
#include "hash_prenotazioni.h"
#include "prenotazione.h"

int carica_contatore_generico(const char* filename, const char* prefix)
{
    FILE* file = fopen(filename, "r");
    int max_id = 0;

    if (file == NULL)
    {
        printf("File '%s' non trovato. Inizializzo il contatore a 0.\n", filename);
        return 0;
    }

    char line[256];
    int prefix_len = strlen(prefix);
    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, prefix, prefix_len) == 0)
        {
            int current_id = atoi(line + prefix_len);
            if (current_id > max_id)
                max_id = current_id;
        }
    }

    fclose(file);
    return max_id;
}

char* genera_id_generico(const char* prefix_letter, const char* filename)
{
    static int contatore_clienti = -1;
    static int contatore_lezioni = -1;
    static int contatore_prenotazioni = -1;
    int* counter = NULL;

    if (strcmp(prefix_letter, "C") == 0)
        counter = &contatore_clienti;
    else if (strcmp(prefix_letter, "L") == 0)
        counter = &contatore_lezioni;
    else if (strcmp(prefix_letter, "P") == 0)
        counter = &contatore_prenotazioni;
    else
    {
        printf("Prefisso non riconosciuto.\n");
        exit(1);
    }

    char prefix[10];
    sprintf(prefix, "ID: %s", prefix_letter);

    if (*counter == -1)
        *counter = carica_contatore_generico(filename, prefix);

    (*counter)++;

    char* id = malloc(10 * sizeof(char));
    if (id == NULL)
    {
        printf("Errore di allocazione memoria per l'ID.\n");
        exit(1);
    }

    sprintf(id, "%s%03d", prefix_letter, *counter);
    return id;
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

void carica_prenotazioni_da_file(hashtable_p h)
{
    FILE *fp = fopen("prenotazioni.txt", "r");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file prenotazioni.txt\n");
        return;
    }

    char id_pren[20], id_cli[50], id_lez[50];
    int giorno, mese, anno;

    while (fscanf(fp, "ID: %s\n", id_pren) == 1)
    {
        fscanf(fp, "ID Cliente: %s\n", id_cli);
        fscanf(fp, "ID Lezione: %s\n", id_lez);
        fscanf(fp, "Data Prenotazione: %d/%d/%d\n", &giorno, &mese, &anno);
        fscanf(fp, "-----------------------\n");

        Data data_pre = crea_data(giorno, mese, anno);

        // Passo id_pren, id_cli, id_lez e data_pre secondo la nuova firma
        Prenotazione p = crea_prenotazione(id_pren, id_cli, id_lez, data_pre);

        insertHash_p(h, p);
    }

    fclose(fp);
}

void aggiorna_file_lezioni(list l)
{
    FILE *fp = fopen("lezioni.txt", "w");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file lezioni.txt.\n");
        return;
    }

    while (!emptyList(l))
    {
        Lezione le = getFirst(l);

        fprintf(fp, "ID: %s\n", get_id_lezione(le));
        fprintf(fp, "Nome: %s\n", get_nome_lezione(le));
        fprintf(fp, "Data: %02d/%02d/%04d\n",
                get_giorno(get_data_lezione(le)), get_mese(get_data_lezione(le)), get_anno(get_data_lezione(le)));
        fprintf(fp, "Orario: %02d:%02d\n", get_ora(get_ora_lezione(le)), get_minuti(get_ora_lezione(le)));
        fprintf(fp, "Posti massimi: %d\n", get_posti_max(le));
        fprintf(fp, "Posti occupati: %d\n", get_posti_occupati(le));
        fprintf(fp, "-----------------------\n");

        l = tailList(l);
    }

    fclose(fp);
}

void aggiorna_file_prenotazioni(hashtable_p h)
{
    if (h == NULL)
        return;

    int size = get_size_hash_p(h);
    Prenotazione* table = get_table_hash_p(h);

    FILE* fp_out = fopen("prenotazioni.txt", "w");
    if (fp_out != NULL)
    {
        for (int i = 0; i < size; i++)
        {
            Prenotazione curr = table[i];
            while (curr != NULL)
            {
                Data d = get_data_prenotazione(curr);
                fprintf(fp_out, "ID: %s\n", get_id_prenotazione(curr));
                fprintf(fp_out, "ID Cliente: %s\n", get_id_cliente_prenotazione(curr));
                fprintf(fp_out, "ID Lezione: %s\n", get_id_lezione_prenotazione(curr));
                fprintf(fp_out, "Data Prenotazione: %02d/%02d/%04d\n",
                        get_giorno(d), get_mese(d), get_anno(d));
                fprintf(fp_out, "-----------------------\n");
                curr = get_next_prenotazione(curr);
            }
        }
        fclose(fp_out);
    }
    else
    {
        printf("Errore durante il salvataggio delle prenotazioni aggiornate.\n");
    }
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

list carica_lezioni_da_file(list l)
{
    FILE *fp = fopen("lezioni.txt", "r");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file lezioni.txt\n");
        return NULL;
    }

    char buffer[256];
    char id[20], nome[50];
    char orario_str[10];
    int ore, minuti;
    int giorno, mese, anno;
    int posti_max, posti_occupati;

    while (1)
    {
        // Leggi ID
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "ID: %s", id) != 1)
            break;

        // Leggi Nome (anche con spazi)
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Nome: %[^\n]", nome) != 1)
            break;

        // Leggi Data
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Data: %d/%d/%d", &giorno, &mese, &anno) != 3)
            break;

        // Leggi Orario (stringa)
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Orario: %s", orario_str) != 1)
            break;

        // Leggi posti massimi
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Posti massimi: %d", &posti_max) != 1)
            break;

        // Leggi posti occupati
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Posti occupati: %d", &posti_occupati) != 1)
            break;

        // Leggi la linea separatrice
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;

        // Converto orario da stringa HH:MM a interi ore e minuti
        if (sscanf(orario_str, "%d:%d", &ore, &minuti) != 2)
        {
            printf("Formato orario non valido per ID %s\n", id);
            continue;
        }

        Data data = crea_data(giorno, mese, anno);
        Orario ora_lez = crea_orario(ore, minuti);

        Lezione lez = crea_lezione(id, nome, data, ora_lez, posti_max);
        set_posti_occupati(lez, posti_occupati);

        l = consList(lez, l); 
    }

    fclose(fp);
    return l;
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

int leggi_intero()
{
    char buffer[64];
    int valore;

    while (1)
    {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            continue;

        if (sscanf(buffer, "%d", &valore) == 1)
            return valore;

        printf("Input non valido. Inserisci un numero intero: ");
    }
}

int solo_lettere(char* s)
{
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (!( (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') ))
        {
            return 0; // Non è una lettera
        }
    }
    return 1; // Tutti i caratteri sono lettere
}
