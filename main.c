//FILE main.c

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"area_clienti.h"
#include"lista_lezioni.h"

int main(void)
{
    int sc;
    hashtable h = newHash();  // crea la tabella hash vuota
    list l = newList();      // crea la lista di lezioni vuota

    do
    {
        printf("===================================\n");
        printf("BENVENUTO NELLA TUA PALESTRA !\n");
        printf("===================================\n");
        printf("1) Per il gestore della palestra\n");
        printf("----------------------------------\n");
        printf("2) Per l'area cliente\n");
        printf("----------------------------------\n");
        printf("0) Per uscire\n");
        printf("Scelta: ");
        scanf("%d", &sc);

        switch (sc) 
        {
            case 1:
            {
                printf("Hai scelto l'area gestore.\n");
                // inserisci qui le funzioni per il gestore
                break;
            }
            case 2:
            {
                printf("Hai scelto l'area cliente.\n");
                Cliente cliente = login_cliente(h);  // hash_clienti deve essere inizializzata prima

                if (cliente != NULL)
                {
                    menu_cliente(cliente, h, l);  // passa anche lista_lezioni
                }
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