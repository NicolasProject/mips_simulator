#ifndef __FONCTIONS_H__
#define __FONCTIONS_H__

#define NB_INSTR_MAX 50
#define TAILLE_MAX_INSTR 30
#include <stdint.h>

typedef enum
{
	ADD, ADDI
} Mnemonic;

typedef enum
{
	typeR, typeI, typeJ
} InstType;


typedef struct {
   int opcode;
   int32_t dec_opcode;
   int nbOperandes;
   int32_t dec_rd;
   int32_t dec_rs;
   int32_t dec_rt;
   int32_t dec_sa;
}TypeR;



#define NBR_CHAR_OPERAND 10

// ******************************************************************
// Definition des structures des operandes des 3 types d'instructions

 typedef struct
{
	//const int nbOp;
	//char *orderOp[nbOp];

	char sa[NBR_CHAR_OPERAND];
	char rd[NBR_CHAR_OPERAND];
	char rt[NBR_CHAR_OPERAND];
	char rs[NBR_CHAR_OPERAND];
} OperandesTypeR;

typedef struct
{
	char immediate[NBR_CHAR_OPERAND];
	char rt[NBR_CHAR_OPERAND];
	char rs[NBR_CHAR_OPERAND];
} OperandesTypeI;

typedef struct
{
	unsigned char target[SIZE_LABEL_NAME];
} OperandesTypeJ;

// ******************************************************************


typedef struct
{
	Mnemonic mnemonic;
	InstType instructionType;
};


// Taille max du nom des labels
//const int SIZE_LABEL_NAME = 20;
#define SIZE_LABEL_NAME 20

// Taille max du nombre de labels
//const int NBR_LABEL = 50;
#define NBR_LABEL 50

typedef struct
{
	char name[SIZE_LABEL_NAME];
	int instNum;
}Label;

typedef struct
{
	Label label[NBR_LABEL];
}Labels;



int lireDonnees(char nomFichier[], char instructions[][TAILLE_MAX_INSTR]);
void afficherTableau(char T[][TAILLE_MAX_INSTR], int nb);
void upper_string(char s[]);
int instructionToHexa(char instruction[]);

#endif