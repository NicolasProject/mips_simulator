/* File that declares the struct data_mem, and its functions */

#ifndef DATA_MEMORY_H_INCLUDED
#define DATA_MEMORY_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// data memory size
#define DATA_MEM_SIZE 1024

struct data_mem
{
	struct data_mem_element
	{
		char var_name[20];
		int val;
	
	}mem[DATA_MEM_SIZE];
};


void store(int pos,struct data_mem *dm,char*var_name,int val);
int get_mem_location(char*var_name,struct data_mem *dm);			// Returns the index where var_name is stored
		

#endif		//End of file

