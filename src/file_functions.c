#include "file_functions.h"

int first_char(char*line)
{
	// Return position of the first character
	// Return -1 if there is no character
	
	int i;
 	for(i=0;(line[i]==32 || line[i]==9) && line[i]!='\x0';i++);	// '\t'=9, 32 => <space>
	
	if(line[i]==10 || line[i]=='\x0')
		return -1;
	else
	 	return i;
}

int read_file(FILE*file,struct instruct_mem*im)
{
	char line[SIZE_INSTR_STR] = "";
	char temp[SIZE_INSTR_STR] = "";
	int instr_encodee[4] = {0}; // initialize array to 0
	int i = 0, k = 0, j = 0;
	
	// lit ligne par ligne le fichier
	while(fgets(line,SIZE_INSTR_STR - 1,file) != NULL)
	{	
		i=first_char(line);				// On récupère l'indice du premier caractère

		//printf("line read : %s\n",line);
		
		if(i==-1 || line[i]=='#'){		// SI i=-1 ou line[i]='#' (commentaire), on saute la ligne
			continue;
		}
		
		for(j=0;line[i]!='#' && line[i]!='\x0' && line[i]!='\n'; i++,j++)	// On parcoure la ligne jusqu'à la fin
		{	
			temp[j]=line[i];	// On récupère chaque caractère dans temp
		}
		temp[j]='\x0';			// On ajoute le caractère de fin de chaine
		
		encode(temp,instr_encodee,k);	// On encode l'instruction contenue dans temp
										// L'instruction encodée sera contenue dans instr_encodee
										// instr_encodee[0] identifie la fonction (mnemonique)
		
		//printf("mnemonique enum : %d\n",instr_encodee[0]);
		
		if(instr_encodee[0])	// si la ligne est une instruction (Mnemonique != 0)
		{	
			//printf("instr_encodee(0, 1, 2, 3) : %d %d %d %d\n",instr_encodee[0],instr_encodee[1],instr_encodee[2],instr_encodee[3]);
			
			load_instruct_mem(im,k,instr_encodee,temp);		// the instruction is loaded into memory instruction
			k++;											// On incrémente le nombre d'instructions
			
			// verif fonctionnement memoire
			/*printf("instruction en memoire (%d) : %d %d %d %d\n",im->mem[k].cod[0],im->mem[k].cod[1],
																	im->mem[k].cod[2],im->mem[k].cod[3]);
			*/
		}
	}
	
	// lorsque le fichier est entièrement lu et que les instructions sont stockées en mémoire
	// on peut générer le code hexadecimal (il est stocké en mémoire avec les instructions)
	
	for(i = 0; i < k; i++)
	{
		// store encoded instruction in memory
		// instrCode() retourne l'instruction encodee selon le codage des instructions du mips
		// convDecToHex() traduit l'instruction en hexadecimal (caracteres)
		convDecToHex( instrCode( im->mem[i].cod ), im->mem[i].hexaStr, HEXA_INST_STR_SIZE );
	}
	
	return k;
}
