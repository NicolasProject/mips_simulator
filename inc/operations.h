#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED


#include "registers.h"
#include "pc.h"
#include "data_mem.h"
#include "instruction_mem.h"

// Functions to execute the instructions (modify memory and registers)


void add (int dest,int reg1,int reg2);		// Stores the sum of values in reg1 and reg2 in dest
void sub (int dest,int reg1,int reg2);
void and (int dest,int reg1,int reg2);
void slt (int dest,int reg1,int reg2);      // Set on Less Than (reg1 < reg2)
void or  (int dest,int reg1,int reg2);
void xor (int dest, int reg1, int reg2);

void rotr(int dest, int reg1, int shift);
void sll(int dest, int reg1, int shift);
void srl(int dest, int reg1, int shift);

void addi(int dest,int reg1,int val);

// Load Immediate
void lui(int dest,int val);

// Load/Store instruction
void load_word(int dest,int addr,struct data_mem*dm);
void store_word(int dest,int addr,struct data_mem*dm);

// Branch & Jump instructions
void beq(int reg1,int reg2,int offset);
void bne(int reg1,int reg2,int offset);
void blez(int reg1, int offset);
void bgtz(int reg1, int offset);
void jump(int labelIdx);
void jal(int labelIdx);
void jr(int reg);


// Syscall
void syscall();

// Move instruction
void move(int dest,int src);
 #endif		//End of file
