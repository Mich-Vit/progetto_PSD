//FILE main.c

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"area_clienti.h"
#include"area_gestore.h"
#include"lista_lezioni.h"

int main(void)
{
    int sc;
    hashtable h = newHashtable(10);  
    list l = newList();      // crea la lista di lezioni vuota

    do
    {
        printf("===================================\n");
        printf("BENVENUTO NELLA TUA PALESTRA !\n");
        printf("===================================\n");
        printf("1) Per il gestore della palestra\n");
        printf("2) Per l'area cliente\n");
        printf("0) Per uscire\n");
        printf("Scelta: ");
        scanf("%d", &sc);
         // Pulisce il buffer dopo scanf
        while (getchar() != '\n'); 

        switch (sc) 
        {
            case 1:
            {
                printf("Hai scelto l'area gestore.\n");
                // inserisci qui le funzioni per il gestore
                inserisci_cliente(h);  // Verifica che i dati vengano inseriti
                break;
            }
            case 2:
            {
                printf("Hai scelto l'area cliente.\n");
                Cliente cliente = login_cliente(h);

                if (cliente != NULL)
                {
                    menu_cliente(cliente, h, l);
                }
                break;
            }
            case 0:
            {
                printf("Uscita dal programma.\n");
                break;
            }
            default:
                printf("Scelta non valida. Riprova.\n");
        }
    } while (sc != 0);




    return 0;
}