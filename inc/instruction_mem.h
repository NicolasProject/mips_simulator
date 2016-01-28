#ifndef INSTRUCTION_MEM_H_INCLUDED
#define INSTRUCTION_MEM_H_INCLUDED


 
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "data_mem.h"
#include "registers.h"
#include "operations.h"
#include "pc.h"

#define SIZE_OPCODE_CHAR 8

#define MOVE 	0b100110	//MOVE  => 6


enum Mnemonique
{
	ADD=1, ADDI, SUB, MULT, DIV, AND, OR, XOR, 
	ROTR, SLL, SRL, SLT, 
	BGTZ, BLEZ, BEQ, BNE, 
	J, JAL, JR, LW, SW, MFHI, MFLO, LUI, SYSCALL
};



// instruction memory size
#define INST_MEM_SIZE 1024

// size of hexa instruction
#define HEXA_INST_SIZE 9


struct instruct_mem_element
{
	int cod[4];
	char c[99];
	char hexaStr[HEXA_INST_SIZE];
};


struct instruct_mem
{
	struct instruct_mem_element mem[INST_MEM_SIZE];
	
};

struct label_table
{
	struct label_element{
		char name[20];
		int inst_num;	
	}label[100];
}labels;
int label_num;


// split the input string (instruction) into opcode and operands
void encode(char *input, int *instr_encodee, struct data_mem *dm, int num);
// decode the instruction and execute it
void decode(int *encoded_inst, struct data_mem *dm);

// get value from string, take and return the index in the string
int getValueStr(char *str, int *idx);

// load 1 instruction into memory instruction
void load_instruct_mem(struct instruct_mem *im, int mem_pos, int *instruct, char* instruction);
// execute the instructions from memory instruction (execute the program)
void execute(struct instruct_mem *im, int fin, struct data_mem *dm, int modePas_A_Pas);

// get index of name label and reserve space if not already exits
int label_pos(char *name);	

// conversion instruction (instr_encodee) to hexa
uint32_t instrCode(int *instr_encodee);
// Converti une valeur decimal en hexadecimal
void convDecToHex(int decimal, char *hexa, int size);
/**
 * retourne la taille necessaire d'une chaine de caractere
 * pour contenir la valeur en hexadecimal de 'decimal'
*/
int getSizeHexaStrFromDec(int decimal);

 #endif 
