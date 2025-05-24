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

void rinnova_abbonamento(Cliente c, hashtable h)
{
    if (c == NULL || h == NULL)
    {
        printf("Cliente o tabella non valida.\n");
        return;
    }

    Data data_scadenza_attuale = get_data_scadenza(c);

    int durata;
    char buffer[100];
    int valido = 0;

    do
    {
        printf("Inserisci la durata del rinnovo in mesi (numero intero positivo): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Controlla che sia un numero intero e che non ci siano caratteri extra
        if (sscanf(buffer, "%d", &durata) == 1 && durata >= 0)
        {
            valido = 1;
        }
        else
        {
            printf("Errore: inserisci un numero intero valido e positivo.\n");
        }
    } while (!valido);

    // Calcola la nuova data di scadenza aggiungendo la durata all'abbonamento
    Data nuova_data_scadenza = calcolo_scadenza_abbonamento(data_scadenza_attuale, durata);
    set_data_scadenza(c, nuova_data_scadenza);

    // Calcola la durata effettiva dell'abbonamento (differenza tra data di scadenza e data di iscrizione)
    int durata_effettiva = calcola_durata_in_mesi(get_data_iscrizione(c), get_data_scadenza(c));    
    set_durata(c, durata_effettiva);

    aggiorna_file_clienti(h);

    printf("Il tuo abbonamento e' stato rinnovato fino al ");
    visualizza_data(nuova_data_scadenza);  // Funzione per visualizzare la data
    printf("\n");
}

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

void aggiorna_file_clienti(hashtable h)
{
    FILE *fp = fopen("clienti.txt", "w");
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file clienti.txt.\n");
        return;
    }

    Cliente *table = get_table_hash(h); 
    int size = get_size_hash(h);       

    for (int i = 0; i < size; i++)
    {
        Cliente curr = table[i];
        while (curr != NULL)
        {
            fprintf(fp, "ID: %s\n", get_id_cliente(curr));
            fprintf(fp, "Nome: %s\n", get_nome_cliente(curr));
            fprintf(fp, "Cognome: %s\n", get_cognome_cliente(curr));
            fprintf(fp, "Durata abbonamento: %d\n", get_durata_abbonamento(curr));
            fprintf(fp, "Data d'iscrizione: %02d/%02d/%04d\n",
                    get_giorno(get_data_iscrizione(curr)),
                    get_mese(get_data_iscrizione(curr)),
                    get_anno(get_data_iscrizione(curr)));
            fprintf(fp, "Data scadenza: %02d/%02d/%04d\n",
                    get_giorno(get_data_scadenza(curr)),
                    get_mese(get_data_scadenza(curr)),
                    get_anno(get_data_scadenza(curr)));
            fprintf(fp, "-----------------------\n");

            curr = get_next_cliente(curr);
        }
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
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "ID: %s", id) != 1)
            break;

        // Leggi Nome (anche con spazi)
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Nome: %[^\n]", nome) != 1)
            break;

        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Data: %d/%d/%d", &giorno, &mese, &anno) != 3)
            break;

        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Orario: %s", orario_str) != 1)
            break;

        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
        if (sscanf(buffer, "Posti massimi: %d", &posti_max) != 1)
            break;

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

void stampa_prenotazioni_cliente(Cliente c, hashtable_p hp, list l)
{
    printf("======================================================\n");
    printf("LISTA DELLE PRENOTAZIONI DI: %s %s!\n", get_nome_cliente(c), get_cognome_cliente(c));
    printf("======================================================\n");

    if (hp == NULL)
    {
        printf("Nessuna tabella di prenotazioni trovata.\n");
        return;
    }

    char* id_cliente = get_id_cliente(c);
    Prenotazione* table = get_table_hash_p(hp);
    int size = get_size_hash_p(hp);
    int trovata = 0;

    printf("%-10s %-20s %-16s\n", "ID", "LEZIONE", "DATA\t    ORA");
    

    for (int i = 0; i < size; i++)
    {
        Prenotazione curr = table[i];
        while (curr != NULL)
        {
            if (strcmp(get_id_cliente_prenotazione(curr), id_cliente) == 0)
            {
                trovata = 1;

                char* id_lezione = get_id_lezione_prenotazione(curr);
                char nome_lezione[50] = "Lezione non trovata";
                int giorno = 0, mese = 0, anno = 0, ore = 0, minuti = 0;

                // Cerca la lezione nella lista
                list temp = l;
                while (!emptyList(temp))
                {
                    Lezione le = getFirst(temp);
                    if (strcmp(get_id_lezione(le), id_lezione) == 0)
                    {
                        strcpy(nome_lezione, get_nome_lezione(le));
                        giorno = get_giorno(get_data_lezione(le));
                        mese = get_mese(get_data_lezione(le));
                        anno = get_anno(get_data_lezione(le));
                        ore = get_ora(get_ora_lezione(le));
                        minuti = get_minuti(get_ora_lezione(le));
                        break;
                    }
                    temp = tailList(temp);
                }

                printf("%-10s %-20s %02d/%02d/%04d %02d:%02d\n",
                       get_id_prenotazione(curr),
                       nome_lezione,
                       giorno, mese, anno,
                       ore, minuti);
            }
            curr = get_next_prenotazione(curr);
        }
    }

    if (!trovata)
        printf("Nessuna prenotazione trovata per questo cliente.\n");
}

void stampa_prenotazioni_lezione(hashtable h, Lezione lezione, hashtable_p hp)
{
    if (lezione == NULL || hp == NULL || h == NULL) {
        printf("Errore: dati mancanti.\n");
        return;
    }

    // Ottieni gli attributi della lezione tramite getter
    char* id_lez = get_id_lezione(lezione);
    char* nome_lez = get_nome_lezione(lezione);
    Data data = get_data_lezione(lezione);
    Orario orario = get_ora_lezione(lezione);

    // Estrai giorno, mese, anno da Data
    int giorno = get_giorno(data);
    int mese = get_mese(data);
    int anno = get_anno(data);

    // Estrai ora e minuti da Orario
    int ore = get_ora(orario);
    int minuti = get_minuti(orario);

    printf("======================================================\n");
    printf("LISTA DELLE PRENOTAZIONI DELLA LEZIONE DI: %s!\n", nome_lez);
    printf("DATA: %02d/%02d/%04d    ORARIO: %02d:%02d\n", giorno, mese, anno, ore, minuti);
    printf("======================================================\n");

    printf("%-10s %-15s %-15s\n", "ID", "CLIENTE", "COGNOME");

    Prenotazione* tabella = get_table_hash_p(hp);  // ottieni array prenotazioni
    int size = get_size_hash_p(hp);

    int trovata = 0;

    // Scorri tutta la tabella delle prenotazioni
    for (int i = 0; i < size; i++) 
    {
        Prenotazione p = tabella[i];
        while (p != NULL) 
        {
            // Verifica se la prenotazione è della lezione richiesta
            if (strcmp(get_id_lezione_prenotazione(p), id_lez) == 0) 
            {
                trovata = 1;
                char* id_cli = get_id_cliente_prenotazione(p);

                // Cerca il cliente associato
                Cliente cliente = hashSearch(h, id_cli);
                if (cliente != NULL) 
                {
                    printf("%-10s %-15s %-15s\n",
                           get_id_prenotazione(p),
                           get_nome_cliente(cliente),
                           get_cognome_cliente(cliente));
                } 
                else 
                {
                    printf("%-10s %-15s %-15s\n",
                           get_id_prenotazione(p), "Cliente", "Non Trovato");
                }
            }
            p = get_next_prenotazione(p);
        }
    }

    if (!trovata) 
    {
        printf("Nessuna prenotazione trovata per questa lezione.\n");
    }
}

Lezione cerca_lezione_per_id(list l, const char *id_lezione)
{
    while (!emptyList(l))
    {
        Lezione le = getFirst(l);
        if (le != NULL && strcmp(get_id_lezione(le), id_lezione) == 0)
        {
            return le;
        }
        l = tailList(l);
    }
    return NULL;  // Non trovata
}

void stampa_lezioni_libere(list l)
{
    Lezione le;
    l = ordina_Lista(l);
    Data oggi = data_oggi();

    printf("%-8s %-15s %-10s %-12s %s\n", "ID", "NOME", "ORA", "DATA", "POSTI");
    printf("======================================================\n");

    while (!emptyList(l))
    {
        le = getFirst(l);

        int posti_occupati = get_posti_occupati(le);
        int posti_max = get_posti_max(le);
        int posti_disponibili = posti_max - posti_occupati;

        if (posti_disponibili > 0 && confronta_date(get_data_lezione(le), oggi) >= 0)
        {
            visualizza_essenziale_lezione(le, posti_occupati, posti_max);
        }

        l = tailList(l);
    }

    libera_data(oggi); // libera la memoria se data_oggi() restituisce data allocata dinamicamente
}

Data calcolo_scadenza_abbonamento(Data data_inizio, int durata_abbonamento) 
{
    int giorno = get_giorno(data_inizio);
    int mese = get_mese(data_inizio);
    int anno = get_anno(data_inizio);

    mese += durata_abbonamento;

    // Gestione overflow mesi
    while (mese > 12) 
    {
        mese -= 12;
        anno += 1;
    }

    Data data_scadenza = crea_data(giorno, mese, anno);

    return data_scadenza;
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
    int contiene_lettera = 0;

    for (int i = 0; s[i] != '\0'; i++)
    {
        if ((s[i] >= '0' && s[i] <= '9') ||        // Cifre non ammesse
            !( (s[i] >= 'A' && s[i] <= 'Z') ||     // Lettere maiuscole
               (s[i] >= 'a' && s[i] <= 'z') ||     // Lettere minuscole
               s[i] == ' '))                      // Solo spazi ammessi
        {
            return 0; // Carattere non valido
        }

        if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z')) {
            contiene_lettera = 1;
        }
    }

    return contiene_lettera;
}

// Converte mese numerico in nome
const char* nome_mese(int mese)
{
    const char* mesi[] = {"Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno",
                          "Luglio", "Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre"};
    if (mese >= 1 && mese <= 12)
        return mesi[mese - 1];
    return "Mese sconosciuto";
}
