#include "menu.h"

int menu(char * chaine, struct data_mem *dm){   // Paramètres d'entrée : chaine de caractère entrée par l'utilisateur, mémoire de donnée
	
	char fonction[20];          // Chaine qui contiendra la fonction (exit, print_reg ou print_mem)
	char reg_or_memAdress[5];   // Chaine qui contiendra le numero du registre à afficher ou l'adresse de la case memoire à afficher
	int quitter_reessayer =0;   // Entier qui sera retourné et qui vaudra 0 si on affiche un registre ou une case mémoire
	                            //                                        1 si on souhaite quitter
	                            //                                        2 si la commande n'est pas reconnue
	
	sscanf(chaine,"%s %s", fonction, reg_or_memAdress);     // On scan la chaine de caractère et récupère la fonction et le numero de registre ou l'adresse

    if((strcasecmp(fonction, "print_reg"))==0)      // Si la fonction est print_reg
    {
        printf("registre : %i (%s)       Valeur : %i \n",   // On affiche :
            reg_num(reg_or_memAdress),                          // le numero de registre,
            reg_file[reg_num(reg_or_memAdress)].alt_name,       // son nom,
            reg_file[reg_num(reg_or_memAdress)].val);           // ainsi que sa valeur.
    }
    
    else if((strcasecmp(fonction, "print_mem"))==0)  // Si la fonction est print_mem
    {
        printf("case memoire : %s       Valeur : %i \n",    // On affiche :
            reg_or_memAdress,                                   // l'adresse mémoire,
            (dm->mem[atoi(reg_or_memAdress)].val));             // la valeur de cette case mémoire.
    }
    
    else if(strcasecmp(fonction, "exit")==0)        // Si la fonction est exit ou EXIT
    {
        quitter_reessayer = 1; 
    }
    
    else                                            // Si la commande n'est pas reconnue
    {
        quitter_reessayer = 2;
        printf("commande non reconnue veuillez reessayer\n");
    }
	
	return quitter_reessayer;      
}

void clear_stdin(void){     // fonction qui purge le flux d'entrée standart
    int c;
    while((c = getchar()) != '\n' && c != EOF){}
}