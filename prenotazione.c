//FILE prenotazione.c

#include<stdio.h>
#include<stdlib.h>
#include"prenotazione.h"
#include"data.h"

struct prenotazione
{
    int id_cliente;   // ID del cliente che prenota
    int id_lezione;   // ID della lezione prenotata
    Data data_prenotazione;
};