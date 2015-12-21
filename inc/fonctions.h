#ifndef __FONCTIONS_H__
#define __FONCTIONS_H__

#define NB_INSTR_MAX 50
#define TAILLE_MAX_INSTR 30
#include <stdint.h>

typedef struct {
   int opcode;
   int32_t dec_opcode;
   int nbOperandes;
   int32_t dec_rd;
   int32_t dec_rs;
   int32_t dec_rt;
   int32_t dec_sa;
}TypeR;



int lireDonnees(char nomFichier[], char instructions[][TAILLE_MAX_INSTR]);
void afficherTableau(char T[][TAILLE_MAX_INSTR], int nb);
void upper_string(char s[]);
int instructionToHexa(char instruction[]);

#endif