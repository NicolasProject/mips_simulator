#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "registers.h"
#include "data_mem.h"


int menu(char * chaine, struct data_mem *dm);
void clear_stdin(void);

#endif 