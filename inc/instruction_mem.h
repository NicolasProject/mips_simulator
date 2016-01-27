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

// opcodes
#define ADD 	0b100000	//ADD   => 32
#define SUB 	0b100010	//SUB   => 34
#define AND 	0b100100	//AND   => 36
#define SLT 	0b101010	//SLT   => 42
#define OR 		0b100101	//OR    => 37
#define LI 		0b100001	//LI    => 33
#define SYSCALL 0b001100	//SYSCALL=>12
#define LW 		0b100011	//LW    => 35
#define SW 		0b101011	//SW    => 43
#define J 		0b000010	//J     => 2
#define BEQ 	0b000100	//BEQ   => 4		
#define MOVE 	0b000110	//MOVE  => 6


#define ADD		0b100000
#define ADDI		
#define SUB		0b100010
#define MULT		
#define DIV		
#define AND		0b100100
#define OR		0b100101
#define XOR		
#define ROTR		
#define SLL		
#define SRL		
#define SLT		0b101010
#define BGTZ		
#define BLEZ		
#define BEQ		
#define BNE		
#define J		
#define JAL		
#define JR		
#define LW		
#define SW		
#define MFHI		
#define MFLO		
#define LUI		
#define SYSCALL		



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


void encode(char *input, int *instr_encodee, struct data_mem *dm, int num);	 
void decode(int *encoded_inst, struct data_mem *dm);	

void load_instruct_mem(struct instruct_mem *im, int mem_pos, int *instruct, char* instruction); 
void execute(struct instruct_mem *im, int fin, struct data_mem *dm, int modePas_A_Pas);

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
