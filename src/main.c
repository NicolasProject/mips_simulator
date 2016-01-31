#include <stdio.h>
#include <stdlib.h>
#include "file_functions.h"
#include "instruction_mem.h"
#include "registers.h"
#include "menu.h"


int main(int argc,char*args[])
{
	init_reg_file();		// Initialisation du fichier des registres
	int sortieBoucle = 0;	// Variable de sortie de boucle
	char chaine[99]= "";	// Contiendra l'instruction en cours en mode interactif
	
	FILE* fichier =NULL;	// Contiendra le descripteur de fichier
	int len = 0;			// Contiendra le nombre d'instructions
	
	struct instruct_mem *im = calloc(sizeof(struct instruct_mem),1);	// On affecte la mémoire nécessaire à la mémoire d'instruction
	struct data_mem *dm = calloc(sizeof(struct data_mem),1);			// On affecte la mémoire nécessaire à la mémoire de données
		
	if(argc == 2){		// 2 arguments donc mode fichier (on passe le nom du fichier)
	
		fichier = fopen(args[1],"r");			// On ouvre le fichier passé en argument
		
		if(fichier != NULL){					// Si l'ouverture s'est bien passée
			
			len = read_file(fichier, im);		// On lit le fichier, stocke les instructions dans la mémoire d'instruction et encode chaque instruction
												// len contient le nombre d'instructions
			fclose(fichier);					// On ferme le fichier 
			execute(im,len-1,dm,0);				// On execute les instructions présentes dans la mémoire d'instructions
												// jusqu'à ce que PC = len-1 
												// le dernier paramètre vaut ici 0 ce qui veut dire que l'on est pas en mode pas à pas
		}
		
		else									// Si l'ouverture s'est mal passée
		{									
			printf("problème lors de l'ouverture du fichier");	
		}
	}
	
	
	else if((argc == 3) && !strcmp(args[2],"-pas")){	// Si on a 3 arguments et que le 3eme est "-pas" : mode fichier pas a pas
		
		fichier = fopen(args[1],"r");		// On ouvre le fichier en premier argument
		
		if(fichier != NULL){				// Si l'ouverture s'est bien passée
			
			len = read_file(fichier, im);	// On lit le fichier, stocke les instructions dans la mémoire d'instruction et encode chaque instruction
											// len contient le nombre d'instructions
			fclose(fichier);				// On ferme le fichier
			execute(im,len-1,dm,1);			// On execute les instructions présentes dans la mémoire d'instructions
											// jusqu'à ce que PC = len-1 
											// le dernier paramètre vaut ici 1 ce qui veut dire que l'on est en mode pas à pas
		}
		
		else								// Si l'ouverture s'est mal passée
		{
			printf("problème lors de l'ouverture du fichier");
		}
	}
	
	
	else if((argc == 3) && !strcmp(args[2],"-hexa")){	// Si on a 3 arguments et que le 3eme est "-hexa" : mode conversion fichier en hexa
	
		fichier = fopen(args[1],"r");		// On ouvre le fichier en premier argument
		
		if(fichier != NULL){				// Si l'ouverture s'est bien passée
			
			len = read_file(fichier, im);			// On lit le fichier, stocke les instructions dans la mémoire d'instruction et encode chaque instruction
													// len contient le nombre d'instructions
			fclose(fichier);						// On ferme le fichier
			execute(im,len-1,dm,2);					// On execute les instructions présentes dans la mémoire d'instructions
													// jusqu'à ce que PC = len-1 
													// le dernier paramètre vaut ici 2 ce qui veut dire que l'on est en mode conversion hexa
		}
		
		else								// Si l'ouverture s'est mal passée
		{
			printf("problème lors de l'ouverture du fichier");
		}
	}
	
	
	else if(argc == 1){		// S'il n'y a qu'un argument : mode interactif
	
		printf("\nMode interactif: à tout moment vous pouvez entrer la commande 'print_reg' suivie du numero de registre pour afficher sa \n");
		printf("valeur, ou la commande 'print_mem' suivie d'une adresse pour afficher le contenu de la mémoire à cette adresse\n\n");
		
		do{
				printf("entrez une instruction :\n");		// L'utilisateur doit entrer une instruction
				fgets(chaine, sizeof(chaine), stdin);		// On la récupère dans la chaine de caractère "chaine"
				
				if((strcasecmp(chaine,"EXIT\n"))==0 ||(strncmp(chaine,"print_reg",9))==0 ||(strncmp(chaine,"print_mem",9))==0){
							// Si l'instruction entrée est EXIT ou qu'elle contient dans ses 9 premiers caractères : print_reg ou print_mem
							
					do{
						sortieBoucle = menu(chaine, dm);	// On appelle la fonction menu à laquelle on passe l'instruction et la mémoire de données
															// Cette fonction renvoie 1 si l'utilisateur a entré EXIT
															//						  2 si l'utilsateur a entré une commande inconnue
															// 						  0 sinon
															// Si la commande entrée est "print_reg x", la fonction affiche le contenu du registre x
															// Si la commande entrée est "print_mem x", la fonction affiche le contenu à l'adresse x
															
					}while(sortieBoucle == 2);		// Tant que l'utilisateur entre une commande inconnue
				}
				
				else		// Si l'instruction entrée ne fait pas partie de celles ci dessus il s'agit normalement d'une instruction MIPS
				{
					fichier = fopen(TMP_FILENAME,"w");		// On ouvre alors le fichier TMP_FILENAME en écriture
				
					if(fichier != NULL){								// Si son ouverture se passe correctement		
						
						fprintf(fichier, "%s", chaine);						// On écrit dans le fichier l'instruction entrée par l'utilisateur
						fclose(fichier);									// On ferme ensuite le fichier
						
						fichier = fopen(TMP_FILENAME,"r");		// On ouvre à nouveau le fichier mais en lecture
						
						if(fichier != NULL){								// Si l'ouverture se passe correctement
							read_file(fichier, im);							// On lit le fichier et stocke l'instruction dans im
							fclose(fichier);								// Puis on referme le fichier
							execute(im,0,dm,0);								// Et on execute l'instruction
																			// Le deuxieme paramètre vaut 0 (valeur max de PC)
																			// Le dernier paramètre vaut 0 (mode non pas à pas)
						}	
						else												// Si l'ouverture en lecture se passe mal
						{													
							printf("problème lors de l'ouverture du fichier en lecture");
						}
					}
					
					else	// Si l'ouverture en écriture se passe mal
					{
						printf("problème lors de l'ouverture du fichier en écriture");
					}
				}
			
		}while(sortieBoucle ==0);	// Tant que sortieBoucle =0 on refais la boucle. 
									// SortieBoucle vaut 1 quand l'utilisateur entre EXIT
		
		remove(TMP_FILENAME);
	}
	
	return 0;
	
}	// Fin fichier
