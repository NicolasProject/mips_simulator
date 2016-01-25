#include<stdio.h>
#include "../include/file_functions.h"
#include "../include/instruction_mem.h"
#include "../include/registers.h"


int main(int argc,char*args[])
{
	init_reg_file();		// Initialize the register file
	label_num=0;
	int sortieBoucle = 0;
	char chaine[50];
	
	FILE* fichier;
	int len = 0;
	
	struct instruct_mem *im = calloc(sizeof(struct instruct_mem),1);
	struct data_mem *dm = calloc(sizeof(struct data_mem),1);
		
	if(argc >= 0){
		fichier = fopen(args[1],"r");
		len = read_file(fichier, im, dm);	// len stores the largest possible value of pc.
		fclose(fichier);
		execute(im,len,dm);
	}
	else{
		do{
			fichier = fopen("./mode_interactif.txt","w+");
			printf("entrez une instruction :\n");
			scanf("%s", chaine);
			if(strcmp(chaine,"EXIT")==0){
				sortieBoucle = 1;
			}
			else{
				fprintf(fichier, "s", chaine);
				read_file(fichier, im, dm);
			}
			fclose(fichier);
			execute(im,1,dm);
		}while(sortieBoucle ==0);
	}
	
	
	return 0;
}
