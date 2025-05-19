#pragma once

#include "types.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Nod* creare_nod(const char* hint, const char* answer);
void adauga_nod_final(Nod** cap, const char* hint, const char* answer);
void elibereaza_lista(Nod** cap);
Nod* incarca_intrebari_din_json(const char *nume_fisier);
Nod* incarca_intrebari_din_fisier(const char* nume_fisier);