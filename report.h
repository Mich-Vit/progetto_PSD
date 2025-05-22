//FILE report.h

#ifndef REPORT_H
#define REPORT_H

#include "hash_prenotazioni.h"
#include "lista_lezioni.h"

typedef struct statistica *Stats;

void genera_report_mensile(hashtable_p hp, list l);

#endif