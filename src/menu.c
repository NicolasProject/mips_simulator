#include "menu.h"

int menu(char * chaine, struct data_mem *dm){
	
	char fonction[20];
	char nom_reg[5];
	int quitter_reessayer =0;
	
	sscanf(chaine,"%s %s", fonction, nom_reg);

    if((strcasecmp(fonction, "print_reg"))==0){
        printf("registre : %i (%s)       Valeur : %i \n", reg_num(nom_reg), reg_file[reg_num(nom_reg)].alt_name, reg_file[reg_num(nom_reg)].val);
    }
    else if((strcasecmp(fonction, "print_mem"))==0){
        printf("case memoire : %s       Valeur : %i \n", nom_reg, (dm->mem[atoi(nom_reg)].val));
    }
    else if(strcasecmp(fonction, "exit")==0){
        quitter_reessayer = 1;
    }
    else{
        quitter_reessayer = 2;
        printf("commande non reconnue veuillez reessayer\n");
    }
	
	return quitter_reessayer;
}

void clear_stdin(void){
    int c;
    while((c = getchar()) != '\n' && c != EOF){}
}