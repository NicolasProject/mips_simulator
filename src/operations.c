#include "operations.h"
#include <time.h>
#include <errno.h>


void add (unsigned int dest,unsigned int reg1,unsigned int reg2)
{
	// Attention aux valeurs négatives des opérandes, car si reg1 < 32, alors elles sont
	// interprétées comme des numéros de registre.
	// Nous avons pris en compte les valeurs immédiate
	// (normalement impossible pour ce type d'instruction du MIPS)
	
	int32_t a;
	if(reg1<32)
	{
		a = reg_file[reg1].val;
	}
	else
	{
		a = reg1 - 32;
	}
	
	// nous utilisons un ternaire pour raccourcir le code. Oui, celà reste moins lisible...
	int32_t b=(reg2<32)?reg_file[reg2].val:(int32_t)(reg2-32);
	
	reg_file[dest].val=a+b;
	
	pc++;
}

void addi(unsigned int dest,unsigned int reg1,int val)
{	
	//	printf("reg1 : %i\n", reg1);
	//	printf("immediate : %i\n", val);
	//	printf("nom du reg dest : %s\n", reg_file[dest].alt_name);
	
	if (reg1 < 32)
	{
		reg_file[dest].val = reg_file[reg1].val + val - 32;
	}
	else
	{
		printf("Error ADDI !\n");
	}
	
	pc++;
}

 
void sub (unsigned int dest,unsigned int reg1,unsigned int reg2)
{	
	int32_t a=(reg1<32)?reg_file[reg1].val:(int32_t)(reg1-32);
	int32_t b=(reg2<32)?reg_file[reg2].val:(int32_t)(reg2-32);
	
	reg_file[dest].val=a-b;
	pc++;
}

void and (unsigned int dest,unsigned int reg1,unsigned int reg2)
{	
	int32_t a=(reg1<32)?reg_file[reg1].val:(int32_t)(reg1-32);
	int32_t b=(reg2<32)?reg_file[reg2].val:(int32_t)(reg2-32);
	
	reg_file[dest].val=a & b;
	pc++;
}
 
void slt (unsigned int dest,unsigned int reg1,unsigned int reg2)
{	
	int32_t a=(reg1<32)?reg_file[reg1].val:(int32_t)(reg1-32);
	int32_t b=(reg2<32)?reg_file[reg2].val:(int32_t)(reg2-32);
	
	reg_file[dest].val=(a<b)?1:0;
	pc++;
}

void or (unsigned int dest,unsigned int reg1,unsigned int reg2)
{
	int32_t a=(reg1<32)?reg_file[reg1].val:(int32_t)(reg1-32);
	int32_t b=(reg2<32)?reg_file[reg2].val:(int32_t)(reg2-32);
	
	reg_file[dest].val=a | b;
	pc++;
}

void lui(unsigned int dest,int val)
{
	reg_file[dest].val=((uint32_t)val) << 16;
	pc++;
}

void load_word(unsigned int dest,unsigned int addr,struct data_mem*dm)
{
	reg_file[dest].val=(dm->mem[addr].val);
	pc++;
}

void store_word(unsigned int dest,unsigned int addr,struct data_mem*dm)
{
	dm->mem[addr].val=reg_file[dest].val;
	pc++;
}

void jump(unsigned int labelIdx)
{
	pc = labels.label[labelIdx].inst_num;
}

void jal (unsigned int labelIdx)
{
	reg_file[31].val = pc + 1;
	jump(labelIdx);
}

void jr (unsigned int reg)
{
	pc = reg_file[reg].val;
}

void beq (unsigned int reg1,unsigned int reg2,int offset)
{
	//printf("beq instruction offset: %i\n", offset);
	
	if(reg_file[reg1].val==reg_file[reg2].val)
		pc= pc + offset;
	else
		pc++;
}

void syscall()
{
	// Différentes opération en fonction de la valeur du registre $v0
	// Quelques fonctions sont implémentées
	
	switch(reg_file[2].val)
	{
		case 1:
		
			//printf("Valeur du registre $4 ($a0) : ");
			printf("%d\n",reg_file[4].val);
			
			break;
			
		case 5:
		
			// Read integer
			scanf("%d",&reg_file[2].val);
			
			break;
			
		case 10:
			
			//printf("Exiting program\n");
			exit(EXIT_SUCCESS);
			
			break;
			
		case 32:
		{
			int result, repeatSleep = 100;
			struct timespec remaining;
			remaining.tv_sec = reg_file[4].val / 1000;
			remaining.tv_nsec = (reg_file[4].val % 1000) * 1000000L; // long
			
			do
			{
				struct timespec ts_sleep = remaining;
				//printf("sleep...\n");
				result = nanosleep(&ts_sleep, &remaining);
				repeatSleep--;
				
			}while(result == -1 && repeatSleep > 0);
			//printf("repeatSleep : %d", 100 - repeatSleep);
			
			if(result)
			{
				printf("nanosleep function error : %s\n", strerror(errno));
			}
		
			break;
		}
		
		case 34:
		{
			// write hexadecimal of reg4 on 8 digits
			/*
			char str[9];
			convDecToHex(reg_file[4].val, str, 9);
			printf("0x%s\n", str);
			*/
			printf("0x%08x\n", reg_file[4].val);
		
			break;
		}
		
		case 35:
		{
			// write binary of reg4 on 32 digits
			char str[33];
			convDecToBase(reg_file[4].val, str, 33, 2);
			printf("0b%s\n", str);
		
			break;
		}
		
		case 36:
		
			// print unsigned integer
			printf("%d\n", reg_file[4].val);
		
			break;
			
			
		default:
			printf("Unknown syscall : %d\n", reg_file[2].val);
	}
	
	pc++;
}


void rotr(unsigned int dest,unsigned int reg1,int shift) {
	
	uint32_t temp;
	//printf("avant rotate right : %i",(reg_file[reg1].val));
    temp = (((uint32_t)reg_file[reg1].val >> shift) | ((uint32_t)reg_file[reg1].val) << (sizeof((uint32_t)reg_file[reg1].val)*8 - shift));
    //printf("après rotate right : %i",temp);
    reg_file[dest].val = temp;
    
   	pc++;
}

void sll (unsigned int dest,unsigned int reg1,int shift) {
	
	uint32_t temp;
	//printf("avant shift left : %i",(reg_file[reg1].val));
    temp = ((uint32_t)reg_file[reg1].val << shift);
    //printf("après left shift : %i",temp);
    reg_file[dest].val = temp;
    
   	pc++;
}

void srl (unsigned int dest,unsigned int reg1,int shift) {
	
	uint32_t temp;
	//printf("avant shift right : %i",(reg_file[reg1].val));
    temp = ((uint32_t)reg_file[reg1].val >> shift);
    //printf("après shift right : %i",temp);
    reg_file[dest].val = temp;
    
   	pc++;
}

void xor (unsigned int dest,unsigned int reg1,unsigned int reg2) {

    reg_file[dest].val = (reg_file[reg1].val ^ reg_file[reg2].val);
   	pc++;
}

void bgtz(unsigned int reg1,int offset){
	
	if(reg_file[reg1].val > 0){
		pc = pc + offset;
	}
	else{
		pc++;
	}
	
}

void blez(unsigned int reg1,int offset){
	
	if(reg_file[reg1].val <= 0){
		pc = pc + offset;
	}
	else{
		pc++;
	}
	
}

void bne (unsigned int reg1,unsigned int reg2,int offset)
{

	if(reg_file[reg1].val!=reg_file[reg2].val)
		pc= pc + offset;
	else
		pc++;
		
	
	return;
}
