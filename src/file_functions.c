/* Contains the definitions of the functions used to read the MIPS code from a file */

// TODO : Introduce string


#include "file_functions.h"
#include <string.h>

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
	int i,k,j,line_num,val,dm_pos;
	line_num=i=k=j=dm_pos=0;
	
	while(!feof(file))
	{	
		line_num++;
		fgets(line,99,file);
		i=first_char(line);
		
		//printf("%s",line);
		
		// SI i=-1 ou line[i]='#' (commentaire), on saute la ligne
		if(i==-1 || line[i]=='#'){
			continue;
		}
		
	
		for(j=0;line[i]!='#' && line[i]!='\x0' && line[i]!='\n'; i++,j++){
			temp[j]=line[i];
		}
		temp[j]='\x0';
		
		
		int coded[4];
		
		encode(temp,coded,dm,k);
		//printf("%d\n",coded[0]);
		
		if(coded[0])
		{	
			//printf("Loaded : %s",temp);
			//printf("\n%d %d %d %d",coded[0],coded[1],coded[2],coded[3]);
			load_instruct_mem(im,k,coded);
			k++;
			//printf("\n%d %d %d %d ",im->mem[k].cod[0],im->mem[k].cod[1],im->mem[k].cod[2],im->mem[k].cod[3]);
			//printf("k=%d\n",j);
		}
		
		//printf("k=%d\n",k);
				
		
		
	}
	return k;
}


