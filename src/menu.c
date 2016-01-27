#include "menu.h"

void menu(char * chaine, struct data_mem *dm){
	
	char fonction[20];
	char nom_reg[5];
	
	sscanf(chaine,"%s %s", fonction, nom_reg);

    if((strcasecmp(fonction, "print_reg"))==0){
        printf("registre : %i (%s)       Valeur : %i \n", reg_num(nom_reg), reg_file[reg_num(nom_reg)].alt_name, reg_file[reg_num(nom_reg)].val);
    }
    else if((strcasecmp(fonction, "print_mem"))==0){
        printf("case memoire : %s       Valeur : %i \n", nom_reg, (dm->mem[atoi(nom_reg)].val));
    }
	
}