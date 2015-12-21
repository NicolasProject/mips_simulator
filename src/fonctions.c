#include <stdio.h> 
#include <stdlib.h>  
#include "fonctions.h"
#include <string.h>
#include <stdint.h>


int lireDonnees(char nomFichier[], char instructions[][TAILLE_MAX_INSTR]) {  
	
	FILE * fic;  
	int ligne=0;

	/* Ouverture du fichier */  
	fic = fopen(nomFichier, "r");  
	if(fic == NULL) {   
		perror("Probleme ouverture fichier ");   
		exit(1);  
	}  
	 
	/* Lecture dans le fichier */  
	while(!feof(fic)) {   
		fgets(instructions[ligne], 100, fic);
		upper_string(instructions[ligne]);
		ligne++; 
	}  
	
	 /* Fermeture du fichier */  
	fclose(fic);  
	return (ligne-1); 
} 


void afficherTableau(char T[][TAILLE_MAX_INSTR], int nb){
	int i;

	for(i=0; i<=nb; i++){
		printf("%s",T[i]);
	}
	printf("\n");
	

}

void upper_string(char s[]) {
   int c = 0;
 
   while (s[c] != '\0') {
      if (s[c] >= 'a' && s[c] <= 'z') {
         s[c] = s[c] - 32;
      }
      c++;
   }
}


int instructionToHexa(char instruction[]){
	
	char instr_decoupe[4][15];
	int i=0;
	int32_t code_binaire =0;
	
	/* obtenir le premier mot de l'instruction */
   char *token = strtok(instruction, " ,");
   
   /* parcourir les autres mots */
   while( token != NULL ) 
   {
      strcpy(instr_decoupe[i],token);
    	printf("mot n°%d : %s\n",i,instr_decoupe[i]);
      token = strtok(NULL, " ,");
      i++;
   }
   
  	if(instr_decoupe[0] =="ADD" || instr_decoupe[0] =="AND" || instr_decoupe[0] =="DIV" || instr_decoupe[0] =="MFHI" || instr_decoupe[0] =="MFLO" || instr_decoupe[0] =="MULT" || instr_decoupe[0] =="OR" || instr_decoupe[0] =="SLT" || instr_decoupe[0] =="SUB" || instr_decoupe[0] =="XOR"){
  		
  		TypeR monInstruction;
  		monInstruction.dec_rd = 11;
   	monInstruction.dec_rs = 21;
   	monInstruction.dec_rt = 16;
		monInstruction.dec_sa = 6;
  		
  		if(instr_decoupe[0] == "ADD"){
  			monInstruction.opcode = 0b100000;
  		}
  		else if(instr_decoupe[0] == "AND"){
  			monInstruction.opcode = 0b100100;
  		}
  		else if(instr_decoupe[0] == "DIV"){
  			monInstruction.opcode = 0b011010;
  		}
  		else if(instr_decoupe[0] == "MFHI"){
  			monInstruction.opcode = 0b010000;
  		}
  		else if(instr_decoupe[0] == "MFLO"){
  			monInstruction.opcode = 0b010010;
  		}
  		else if(instr_decoupe[0] == "MULT"){
  			monInstruction.opcode = 0b011000;
  		}
  		else if(instr_decoupe[0] == "OR"){
  			monInstruction.opcode = 0b100101;
  		}
  		else if(instr_decoupe[0] == "SLT"){
  			monInstruction.opcode = 0b101010;
  		}
  		else if(instr_decoupe[0] == "SUB"){
  			monInstruction.opcode = 0b100010;
  		}
  		else if(instr_decoupe[0] == "XOR"){
  			monInstruction.opcode = 0b100110;
  		}
  		
  		
  		
  	}
   
   
}





