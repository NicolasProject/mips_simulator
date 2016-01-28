#include <stdio.h>
#include <stdlib.h>
#include "file_functions.h"
#include "instruction_mem.h"
#include "registers.h"
#include "menu.h"


int main(int argc,char*args[])
{
	init_reg_file();		// Initialize the register file
	label_num=0;
	int sortieBoucle = 0;
	char chaine[99]= "";
	
	FILE* fichier =NULL;
	int len = 0;
	
	struct instruct_mem *im = calloc(sizeof(struct instruct_mem),1);
	struct data_mem *dm = calloc(sizeof(struct data_mem),1);
		
	if(argc == 2){		//mode fichier
		fichier = fopen(args[1],"r");
		if(fichier != NULL){
			len = read_file(fichier, im, dm);	// len stores the largest possible value of pc.
			fclose(fichier);
			execute(im,len-1,dm,0);
		}
		else{
			printf("problème lors de l'ouverture du fichier");
		}
	}
	
	else if((argc == 3) && !strcmp(args[2],"-pas")){	// mode fichier pas a pas
		fichier = fopen(args[1],"r");
		if(fichier != NULL){
			len = read_file(fichier, im, dm);	// len stores the largest possible value of pc.
			fclose(fichier);
			execute(im,len-1,dm,1);
		}
		else{
			printf("problème lors de l'ouverture du fichier");
		}
	}
	
	else if(argc == 1){		// mode interactif
		do{
				printf("entrez une instruction :\n");
				fgets(chaine, sizeof(chaine), stdin);
				
				if((strcasecmp(chaine,"EXIT\n"))==0 ||(strncmp(chaine,"print_reg",9))==0 ||(strncmp(chaine,"print_mem",9))==0){
					do{
						sortieBoucle = menu(chaine, dm);
					}while(sortieBoucle == 2);
				}
				else{
					fichier = fopen("./mode_interactif.txt","w");
					if(fichier != NULL){
						
						fprintf(fichier, "%s", chaine);
						fclose(fichier);
						
						fichier = fopen("./mode_interactif.txt","r");
						if(fichier != NULL){
							read_file(fichier, im, dm);
							fclose(fichier);
							execute(im,0,dm,0);
						}
						else{
							printf("problème lors de l'ouverture du fichier en lecture");
						}
					}
					else{
						printf("problème lors de l'ouverture du fichier en écriture");
					}
				}
			
		}while(sortieBoucle ==0);
		
		//remove("./mode_interactif.txt");
	}
	
	
	
	
	return 0;
}
