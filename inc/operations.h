#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED


#include "registers.h"
#include "pc.h"
#include "data_mem.h"
#include "instruction_mem.h"

// Functions to execute the instructions (modify memory and registers)


void add (unsigned int dest,unsigned int reg1,unsigned int reg2);		// Stores the sum of values in reg1 and reg2 in dest
void sub (unsigned int dest,unsigned int reg1,unsigned int reg2);
void and (unsigned int dest,unsigned int reg1,unsigned int reg2);
void slt (unsigned int dest,unsigned int reg1,unsigned int reg2);      // Set on Less Than (reg1 < reg2)
void or  (unsigned int dest,unsigned int reg1,unsigned int reg2);
void xor (unsigned int dest,unsigned int reg1,unsigned int reg2);

void rotr(unsigned int dest,unsigned int reg1,int shift);
void sll (unsigned int dest,unsigned int reg1,int shift);
void srl (unsigned int dest,unsigned int reg1,int shift);

void addi(unsigned int dest,unsigned int reg1,int val);

// Load Immediate
void lui(unsigned int dest,int val);

// Load/Store instruction
void load_word(unsigned int dest,unsigned int addr,struct data_mem*dm);
void store_word(unsigned int dest,unsigned int addr,struct data_mem*dm);

// Branch & Jump instructions
void beq (unsigned int reg1,unsigned int reg2,int offset);
void bne (unsigned int reg1,unsigned int reg2,int offset);
void blez(unsigned int reg1,int offset);
void bgtz(unsigned int reg1,int offset);
void jump(unsigned int labelIdx);
void jal (unsigned int labelIdx);
void jr  (unsigned int reg);

// Syscall
void syscall();


 #endif		//End of file
