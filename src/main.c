#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonctions.h"


int main(int argc,char *argv[])
{
	int nbrInstructions=0;
	int i, j ;
	char instructions[NB_INSTR_MAX][TAILLE_MAX_INSTR] ;
	
	nbrInstructions = lireDonnees(argv[1],instructions);
	//afficherTableau(instructions, nbrInstructions);
 	instructionToHexa(instructions[0]);
	return 0;
}