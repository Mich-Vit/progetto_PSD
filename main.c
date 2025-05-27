//FILE main.c

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
#include "menu_testing.h"

int main(void)
{
    int sc;
    hashtable h = newHashtable(50);  
    carica_clienti_da_file(h);
    list l = newList();
    l = carica_lezioni_da_file(l);
    hashtable_p hp = newHashtable_p(50);
    carica_prenotazioni_da_file(hp);

    do
    {
        pulisci_schermo();
        printf("==============================================\n");
        printf("\tBENVENUTO NELLA TUA PALESTRA !\n");
        printf("==============================================\n");
        printf("1) Per il gestore della palestra\n");
        printf("2) Per l'area cliente (Devi conoscere il tuo ID)\n");
        printf("3) Per i casi di test\n");
        printf("0) Per uscire\n");
        printf("Scelta: ");
        sc = leggi_intero();

        switch (sc) 
        {
            case 1:
            {
                pulisci_schermo();
                menu_gestore(h, l, hp);
                // Dopo aver potenzialmente aggiunto o rimosso lezioni nel menu_gestore,
                // ricarico da file per aggiornare la lista reale
                l = carica_lezioni_da_file(newList());
                break;
            }
            case 2:
            {
                pulisci_schermo();
                printf("==============================================\n");
                printf("\tBENVENUTO NELLA TUA AREA !\n");
                printf("==============================================\n");
                Cliente cliente = login_cliente(h);

                if (cliente != NULL)
                {
                    pulisci_schermo();
                    menu_cliente(cliente, h, l, hp);
                }
                break;
            }
            case 3:
            {
                pulisci_schermo();
                
                menu_testing(h, l, hp);

                break;
            }
            case 0:
            {
                genera_report_mensile(hp, l);
                printf("Uscita dal programma.\n");
                break;
            }
            default:
                printf("Opzione non valida. Riprova.\n");
                printf("\nPremi INVIO...");
                while (getchar() != '\n');
                break;
        }
    } while (sc != 0);

    return 0;
}