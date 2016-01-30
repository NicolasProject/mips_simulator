#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "registers.h"
#include "data_mem.h"


int menu(char * chaine, struct data_mem *dm);   // éxecute une action selon ce que l'utilisateur a entrée dans "chaine"
void clear_stdin(void);                         // purge le flux d'entrée

#endif 