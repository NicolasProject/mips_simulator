#include <stdio.h>
#include <stdlib.h>
#include "file_functions.h"
#include "instruction_mem.h"
#include "registers.h"


int main(int argc,char*args[])
{
	init_reg_file();		// Initialize the register file
	label_num=0;
	int sortieBoucle = 0;
	char chaine[50]= "";
	
	FILE* fichier =NULL;
	int len = 0;
	
	struct instruct_mem *im = calloc(sizeof(struct instruct_mem),1);
	struct data_mem *dm = calloc(sizeof(struct data_mem),1);
		
	if(argc > 1){
		fichier = fopen(args[1],"r");
		if(fichier != NULL){
			len = read_file(fichier, im, dm);	// len stores the largest possible value of pc.
			fclose(fichier);
			execute(im,len,dm);
		}
		else{
			printf("problème lors de l'ouverture du fichier");
		}
	}
	else{
		do{
			fichier = fopen("./mode_interactif.txt","w");
			if(fichier != NULL){
				printf("entrez une instruction :\n");
				fgets(chaine, sizeof(chaine), stdin);
				
				if(strcmp(chaine,"EXIT\n")==0){
					sortieBoucle = 1;
				}
				else{
					fprintf(fichier, "%s", chaine);
					fflush(fichier);
					fclose(fichier);
					
					fichier = fopen("./mode_interactif.txt","r");
					if(fichier != NULL){
						read_file(fichier, im, dm);
						fclose(fichier);
						execute(im,1,dm);
						afficher_registres();
					}
					else{
						printf("problème lors de l'ouverture du fichier en lecture");
					}
				}
			}
			else{
				printf("problème lors de l'ouverture du fichier en écriture");
			}
			
		}while(sortieBoucle ==0);
	}
	
	
	return 0;
}
