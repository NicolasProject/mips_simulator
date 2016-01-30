#ifndef REGISTERS_H_INCLUDED
#define REGISTERS_H_INCLUDED


#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define REG_NAME_SIZE 5

// definition of a register
struct reg
{
	char alt_name[REG_NAME_SIZE];			// Register name
	int val;								// Register value
};

// definition in registers.c
struct reg reg_file[32];


void init_reg_file();	   		// Initialize registers, must be called at the begining of the programm
int reg_num(char*alt_name);		// Returns the number of the register
void afficher_registres();		// Display content and name of register


#endif
