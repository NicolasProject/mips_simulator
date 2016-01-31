
// Encodage et décodage des instructions. 

#include "instruction_mem.h"
#include "menu.h"


//Definition des opcodes 
static const char funct[][SIZE_OPCODE_CHAR] = {
	"add", "addi", "sub", "mult", "div", "and", "or", "xor", 
	"rotr", "sll", "srl", "slt", 
	"bgtz", "blez", "beq", "bne", 
	"j", "jal", "jr", "lw", "sw", "mfhi", "mflo", "lui", "syscall"
};
// Tableau contenant les opcodes
static const int opcode[] = {
	ADD, ADDI, SUB, MULT, DIV, AND, OR, XOR, 
	ROTR, SLL, SRL, SLT, 
	BGTZ, BLEZ, BEQ, BNE, 
	J, JAL, JR, LW, SW, MFHI, MFLO, LUI, SYSCALL
};

// must be in the same order as above and as the enum Mnemonique
// used for hexadecimal translation -> opcodeVal[ADD-1]
static const int opcodeVal[] = {
	0b100000, 0b001000, 0b100010, 0b011000, 0b011010, 0b100100, 0b100101, 0b100110, 
	0b000010, 0b000000, 0b000010, 0b101010, 
	0b000111, 0b000110, 0b000100, 0b000101, 
	0b000010, 0b000011, 0b001000, 0b100011, 0b101011, 0b010000, 0b010010, 0b001111, 0b001100
};


void encode(char*input,int*instr_encodee,struct data_mem*dm,int num)
{
	char inst[SIZE_OPCODE_CHAR];		// Contient l'instruction (comme add,sub,move,b)
	int i=0,j,k;
	char*ptr;
	int memLocationIdx;
	
	// **************************************************************************
	// Traitement particulier pour les labels
			
	if( (ptr = strchr(input,':')) != 0 )
	{
		char lbl[LABEL_STR_SIZE];
		// Pour les labels, on met instr_encodee[0]=0 (this way it is not loaded into the instruction memory if this value doesn't change)
		instr_encodee[0]=0;
		int already_exists=0;			// Variable indiquant si le label est présent en mémoire
					
		for(; input[i]!=*ptr && input[i]!=32; i++){		//On recupere le texte du label dans lbl
			lbl[i]=input[i];
		}
		
		lbl[i]='\x0';		//Ajout du caractère de fin de chaine
		
		
		for(j=0; j<label_num; j++)	// On parcoure les labels existants (label_num contient le nombre de label)
		{
			if(!strcmp(labels.label[j].name,lbl))	// si le label lbl est dejà dans le tableau des labels à la case j
			{
				if(labels.label[j].inst_num==-1)	// mais qu'il ne référence pas une instruction : inst_num = -1
				{
					already_exists=1;				// alors maintenant il existe
					labels.label[j].inst_num=num;	// On affecte le numéro de l'instruction dans laquelle il se trouve
					return;
				}
				else	// Sinon, il a été utilisé plus d'une fois
				{
					printf("Label %s a été utilisé plus d'une fois \n",lbl);
					exit(4);		// Label re-use
				}
			}
		}
		
		if(!already_exists)		// Si le label n'existe pas
		{
			strcpy(labels.label[label_num].name,lbl);	// On le met dans le tableau des labels
			labels.label[label_num].inst_num=num;		// On affecte le numéro de l'instruction correspondante
			label_num++;								// On incrémente le nombre de label
			
			
			for(; input[i]==32 || input[i]==':' || input[i]==9; i++);
			
			if(input[i]=='\n' || input[i]=='\x0' || input[i]=='#'){ 	// On arrete on sort de la fonction encode et on va lire l'instruction suivante
				return;
			}
			
		}		
	}		
	
	// **************************************************************************
	// Traitement de l'instruction si elle ne contient pas de label
	
	// On copie le premier mot dans inst
	for(j=0; input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=9; i++,j++)
	{
		inst[j]=input[i];
	}
	inst[j]='\x0';			// On ajoute le caractère de fin de chaine
	
	const int nbOpcode = sizeof(funct) / (sizeof(char) * SIZE_OPCODE_CHAR); // number of available opcode
	//printf("nombre opcodes = %i", nbOpcode);
	
	for(j=0; j<=nbOpcode; j++)				// On balaye les opcodes
	{
		if(!strcasecmp(inst,funct[j])){		// Si l'opcode à l'indice j est celui que l'on a récuperé
			break;							// On sort de la boucle en gardant la valeur de l'indice j
		}
	}
	
	if(j <= nbOpcode){					// Si l'opcode récuperé est parmi ceux du tableau funct
		instr_encodee[0]=opcode[j];		// On affecte l'opcode à la chaine encodée à l'indice 0 (l'opcode est une énumération dans instruction_mem.h)
	}
	
	else 											// Si l'opcode recupéré n'est pas dans le tableau funct
	{
		printf("Instruction inconnue : %s\n",inst);	// Alors il s'agit d'une instruction inconnue
		exit(3);
	}
	
	// **************************************************************************
	// Gestion des instructions de type R (ADD/SUB/AND/OR/SLT) + ADDI
	
	if(instr_encodee[0]==ADD||instr_encodee[0]==SUB||instr_encodee[0]==AND||instr_encodee[0]==OR||instr_encodee[0]==SLT||instr_encodee[0]==ADDI||
		instr_encodee[0]==ROTR || instr_encodee[0]==SLL || instr_encodee[0]==SRL || instr_encodee[0]==XOR || instr_encodee[0]==JR)
	{	
		// On vérifie si l'instruction est de type R
		
		char reg[REG_NAME_SIZE];  // chaine qui contiendra un nom de registre
	
		for(k=1; k<=3; k++){
			
			for(; input[i]==',' || input[i]==32 || input[i]=='$'; i++);		// On se rend au mot suivant
			
			// Il peut être une valeur immediate ou un registre (si il est précedé de $ c'est un registre)
			if(input[i-1]!='$')		// Valeur immédiate
			{
				// Pour différencier valeur immédiate et registre on utilisera les nombres à partir de 32 pour les valeurs immédiate
				// Les registres allant de 0 à 31
				instr_encodee[k] = 32 + getValueStr(input, &i);		// On ajoute alors la valeur à l'instruction encodée à l'indice 1
			}
			else	// Registre
			{
				for(j=0; input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && j<4 && input[i]!='#' && input[i]!=9; i++,j++)
				{	
					reg[j]=input[i];	//On récupère le nom du registre
				
				}	
				reg[j]='\0';	// On ajoute le caractère de fin de chaine
				instr_encodee[k]=reg_num(reg);	// On recupere son numéro grâce à la fonction reg_num et on l'ajoute à l'instruction encodée
			}
		}
		
	}
	
	// **************************************************************************
	// Traitement type I
	
	else if(instr_encodee[0]==LUI || instr_encodee[0]==BLEZ || instr_encodee[0]==BGTZ)
	{
		char reg[REG_NAME_SIZE];
		

		for(;input[i]==',' || input[i]==32 || input[i]=='$';i++);	// On va jusqu'au 1er registre
		
		for(j=0;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && j<2 && input[i]!='#' && input[i]!=9;i++,j++)
		{	
			reg[j]=input[i];
		
		}	
		reg[j]='\x0';				// Add end of string
		instr_encodee[1]=reg_num(reg);
		
		
		// Next information is a value (instruction address or offset)
		for(;input[i]==',' || input[i]==32 || input[i]=='$';i++);	// On va jusqu'a la valeur suivante
		
		instr_encodee[2] = getValueStr(input, &i); // On récupère la valeur
	}
	
	else if (instr_encodee[0]==LW || instr_encodee[0]==SW)
	{
		// operands : register, memory destination
		
		// memory destination : offset(base)
		// base is a register, offset is an immediate value
		// to simplify our assembly code, memory destination operand is only the address in data memory
		
		char reg[REG_NAME_SIZE], addr[20];
	
		for(;input[i]==',' || input[i]==32 || input[i]=='$';i++);	// On va jusqu'au registre
		
		for(j=0;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && j<2 && input[i]!='#' && input[i]!=9;i++,j++)
		{	
			reg[j]=input[i];
		
		}	
		reg[j]='\x0';
		instr_encodee[1]=reg_num(reg);
		
		for(;input[i]==',' || input[i]==32;i++);		// On se déplace jusqu'a la valeur suivante (adresse mémoire)
		
		// get memory location
		for(j=0;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && input[i]!='#' && input[i]!=9;i++,j++)
		{	
			addr[j]=input[i];
		}	
		addr[j]='\x0';
		
		// translate string into integer
		memLocationIdx = atoi(addr);
		if(memLocationIdx >= 0 && memLocationIdx <= DATA_MEM_SIZE)
		{
			instr_encodee[2] = memLocationIdx;
		}
		else
		{
			printf ("Error on getting memory location !");
		}
	}
	
	// **************************************************************************
	// Traitement BEQ et BNE
	
	else if(instr_encodee[0]==BEQ || instr_encodee[0]==BNE)
	{
		char reg[REG_NAME_SIZE];
		char offset[20];
	
		for(;input[i]==',' || input[i]==32 || input[i]=='$';i++);		// Move to the next register
		
		for(j=0;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && j<2 && input[i]!='#' && input[i]!=9;i++,j++)
		{	
			reg[j]=input[i];
		
		}	
		reg[j]='\x0';				// Add NULL character to terminate string
		instr_encodee[1]=reg_num(reg);
		
		for(;input[i]==',' || input[i]==32 || input[i]=='$';i++);		// Move to the next register
		
		for(j=0;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && j<2 && input[i]!='#' && input[i]!=9;i++,j++)
		{	
			reg[j]=input[i];
			
		}	
		reg[j]='\x0';				// Add NULL character to terminate string
		instr_encodee[2]=reg_num(reg);
		
		
		for(;input[i]==',' || input[i]==32;i++);		// Move to the offset
		
		// Copy the label
		for(j=0;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && input[i]!='#' && input[i]!=9;i++,j++)
		{
			offset[j]=input[i];
			
		}	
		offset[j]='\x0';				// Add NULL character to terminate string
		
		instr_encodee[3] = getValueStr(offset, NULL);
	}
	
	// **************************************************************************
	// Traitement J et JAL
	
	else if(instr_encodee[0] == J || instr_encodee[0] == JAL)
	{
		char label[20];
		
		for(;input[i]==32;i++);		// Move to the label
		
		// Copy the label
		for(j=0;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && input[i]!='#' && input[i]!=9;i++,j++)
		{
			label[j]=input[i];
		}
		label[j]='\x0';				// Add NULL character to terminate string
		
		instr_encodee[1]=label_pos(label);
	}
	
	
	// debuggage
	// print instr_encodee
	/*
	printf("instr_encodee : ");
	for(k=0; k<FIELD_INST_NBR; k++){
		printf("%d ", instr_encodee[k]);
	}
	printf("\n");
	*/
}	


void load_instruct_mem(struct instruct_mem* im,int mem_pos,int* instruct, char* instruction)
{
	// copy the instruction string into memory instruction
	strcpy( im->mem[mem_pos].c, instruction);
	
	// copy the instruction encoded into memory instruction
	int j;
	for(j=0; j<FIELD_INST_NBR; j++){
		im->mem[mem_pos].cod[j]=instruct[j];
	}
}



void decode(int*instr_encodee_inst,struct data_mem*dm)
{
	switch(instr_encodee_inst[0])
	{
		case ADD :
				add(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;
				
		case SUB :
				sub(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;
				
		case AND :
				and(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;
				
		case OR  :
				or(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;
				
		case XOR  :
				xor(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;
				
		case SLT :
				slt(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);	
				break;
				
		case ADDI :
				addi(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;
				
		case LUI : 	
				lui(instr_encodee_inst[1],instr_encodee_inst[2]);
				break;
				
		case ROTR:
				rotr(instr_encodee_inst[1], instr_encodee_inst[2], instr_encodee_inst[3]);
				break;
				
		case SLL:
				sll(instr_encodee_inst[1], instr_encodee_inst[2], instr_encodee_inst[3]);
				break;
				
		case SRL:
				srl(instr_encodee_inst[1], instr_encodee_inst[2], instr_encodee_inst[3]);
				break;
				
		case SYSCALL : 
				syscall();
				break;
				
		case LW   :
				load_word(instr_encodee_inst[1],instr_encodee_inst[2],dm);
				break;
				
		case SW   :
				store_word(instr_encodee_inst[1],instr_encodee_inst[2],dm);
				break;
				
		case J	  :	
				jump(instr_encodee_inst[1]);
				break;
				
		case JAL :
				jal(instr_encodee_inst[1]);
				break;
				
		case JR  :
				jr(instr_encodee_inst[1]);
				break;
				
		case BEQ  :
				beq(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;
				
		case BNE  :
				bne(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;
				
		case BGTZ  :
				bgtz(instr_encodee_inst[1],instr_encodee_inst[2]);
				break;
				
		case BLEZ  :
				blez(instr_encodee_inst[1],instr_encodee_inst[2]);
				break;
				
				
		default   :
				printf("Unknown instruction");
				pc++;
	}
}


int label_pos(char*name)
{
	int i;
	
	for(i=0;i<=label_num;i++)
	{
		if(!strcmp(labels.label[i].name,name))
			return i;
	}
	
	// Le label n'a pas ete trouve donc on creer un emplacement pour le label 
	strcpy(labels.label[label_num].name,name);
	labels.label[label_num].inst_num=-1;
	
	// on retourne la position du label dans le tableau des labels et on incrémente le nombre de labels
	return label_num++;
}


void execute(struct instruct_mem*im,int fin,struct data_mem*dm, int modePas_A_Pas)
{
	char c;
	char chaine[25];
	int sortieBoucle = 0;
	int quitter =0;
	
	pc = 0;			// Starts with the program counter at zero
	
	while(pc<=fin)	// Tant que pc n'a pas atteint sa valeur max
	{
		//printf("pc = %d\n",pc);
		printf("\nInstruction %i : %s (hexa: 0x%s)\n",pc, im->mem[pc].c, im->mem[pc].hexaStr);		// On affiche l'instruction et sa valeur en hexa
		decode(im->mem[pc].cod,dm);																	// On decode et execute l'instruction
		afficher_registres();																		// On affiche le contenu des registres
		
		
		if((modePas_A_Pas == 1) && (pc<=fin)){	// Si on est en mode pas a pas
			
			do{
				printf("\nPour executer l'instruction suivante appuyez sur 's', tapez 'f' pour entrer une autre fonction \n");
				
				c = getchar();		// on récupère le caractère entré par l'utilisateur
				
				if(c == 'f' || c == 'F'){	// Si c'est un F
				
					clear_stdin();		// On purge le flux d'entrée standard
					
					do{
						printf("Entrez une fonction ou EXIT pour quitter :\n");		// On propose a l'utilisateur d'entrer une fonction ou de quitter
						fgets(chaine, sizeof(chaine), stdin);						// On récupere la chaine entrée
						quitter = menu(chaine, dm);									// On la passe a la fonction menu
																					// Cette fonction renvoie 1 si l'utilisateur a entré EXIT
																					//						  2 si l'utilsateur a entré une commande inconnue
																					// 						  0 sinon
																					// Si la commande entrée est "print_reg x", la fonction affiche le contenu du registre x
																					// Si la commande entrée est "print_mem x", la fonction affiche le contenu à l'adresse x
																					
					}while(quitter == 2);	// On repete tant que la commande n'est pas reconnue 
					
					if(quitter == 1)		// Si la commande est exit
					{
						pc = fin;			// On met pc à fin
						sortieBoucle = 1;	// Et on sortira de la boucle
					}
					
				}
				
				else if( c == 's' || c == 'S')	// Si la lettre est un S
				{
					sortieBoucle = 1;	// On sort de la boucle pour executer l'instruction suivante si pc <= fin
				}
				
				else	// Si la lettre n'est ni s ni f
				{
					printf("Commande non reconnue");	// On laisse sortieBoucle à 0
				}
				
			}while(sortieBoucle==0);
		}
	}
}

int getValueStr(char *str, int *idx)
{
	int val = 0;
	int i;
	
	// get index
	if(idx == NULL)
	{
		i = 0;
	}
	else
	{
		i = *idx;
	}
	
	// jump space
	for(;str[i] == ' '; i++);
	
	// sign test
	int sign = 1;
	if(str[i] == '+' || str[i] == '-')
	{
		if(str[i] == '-')
		{
			sign = -1;
		}
		
		i++;
	}
	
	// get integer
	for(; str[i]!=32 && str[i]!=10 && str[i]!='\x0' && str[i]!=',' && str[i]!='#'; i++)
	{
		val = val * 10 + (str[i] - '0');
	}
	
	if(idx != NULL)
		*idx = i;
	
	return val * sign;
}

uint32_t instrCode(int *instr_encodee)
{
	uint32_t hexa = 0;
	
	switch(instr_encodee[0])
	{
		// R-Type
		case ADD :
		case AND :
		case SUB :
		case SLT :
		case OR  :
		case XOR :
			hexa |= ((uint32_t)instr_encodee[1]) << 11;
			hexa |= ((uint32_t)instr_encodee[2]) << 21;
			hexa |= ((uint32_t)instr_encodee[3]) << 16;
			hexa |= (uint32_t)opcodeVal[ instr_encodee[0] -1 ];
			break;
		
		case ADDI :
			hexa |= ((uint32_t)opcodeVal[ADDI -1]) << 26;
			hexa |= ((uint32_t)instr_encodee[1]) << 16;
			hexa |= ((uint32_t)instr_encodee[2]) << 21;
			hexa |= (uint32_t)instr_encodee[3];
			break;
			
		case MULT :
		case DIV  :
			hexa |= ((uint32_t)instr_encodee[1]) << 21;
			hexa |= ((uint32_t)instr_encodee[2]) << 16;
			hexa |= (uint32_t)opcodeVal[ instr_encodee[0] -1 ];
			break;
			
		case BEQ :
		case BNE :
			hexa |= ((uint32_t)opcodeVal[ instr_encodee[0] -1 ]) << 26;
			hexa |= ((uint32_t)instr_encodee[1]) << 21;
			hexa |= ((uint32_t)instr_encodee[2]) << 16;
			hexa |= (uint32_t)instr_encodee[3];
			break;
			
		case BGTZ :
		case BLEZ :
			hexa |= ((uint32_t)opcodeVal[ instr_encodee[0] -1 ]) << 26;
			hexa |= ((uint32_t)instr_encodee[1]) << 21;
			hexa |= (uint32_t)instr_encodee[2];
			break;
			
		case J 	 :
		case JAL :
			hexa |= ((uint32_t)opcodeVal[ instr_encodee[0] -1 ]) << 26;
			// the operand instr_encodee[1] contain the label index (done in encode function)
			// normalement dans le champs 'instr_index' (bit0-25) il doit y avoir les bits 2 à 27 de l'adresse de saut,
			// mais on ne gère pas la mémoire à l'octet.
			// lors de l'exécution de ces instructions, on doit charger PC avec la valeur de l'opérande décalée
			// à gauche de 2 et auquel on ajoute les 4 bits de poids fort de PC.
			// Dans notre cas, comme la mémoire n'est pas gérée à l'octet mais à une taille d'instruction (structure),
			// cette opérande devrait prendre les bits 0 à 25 de l'adresse de saut, et donc lors de l'exécution,
			// l'adresse serait cette opérande + les 6 bits de poids fort de PC
			// On peut donc accéder à une instruction dans la région d'adresses alignées de 2^(26) = 64Mi (64MB)
			// On ne fait pas celà car notre mémoire d'instruction est bien plus petite (plantage assuré si allocation d'un tel tableau).
			// Donc cette opérande prendra directement l'adresse de saut.
			hexa |= (uint32_t)labels.label[ instr_encodee[1] ].inst_num;
			break;
			
		case JR :
			hexa |= ((uint32_t)instr_encodee[1]) << 21;
			hexa |= (uint32_t)opcodeVal[ JR -1 ];
			break;
			
		case LW :
		case SW :
			// as said above (in encode function for LW and SW), we just give memory location to simplify assembler code
			// ADDRESS IS PLACED IN OFFSET FIELD (bit0-15) !
			hexa |= ((uint32_t)opcodeVal[ instr_encodee[0] -1 ]) << 26;
			hexa |= ((uint32_t)instr_encodee[1]) << 16;
			hexa |= (((uint32_t)instr_encodee[2]) & 0x0000FFFF); // masque de sécurité (si l'adresse est trop grande)
			break;
			
		case LUI :
			hexa |= ((uint32_t)opcodeVal[LUI -1]) << 26;
			hexa |= ((uint32_t)instr_encodee[1]) << 16;
			hexa |= ((uint32_t)instr_encodee[2]);
			break;
			
		case MFHI :
		case MFLO :
			hexa |= ((uint32_t)instr_encodee[1]) << 11;
			hexa |= (uint32_t)opcodeVal[ instr_encodee[0] -1 ];
			break;
			
		case ROTR :
		case SRL  :
		case SLL  :
			if(instr_encodee[0] == ROTR)
			{
				hexa |= 0b00000000001000000000000000000000;
			}
			// sinon si () R est déjà à 0
			/*
			else if(instr_encodee[0] == SRL)
			{
				
			}*/
			hexa |= ((uint32_t)instr_encodee[1]) << 11;
			hexa |= ((uint32_t)instr_encodee[2]) << 16;
			hexa |= ((uint32_t)instr_encodee[3]) << 6;
			hexa |= (uint32_t)opcodeVal[ instr_encodee[0] -1 ];
			break;
		
		case SYSCALL :
			hexa |= (uint32_t)opcodeVal[ SYSCALL -1 ];
			break;
			
		default :
			printf("Impossible de traduire une instruction (enum opcode : %i) en hexadecimal (par default : 0x00000000)\n"
					"Dans la fonction (uint32_t instrCode(int *instr_encodee))\n"
					"Elle n'est peut-etre pas referencee\n\n", instr_encodee[0]);
	}
	
	return hexa;
}

void convDecToBase(int decimal, char *hexa, int size, const int base)
{
	int value, i = size -2;
    char car[] = "0123456789ABCDEF";
	
	hexa[size-1] = '\0';
	
	if(base >= 2 && base <= 16)
	{
	    // The lower digit has the highest index (i) (just before '\0')
	    do
	    {
	        value = decimal % base;
	        decimal /= base;
	
	        // adding value to hexa string
	        hexa[i] = car[value];
	
	        i--;
	    }while(decimal > 0 && i >= 0);
	    
	    if(decimal > 0 && i < 0)
	    {
	    	// should never happen
	    	printf("Instruction size error during conversion into hexadecimal !\n");
	    }
	}
	else
	{
		printf("Impossible to convert to base %d\n", base);
	}
    
    // fill with zeros
    for(; i >= 0; i--)
    {
    	hexa[i] = '0';
    }
}

void convDecToHex(int decimal, char *hexa, int size)
{
    convDecToBase(decimal, hexa, size, 16);
}

// finally not used because the instructions contain 8 hexadecimal characters (fix number)
int getSizeHexaStrFromDec(int decimal)
{
    // (log (base 16) decimal) + 1 + 1 (end of string character)
    return (log(decimal) / log(16) + 2);
}
