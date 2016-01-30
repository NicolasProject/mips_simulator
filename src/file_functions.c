#include "file_functions.h"

int first_char(char*line)
{
	// Function to find the position of the first character (other than space,tab and enter)
	// and return the position
	// Return values (ret)
	// 0 (or greater) => position of the first character in the string
	// -1 		  => '\n' is the only character
	
	int i;
 	for(i=0;(line[i]==32 || line[i]==9) && line[i]!='\x0';i++);	// '\t'=9, 32 => <space>
	
	if(line[i]==10)
		return -1;
	else
	 	return i;
	 	
	// The return statement could have also been framed as 
	// return (line[i]=='\n')?-1:i;	
		
}

int read_file(FILE*file,struct instruct_mem*im,struct data_mem*dm)
{	
	// Function to read the file and modifies the input instruction memory (struct instruction_mem)
	// This function uses 'encode_to_opcode' function from the instruction_mem library.
	// This function also allocates space for the variables declared in the .data segment of the memory
	
	char line[100] = "";
	char temp[100] = "";
	int instr_encodee[4];
	int i, k, j;
	i = 0;
	k = 0;
	j = 0;
	
	
	while(fgets(line,99,file) != NULL)
	{	
		i=first_char(line);				// On récupère l'indice du premier caractère

		//printf("%s",line);
		
		if(i==-1 || line[i]=='#'){		// SI i=-1 ou line[i]='#' (commentaire), on saute la ligne
			continue;
		}
		
		for(j=0;line[i]!='#' && line[i]!='\x0' && line[i]!='\n'; i++,j++)	// On parcoure la ligne jusqu'à la fin
		{	
			temp[j]=line[i];	// On récupère chaque caractère dans temp
		}
		temp[j]='\x0';			// On ajoute le caractère de fin de chaine
		
		encode(temp,instr_encodee,dm,k);	// On encode l'instruction contenue dans temp
											// L'instruction encodée sera contenue dans instr_encodee
											// instr_encodee[0] identifie la fonction (mnemonique)
		
		//printf("%d\n",instr_encodee[0]);
		
		if(instr_encodee[0])	// si la ligne est une instruction (Mnemonique != 0)
		{	
			//printf("\n%d %d %d %d",instr_encodee[0],instr_encodee[1],instr_encodee[2],instr_encodee[3]);
			
			load_instruct_mem(im,k,instr_encodee,temp);		// the instruction is loaded into memory instruction
			k++;											// On incrémente le nombre d'instructions
			
			//printf("\n%d %d %d %d ",im->mem[k].cod[0],im->mem[k].cod[1],im->mem[k].cod[2],im->mem[k].cod[3]);
			//printf("k=%d\n",j);
		}
		
		//printf("k=%d\n",k);
		
		
	}
	
	// lorsque le fichier est entièrement lu et que les instructions sont stockées en mémoire
	// on peut générer le code hexadecimal (il est stocké en mémoire avec les instructions)
	
	for(i = 0; i < k; i++)
	{
		// store encoded instruction in memory
		convDecToHex( instrCode( im->mem[i].cod ), im->mem[i].hexaStr, HEXA_INST_SIZE );
	}
	
	return k;
	
}