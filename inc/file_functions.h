#ifndef FILE_FUNC_INCLUDED
#define FILE_FUNC_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>

// Including libraries declaring the instruction memory and data memory structures
#include "instruction_mem.h"
#include "data_mem.h"

// This function read line by line the file passed in argument and load
// the instruction into memory instruction in the encoded form.

int read_file(FILE *file,struct instruct_mem *im,struct data_mem *dm);

#endif 		//End of file
