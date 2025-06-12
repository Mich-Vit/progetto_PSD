#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"area_clienti.h"
#include"area_gestore.h"
#include"lista_lezioni.h"
#include"utils.h"
#include "hash.h"
#include "hash_prenotazioni.h"
#include "report.h"

/*
* Funzione: confronta_file
* ----------------------------------------
* Confronta due file riga per riga per verificarne l’identicità.
*
* Parametri:
*   oracle: percorso del file oracle.
*   output: percorso del file output (generato).
*
* Pre-condizione:
*   I file devono esistere e contenere lo stesso numero di righe significative.
*
* Post-condizione:
*   Ritorna 1 se i file sono identici, altrimenti 0.
*
* Come funziona:
* - Apre entrambi i file in lettura.
* - Confronta ogni riga.
* - Se trova una differenza, ritorna 0.
* - Se tutti i confronti sono uguali, ritorna 1.
*/
int confronta_file(char *oracle, char *output)
{
    FILE *fp_oracle, *fp_output;
 
    if((fp_oracle = fopen(oracle, "r")) == NULL)
    {
        printf("Errore nell'apertura del file di oracle\n");
        exit(EXIT_FAILURE);
    }

    if((fp_output = fopen(output, "r")) == NULL)
    {
        printf("Errore nell'apertura del file di output\n");
        exit(EXIT_FAILURE);
    }

    char buffer[256], buffer2[256];
    while (fgets(buffer, sizeof(buffer), fp_oracle) && fgets(buffer2, sizeof(buffer2), fp_output))
    {
        if(strcmp(buffer,buffer2)!=0)
        {
            printf("RIGA DIVERSA\nOracle: [%s]\nOutput: [%s]\n", buffer, buffer2); //DEBUG
            return 0;
        }
    }

    fclose(fp_oracle);
    fclose(fp_output);

    return 1;  
}

/*
* Funzione: salva_prenotazione_test
* ----------------------------------------
* Salva su file i dati relativi a una prenotazione, in formato leggibile.
*
* Parametri:
*   p: prenotazione da salvare.
*   po: numero di posti occupati aggiornato.
*   output_fname: nome del file di output dove scrivere i dati.
*
* Pre-condizione:
*   La prenotazione deve essere valida.
*
* Post-condizione:
*   I dati della prenotazione vengono scritti su file.
*
* Come funziona:
* - Apre un file in scrittura.
* - Scrive l’ID della prenotazione, cliente, lezione, data e posti occupati.
* - Chiude il file.
*/
void salva_prenotazione_test(Prenotazione p, int po, const char* output_fname)
{
    FILE *fp = fopen(output_fname, "w");
    if (fp == NULL)
    {
        printf("Errore nell'aprire il file %s.\n", output_fname);
        return;
    }

    fprintf(fp, "ID: %s\n", get_id_prenotazione(p));
    fprintf(fp, "ID Cliente: %s\n", get_id_cliente_prenotazione(p));
    fprintf(fp, "ID Lezione: %s\n", get_id_lezione_prenotazione(p));
    fprintf(fp, "Data Prenotazione: %02d/%02d/%04d\n",
            get_giorno(get_data_prenotazione(p)),
            get_mese(get_data_prenotazione(p)),
            get_anno(get_data_prenotazione(p)));

    fprintf(fp, "-----------------------\n");
    fprintf(fp, "Posti occupati: %d", po);
    fclose(fp);
}

/*
* Funzione: prenota_lezione_test_1
* -----------------------------------
* Permette a un cliente di prenotare una lezione in un ambiente di test.
*
* Parametri:
*   c: cliente autenticato che vuole prenotare.
*   l: lista delle lezioni disponibili.
*   hp: hashtable delle prenotazioni.
*   id_lezione_scelta: ID della lezione da prenotare.
*   ncase: numero del caso di test.
*
* Pre-condizione:
*   Il cliente deve esistere ed essere autenticato.
*   Deve avere un abbonamento valido.
*   La lista delle lezioni e la hashtable devono essere inizializzate.
*
* Post-condizione:
*   Se la prenotazione è possibile, viene effettuata e salvata su file.
*   Al termine del test, lo stato viene ripristinato.
*
* Come funziona:
* - Verifica la validità dell’abbonamento e dell’ID lezione.
* - Controlla se ci sono posti disponibili.
* - Evita duplicati di prenotazione.
* - Genera un ID per la prenotazione.
* - Inserisce temporaneamente la prenotazione nella hashtable.
* - Salva i dettagli su file e rimuove la prenotazione.
*/
int prenota_lezione_test(Cliente c, list l, hashtable_p hp, char *id_lezione_scelta, const char* output_fname)
{
    if(c == NULL || hp == NULL)
    {
        return 0;
    }

    if(!abbonamento_valido(data_oggi(), get_data_scadenza(c)))
    {
        return 0;
    }

    list tmp = l;
    int trovate = 0;

    if(emptyList(l))
    {
        return 0;
    }

    // Cerca la lezione nella lista
    Lezione l_selezionata = cerca_lezione_per_id(l, id_lezione_scelta);
    if (l_selezionata == NULL)
    {
        return 0;
    }

    if (confronta_date(get_data_lezione(l_selezionata), get_data_scadenza(c)) > 0)
    {
        return 0;
    }
    else if (confronta_date(data_oggi(), get_data_lezione(l_selezionata)) > 0)
    {
        return 0;
    }

    int posti_occupati = get_posti_occupati(l_selezionata);
    int posti_max = get_posti_max(l_selezionata);
    int posti_disponibili = posti_max - posti_occupati;

    if (posti_disponibili <= 0)
    {
        return 0;
    }

    // Verifica se il cliente è già prenotato alla lezione
    Prenotazione* table = get_table_hash_p(hp);
    for (int i = 0; i < get_size_hash_p(hp); i++)
    {
        Prenotazione curr = table[i];
        while (curr != NULL)
        {
            if (strcmp(get_id_cliente_prenotazione(curr), get_id_cliente(c)) == 0 &&
            strcmp(get_id_lezione_prenotazione(curr), get_id_lezione(l_selezionata)) == 0)
            {
                return 0;
            }
            curr = get_next_prenotazione(curr);
        }
    }

    set_posti_occupati(l_selezionata, posti_occupati + 1);

    Data data_pre = data_oggi();

    char *id_prenotazione = genera_id_generico("P", "prenotazioni.txt",1);
    Prenotazione p = crea_prenotazione(id_prenotazione, get_id_cliente(c), get_id_lezione(l_selezionata), data_pre);
    free(id_prenotazione);

    insertHash_p(hp, p);

    int posti_occupati_nuovo = get_posti_occupati(l_selezionata);
    salva_prenotazione_test(p, posti_occupati_nuovo, output_fname);

    set_posti_occupati(l_selezionata, posti_occupati_nuovo - 1);

    hashDelete_p(hp, get_id_prenotazione(p));

    return 1;
}

/*
* Funzione: esegui_test_prenotazione
* ----------------------------------------
* Esegue un test automatico per la prenotazione lezioni.
*
* Parametri:
*   input_fname: nome del file con input di test.
*   output_fname: file dove scrivere l’output.
*   oracle_fname: file oracle per confronto.
*   h: hashtable dei clienti.
*   l: lista delle lezioni.
*   hp: hashtable delle prenotazioni.
*
* Pre-condizione:
*   File di input deve esistere e contenere dati corretti.
*
* Post-condizione:
*   Il risultato del test viene confrontato con il file oracle.
*
* Come funziona:
* - Legge le righe di input e tenta la prenotazione per ogni coppia cliente-lezione.
* - Scrive l’esito sul file output.
* - Confronta l’output con il file oracle.
*/
int esegui_test_prenotazione(char *input_fname, char *output_fname, char *oracle_fname, hashtable h, list l, hashtable_p hp)
{
    FILE *fp_input, *fp_output;

    if((fp_input = fopen(input_fname, "r")) == NULL)
    {
        printf("Errore nell'apertura del file di input\n");
        return 0;
    }

    if((fp_output = fopen(output_fname, "w")) == NULL)
    {
        printf("Errore nell'apertura del file di output\n");
        fclose(fp_input);
        return 0;
    }

    char buffer[256];
    char id_cliente[20], id_lezione[20];
    int k;

    while(fgets(buffer, sizeof(buffer), fp_input))
    {
        if (sscanf(buffer, "%s %s", id_cliente, id_lezione) == 2)
        {
            Cliente c = hashSearch(h, id_cliente);

            k = prenota_lezione_test(c, l, hp, id_lezione, output_fname);

            if(k == 0)
            {
                fprintf(fp_output, "Prenotazione fallita per %s", id_cliente);
            }
        }
    }

    fclose(fp_input);
    fclose(fp_output);

    // Ora confronta output con oracle
    int z;
    z = confronta_file(oracle_fname, output_fname);

    return z;  // 1 = PASS, 0 = FAIL
}

/*
* Funzione: salva_cliente_test
* ----------------------------------------
* Salva i dati relativi all’abbonamento di un cliente su file.
*
* Parametri:
*   c: cliente da salvare.
*   output_fname: nome del file su cui scrivere.
*
* Pre-condizione:
*   Cliente deve essere valido.
*
* Post-condizione:
*   I dati vengono scritti su file in formato leggibile.
*
* Come funziona:
* - Apre il file.
* - Scrive durata abbonamento e data di scadenza.
* - Chiude il file.
*/
void salva_cliente_test(Cliente c, const char* output_fname)
{
    FILE *fp = fopen(output_fname, "w");
    if (fp == NULL)
    {
        printf("Errore nell'aprire il file %s\n", output_fname);
        return;
    }
 
    fprintf(fp, "Durata abbonamento: %d\n", get_durata_abbonamento(c));

    fprintf(fp, "Data scadenza: %02d/%02d/%04d", 
            get_giorno(get_data_scadenza(c)), get_mese(get_data_scadenza(c)), get_anno(get_data_scadenza(c)));

    fclose(fp);
}

/*
* Funzione: rinnova_abbonamento_test
* ----------------------------------------
* Rinnova l’abbonamento di un cliente per un numero specificato di mesi.
*
* Parametri:
*   c: cliente da aggiornare.
*   h: hashtable dei clienti (non usata).
*   durata: mesi da aggiungere all’abbonamento.
*   output_fname: file dove scrivere i dati aggiornati.
*
* Pre-condizione:
*   Cliente deve esistere.
*
* Post-condizione:
*   La data di scadenza e la durata dell’abbonamento vengono aggiornate.
*   I dati vengono scritti su file.
*
* Come funziona:
* - Calcola nuova scadenza.
* - Aggiorna cliente.
* - Salva i dati aggiornati su file.
*/
int rinnova_abbonamento_test(Cliente c, hashtable h, int durata, const char* output_fname)
{
    if (c == NULL)
    {
        return 0;
    }

    Data data_scadenza_attuale = get_data_scadenza(c);

    char buffer[100];

    Data nuova_data_scadenza = calcolo_scadenza_abbonamento(data_scadenza_attuale, durata);
    set_data_scadenza(c, nuova_data_scadenza);

    // Calcola la durata effettiva dell'abbonamento (differenza tra data di scadenza e data di iscrizione)
    int durata_effettiva = calcola_durata_in_mesi(get_data_iscrizione(c), get_data_scadenza(c));    
    set_durata(c, durata_effettiva);

    salva_cliente_test(c, output_fname);

    return 1;
}

/*
* Funzione: esegui_test_abbonamenti
* ----------------------------------------
* Esegue un test automatico su abbonamenti: validità o rinnovo.
*
* Parametri:
*   input_fname: nome del file di input.
*   output_fname: nome del file di output.
*   oracle_fname: file con output atteso.
*   h: hashtable dei clienti.
*
* Pre-condizione:
*   Il file di input deve esistere e contenere ID cliente.
*
* Post-condizione:
*   L’output del test viene confrontato con l’oracle.
*
* Come funziona:
* - Legge i parametri (cliente e durata).
* - Esegue test di rinnovo o verifica validità.
* - Scrive esito su file.
* - Confronta con oracle.
*/
int esegui_test_abbonamenti(char *input_fname, char *output_fname, char *oracle_fname, hashtable h)
{
    FILE *fp_input, *fp_output;

    if((fp_input = fopen(input_fname, "r")) == NULL)
    {
        printf("Errore nell'apertura del file di input\n");
        return 0;
    }

    if((fp_output = fopen(output_fname, "w")) == NULL)
    {
        printf("Errore nell'apertura del file di output\n");
        fclose(fp_input);
        return 0;
    }

    char buffer[256];
    char id_cliente[20];
    int k, z, durata;

    if (fgets(buffer, sizeof(buffer), fp_input) == NULL)
    {
        printf("Errore nella lettura del file di input\n");
        fclose(fp_input);
        fclose(fp_output);
        return 0;
    }

    // Controlla se c'è anche una durata (2 parametri) o solo id_cliente (1 parametro)
    int parametri = sscanf(buffer, "%s %d", id_cliente, &durata);

    if (parametri == 2) // Test di rinnovo abbonamento
    {
        Cliente c = hashSearch(h, id_cliente);
        k = rinnova_abbonamento_test(c, h, durata, output_fname);
        if (k == 0)
        {
            fprintf(fp_output, "Rinnovo fallito per %s\n", id_cliente);
        }
    }
    else if (parametri == 1) // Test di validità abbonamento
    {
        Cliente c = hashSearch(h, id_cliente);

        if (c == NULL)
        {
            fprintf(fp_output, "Cliente non trovato");
        }
        else
        {
            int valido = abbonamento_valido(data_oggi(), get_data_scadenza(c));
            if(valido)
                fprintf(fp_output, "Abbonamento valido");
            else
                fprintf(fp_output, "Abbonamento non valido");
        }
    }

    fclose(fp_input);
    fclose(fp_output);

    // Ora confronta output con oracle
    z = confronta_file(oracle_fname, output_fname);

    return z;  // 1 = PASS, 0 = FAIL
}

/*
* Funzione: prenotazioni_per_report
* ----------------------------------------
* Prenota una lezione solo se valida per generare il report.
*
* Parametri:
*   c: cliente che prenota.
*   l: lista lezioni.
*   hp_test: hashtable temporanea per prenotazioni.
*   id_lezione: ID della lezione da prenotare.
*
* Pre-condizione:
*   Cliente e dati validi.
*
* Post-condizione:
*   La prenotazione viene inserita nella hashtable temporanea.
*
* Come funziona:
* - Controlla validità abbonamento, disponibilità e duplicati.
* - Inserisce la prenotazione in struttura temporanea.
*/
int prenotazioni_per_report(Cliente c, list l, hashtable_p hp_test, char *id_lezione)
{
    if(!abbonamento_valido(data_oggi(), get_data_scadenza(c)))
    {
        return 0;
    }

    list tmp = l;
    int trovate = 0;

    if(emptyList(l))
    {
        return 0;
    }

    // Cerca la lezione nella lista
    Lezione l_selezionata = cerca_lezione_per_id(l, id_lezione);
    if (l_selezionata == NULL)
    {
        return 0;
    }

    if (confronta_date(get_data_lezione(l_selezionata), get_data_scadenza(c)) > 0)
    {
        return 0;
    }
    else if (confronta_date(data_oggi(), get_data_lezione(l_selezionata)) > 0)
    {
        return 0;
    }

    int posti_occupati = get_posti_occupati(l_selezionata);
    int posti_max = get_posti_max(l_selezionata);
    int posti_disponibili = posti_max - posti_occupati;

    if (posti_disponibili <= 0)
    {
        return 0;
    }

    // Verifica se il cliente è già prenotato alla lezione
    Prenotazione* table = get_table_hash_p(hp_test);
    for (int i = 0; i < get_size_hash_p(hp_test); i++)
    {
        Prenotazione curr = table[i];
        while (curr != NULL)
        {
            if (strcmp(get_id_cliente_prenotazione(curr), get_id_cliente(c)) == 0 &&
            strcmp(get_id_lezione_prenotazione(curr), get_id_lezione(l_selezionata)) == 0)
            {
                return 0;
            }
            curr = get_next_prenotazione(curr);
        }
    }

    set_posti_occupati(l_selezionata, posti_occupati + 1);

    Data data_pre = data_oggi();

    char *id_prenotazione = genera_id_generico("P", "prenotazioni.txt",0);
    Prenotazione p = crea_prenotazione(id_prenotazione, get_id_cliente(c), get_id_lezione(l_selezionata), data_pre);
    free(id_prenotazione);

    insertHash_p(hp_test, p);

    return 1;
}

/*
* Funzione: esegui_test_report
* ----------------------------------------
* Esegue test per la generazione del report delle prenotazioni.
*
* Parametri:
*   input_fname: nome del file di input.
*   output_fname: nome del file output.
*   oracle_fname: nome del file oracle.
*   h: hashtable dei clienti.
*   l: lista lezioni.
*
* Pre-condizione:
*   I file devono esistere e contenere dati coerenti.
*
* Post-condizione:
*   Il report viene generato e confrontato con il file oracle.
*
* Come funziona:
* - Legge prenotazioni da file input.
* - Inserisce in hashtable temporanea.
* - Genera report.
* - Confronta con oracle.
*/
int esegui_test_report(char *input_fname, char *output_fname, char *oracle_fname, hashtable h, list l)
{
    hashtable_p hp_test = newHashtable_p(50);  // Hash temporanea per test

    FILE *fp_input, *fp_output;

    if((fp_input = fopen(input_fname, "r")) == NULL)
    {
        printf("Errore nell'apertura del file di input\n");
        return 0;
    }

    if((fp_output = fopen(output_fname, "w")) == NULL)
    {
        printf("Errore nell'apertura del file di output\n");
        fclose(fp_input);
        return 0;
    }

    char buffer[256];
    char id_cliente[20], id_lezione[20];
    int k;

    while(fgets(buffer, sizeof(buffer), fp_input))
    {
        if (sscanf(buffer, "%s %s", id_cliente, id_lezione) == 2)
        {
            Cliente c = hashSearch(h, id_cliente);

            k = prenotazioni_per_report(c, l, hp_test, id_lezione);

            if(k == 0)
            {
                fprintf(fp_output, "Prenotazione fallita per %s", id_cliente);
            }
        }
    }

    genera_report_test(hp_test, l, output_fname);

    // Confronto con oracle
    int z = confronta_file(oracle_fname, output_fname);

    return z;
}

/*
* Funzione: run_test_case
* ----------------------------------------
* Avvia l’esecuzione di un caso di test specificato.
*
* Parametri:
*   tc_id: identificatore del caso di test.
*   tipo_test: tipo di test (1 = prenotazione, 2 = abbonamenti, 3 = report).
*   h: hashtable dei clienti.
*   l: lista lezioni.
*   hp: hashtable prenotazioni.
*
* Pre-condizione:
*   ID e tipo test validi.
*
* Post-condizione:
*   Esegue e ritorna il risultato del test.
*
* Come funziona:
* - Costruisce i nomi dei file input/output/oracle.
* - Chiama la funzione di test in base al tipo.
*/
int run_test_case(char *tc_id, int tipo_test, hashtable h, list l, hashtable_p hp)
{
    int esito = 0;
    char input_fname[20], output_fname[20], oracle_fname[20];	

    // costruiamo i nomi dei file
    sprintf(input_fname, "%s_input.txt", tc_id);
    sprintf(output_fname, "%s_output.txt", tc_id);
    sprintf(oracle_fname, "%s_oracle.txt", tc_id);

    printf("Input: %s\n", input_fname);  // Debug
    printf("Output: %s\n", output_fname);  // Debug
    printf("Oracle: %s\n", oracle_fname);  // Debug


    switch (tipo_test)
    {
        case 1:
            esito = esegui_test_prenotazione(input_fname, output_fname, oracle_fname, h, l, hp);
            break;
        case 2:
            esito = esegui_test_abbonamenti(input_fname, output_fname, oracle_fname, h);
            break;
        case 3:
            esito = esegui_test_report(input_fname, output_fname, oracle_fname, h, l);
            break;
        default:
            printf("Tipo di test non riconosciuto: %d\n", tipo_test);
            break;
    }

    return esito;
}

int main(void)
{
    hashtable h = newHashtable(50);  
    carica_clienti_da_file(h);
    list l = newList();
    l = carica_lezioni_da_file(l);
    hashtable_p hp = newHashtable_p(50);
    carica_prenotazioni_da_file(hp);

    FILE *test_suite, *result; 
    char tc_id[20];  // stringa contenente l'identificativo del test case
    int pass, tipo_test;

    test_suite = fopen("test_suite.txt", "r");  // apro file contenente test suite
    result = fopen("result.txt", "w");   // apro file contenente risultati test

    if(test_suite == NULL || result == NULL ) 
    {
        printf("Errore in apertura dei file \n");    
        exit(1);
    }

    /* Scansione del file di input nel ciclo while. 
    Ad ogni iterazione si leggono gli elementi di una riga:
    l'identificativo del test case, va in tc_id;
    il tipo di test, va in tipo_test */
    while(fscanf(test_suite, "%s %d", tc_id, &tipo_test) == 2) 
    {
        pass = run_test_case(tc_id, tipo_test, h, l, hp);
    
        fprintf(result,"%s ", tc_id); //scrive nel file result, la stringa tc_id
        if(pass == 1)
            fprintf(result, "PASS \n");
        else   
           fprintf(result, "FAIL \n");	
    }	
    
    fclose(test_suite); 
    fclose(result);  
    
    printf("=====================================================\n");
    printf("FILE GENERATI, CONTROLLARE I RISULTATI NELLA CARTELLA\n");
    printf("\nPremi INVIO PER LA FINE DEL PROGRAMMA...");
    while (getchar() != '\n');

    return 0;
}