#ifndef FILE_FUNC_INCLUDED
#define FILE_FUNC_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>

// Including libraries declaring the instruction memory and data memory structures
#include "instruction_mem.h"
#include "data_mem.h"

int read_file(FILE *file,struct instruct_mem *im,struct data_mem *dm);   //Reads the file and includes the necessary instructions into the Instruction Memory
					   //in the encoded form.

// creer le fichier hexa
FILE* createHexaFile(char filename[], struct instruct_mem *im, int instNumber);
// ferme le fichier hexa
void closeHexaFile(FILE* file);

#endif 		//End of file
