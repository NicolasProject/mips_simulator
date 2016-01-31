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

#define SIZE_INSTR_STR 		100
#define SIZE_OPCODE_CHAR 	8


// Available mnemonic enumeration
enum Mnemonique
{
	ADD=1, ADDI, SUB, MULT, DIV, AND, OR, XOR, 
	ROTR, SLL, SRL, SLT, 
	BGTZ, BLEZ, BEQ, BNE, 
	J, JAL, JR, LW, SW, MFHI, MFLO, LUI, SYSCALL
};


// instruction memory size
#define INST_MEM_SIZE 		1024

// number (max) of field in instruction (mnemonic, operand 1, operande 2, ...)
#define FIELD_INST_NBR 		4

// size of hexa instruction
#define HEXA_INST_STR_SIZE 	9

// size of label name
#define LABEL_STR_SIZE 		20


// This structure define an array of instruct_mem_element, it represents the memory instruction.
// It encapsulate the array and we pass to functions a variable of type instruct_mem
// to access to the memory instruction
struct instruct_mem
{
	// This structure define an instruction in memory.
	// It is defined here because we only use it here
	struct instruct_mem_element
	{
		int cod[FIELD_INST_NBR];
		char c[SIZE_INSTR_STR];
		char hexaStr[HEXA_INST_STR_SIZE];
		
	} mem[INST_MEM_SIZE]; // array of instruct_mem_element is created
};

// This structure define an array of label_element,
// it contains all the label used in the assembler program
struct label_table
{
	// This structure define a label (name and the address referencing the instruction)
	struct label_element{
		char name[LABEL_STR_SIZE];
		int inst_num;	
		
	} label[100];
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

// get index of 'name' label and reserve space if not already exits
int label_pos(char *name);	

// instruction conversion (instr_encodee) into binary
// returns the instruction code understandable by the MIPS processor
uint32_t instrCode(int *instr_encodee);

// decimal conversion into hexa
void convDecToHex(int decimal, char *hexa, int size);


/**
 * retourne la taille necessaire d'une chaine de caractere
 * pour contenir la valeur en hexadecimal de 'decimal'
*/
int getSizeHexaStrFromDec(int decimal);


#endif 
