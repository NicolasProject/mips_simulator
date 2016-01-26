
// Encodage et décodage des instructions. 

#include "instruction_mem.h"


//Definition des opcodes 
static const char funct[][8]={"add","sub","slt","and","or","syscall","beq","lw","sw","li","move","j"};
static const int opcode[]={ADD,SUB,SLT,AND,OR,SYSCALL,BEQ,LW,SW,LI,MOVE,J};

void encode(char*input,int*instr_encodee,struct data_mem*dm,int num)
{
	char inst[10];		// Contient l'instruction (comme add,sub,move,b)
	int j=0,i=0,k=0;
	char*ptr;
	int memLocationIdx;
	
	/****** Traitement particulier pour les labels ****************/
			
	if( (ptr = strchr(input,':')) != 0 )
	{
		// Pour les labels, on met instr_encodee[0]=0 (this way it is not loaded into the instruction memory if this value doesn't change)
		instr_encodee[0]=0;
		int already_exists=0;			// Variable indiquant si le label est présent en mémoire
					
		for(; input[i]!=*ptr && input[i]!=32; i++){		//On recupere le texte du label dans inst
			inst[i]=input[i];
		}
		
		inst[i]='\x0';		//Ajout du caractère de fin de chaine
		
		
		for(j=0; j<label_num; j++)	// On parcoure les labels existants (label_num à 0 au début)
		{
			if(!strcmp(labels.label[j].name,inst))	// si le label dans inst est dejà dans le tableau des labels à la case j
			{
				if(labels.label[j].inst_num==-1)	// mais qu'il n'est pas dans une instruction : inst_num = -1
				{
					already_exists=1;				// alors maintenant il existe
					labels.label[j].inst_num=num;	// On affecte le numéro de l'instruction dans laquelle il se trouve
					return;
				}
				else	// Sinon, il a été utilisé plus d'une fois
				{
					printf("Label %s a été utilisé plus d'une fois \n",inst);
					exit(4);		// Label re-use
				}
			}
		}
		
		if(!already_exists)		// Si le label n'existe pas
		{
			strcpy(labels.label[label_num].name,inst);	// On le met dans le tableau des labels
			labels.label[label_num].inst_num=num;		// On affecte le numéro de l'instruction correspondante
			label_num++;								// On incrémente le nombre de label
		
			//printf("%d",i);
			for(; input[i]==32 || input[i]==':' || input[i]==9; i++);
		
			//printf("char=%c\n",input[i]);
		
			if(input[i]=='\n' || input[i]=='\x0' || input[i]=='#'){
				return;
			}
			
		}		
	}		
	
	/*****************Traitement de l'instruction si elle ne contient pas de label***********************************************/
	
	
	for(j=0; input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=9; i++,j++)
	{
		//On copie le premier mot dans inst
		inst[j]=input[i];
			
	}
	inst[j]='\x0';			// On ajoute le caractère de fin de chaine
	
	for(j=0;j<=11;j++)		// On balaye les opcodes
	{
		if(!strcmp(inst,funct[j])){		// Si l'opcode à l'indice j est celui que l'on a récuperé
			break;						// On sort de la boucle en gardant la valeur de l'indice j
		}
		
	}
	
	if(j<=11){							// Si l'opcode récuperé est parmi ceux du tableau funct
		instr_encodee[0]=opcode[j];		// On affecte l'opcode à la chaine encodée à l'indice 0 (l'opcode est donné par un define dans instruction_mem.h)
	}
	
	else 											// Si l'opcode recupéré n'est pas dans le tableau funct
	{
		printf("Instruction inconnue : %s",inst);	// Alors il s'agit d'une instruction inconnue
		exit(3);
	}
		
	/***************** Gestion des instructions de type R (ADD/SUB/AND/OR/SLT)****************/
	
	if(instr_encodee[0]==ADD||instr_encodee[0]==SUB||instr_encodee[0]==AND||instr_encodee[0]==OR||instr_encodee[0]==SLT)
	{	
		// On vérifie si l'instruction est de type R
		
		char reg[3];  // chaine qui contiendra un nom de registre
		int j;	
	
		for(k=1; k<=3; k++){
			
			for(; input[i]==',' || input[i]==32 || input[i]=='$'; i++);		// On se rend au mot suivant qui sera le premier registre
			
			// Il peut être une valeur immediate ou un registre (si il est précedé de $ c'est un registre)
			if(input[i-1]!='$')		// Valeur immédiate
			{
				int val=0;
				// On récupère le nombre entier
				for(; input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && input[i]!='#'; i++){
					val=val*10+(input[i]-'0');
				}
				
				// Pour différencier valeur immédiate et registre on utilisera les nombres à partir de 32 pour les valeurs immédiate
				// Les registres allant de 0 à 31
				instr_encodee[k]=32+val;		// On ajoute alors la valeur à l'instruction encodée à l'indice 1
			}
			else	// Registre
			{
				for(j=0; input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && j<2 && input[i]!='#' && input[i]!=9; i++,j++)
				{	
					reg[j]=input[i];	//On récupère le nom du registre
				
				}	
				reg[j]='\x0';	// On ajoute le caractère de fin de chaine
				instr_encodee[k]=reg_num(reg);	// On recupere son numéro grâce à la fonction reg_num et on l'ajoute à l'instruction encodée
			}
		}
		
		//printf ("%d %d %d %d",instr_encodee[0],instr_encodee[1],instr_encodee[2],instr_encodee[3]);	 
	}
	
	
	/******************** Traitement type I *****************/
	else if(instr_encodee[0]==LI)
	{
		char reg[3];
		int j;	
		

		for(;input[i]==',' || input[i]==32 || input[i]=='$';i++);		// Move to the next register
		
		for(j=0;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && j<2 && input[i]!='#' && input[i]!=9;i++,j++)
		{	
			reg[j]=input[i];
		
		}	
		reg[j]='\x0';				// Add NULL character to terminate string
		instr_encodee[1]=reg_num(reg);
		
		
		// Next information is the value to be stored.
		for(;input[i]==',' || input[i]==32 || input[i]=='$';i++);		// Move to the next register
		
		instr_encodee[2]=0;
		for(;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && input[i]!='#' && input[i]!=9;i++)
		{	
			instr_encodee[2]=instr_encodee[2]*10+(input[i]-'0');
		}	
		//printf ("%d %d %d",instr_encodee[0],instr_encodee[1],instr_encodee[2]);
	}
	
	else if (instr_encodee[0]==LW || instr_encodee[0]==SW)
	{
		/* These instructions have a register and then a memory destination */
		
		char reg[3],var_name[20];
		int j;	
	
		for(;input[i]==',' || input[i]==32 || input[i]=='$';i++);		// Move to the next register
		
		for(j=0;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && j<2 && input[i]!='#' && input[i]!=9;i++,j++)
		{	
			reg[j]=input[i];
		
		}	
		reg[j]='\x0';				// Add NULL character to terminate string
		instr_encodee[1]=reg_num(reg);
		
		for(;input[i]==',' || input[i]==32;i++);		// Move to the memory location given
		
		for(j=0;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && input[i]!='#' && input[i]!=9;i++,j++)
		{	
			var_name[j]=input[i];
		
		}	
		var_name[j]='\x0';
		
		// Store the mem_location (in data memory)
		if((memLocationIdx = get_mem_location(var_name,dm)) != -1)
		{
			instr_encodee[2] = memLocationIdx;
		}
		else
		{
			printf ("Error on getting memory location !");
		}
		
		//printf ("%d %d %d",instr_encodee[0],instr_encodee[1],instr_encodee[2]);
	}
	/******************************************************************************/
	
	
	/************************* DEALING WITH MOVE *******************************/
	else if(instr_encodee[0]==MOVE)
	{
		char reg[3];
		int j;
	
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
	}
	/*************************************************************************************/
	
	/******************************* DEALING WITH beq AND j *****************************/
	
	else if(instr_encodee[0]==BEQ)
	{
		char reg[3];
		char label[20];
		
		int j;
	
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
		
		
		for(;input[i]==',' || input[i]==32;i++);		// Move to the label
		
		// Copy the label
		for(j=0;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && input[i]!='#' && input[i]!=9;i++,j++)
		{
			label[j]=input[i];
			
		}	
		label[j]='\x0';				// Add NULL character to terminate string
		
		instr_encodee[3]=label_pos(label);
	}
	
	else if(instr_encodee[0]==J)
	{
		char label[20];
		int j;
		
		for(;input[i]==32;i++);		// Move to the label
		
		// Copy the label
		for(j=0;input[i]!=32 && input[i]!=10 && input[i]!='\x0' && input[i]!=',' && input[i]!='#' && input[i]!=9;i++,j++)
		{
			label[j]=input[i];
		}
		label[j]='\x0';				// Add NULL character to terminate string
		
		instr_encodee[1]=label_pos(label);
		
	
	}
	
	// convert intruction to hexa and write it in a file
	FILE* hexaFile = NULL;
	hexaFile = fopen("hexa.txt", "a");
	if(hexaFile != NULL)
	{
		char hexaStr[9];
		
		convDecToHex(instrCode(instr_encodee), hexaStr);
		fprintf(hexaFile, "%s\n", hexaStr);
		
		// close file
		if(fclose(hexaFile) == EOF)
		{
			printf("Error closing file hexa.txt !");
		}
	}
	else
	{
		printf("Error opening file hexa.txt !");
	}
	
}	



void load_instruct_mem(struct instruct_mem*im,int mem_pos,int*instruct)
{
	//printf("\ninside_load_inst :%d %d %d %d",instruct[0],instruct[1],instruct[2],instruct[3]);
	int j=0;
	for(j=0;j<4;j++)
		im->mem[mem_pos].cod[j]=instruct[j];
	
	//printf("\n%d %d %d %d ",im->mem[mem_pos].cod[0],im->mem[mem_pos].cod[1],im->mem[mem_pos].cod[2],im->mem[mem_pos].cod[3]);
}



void decode(int*instr_encodee_inst,struct data_mem*dm)
{
	//printf("(decode) %d %d %d %d\n",instr_encodee_inst[0],instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
	switch(instr_encodee_inst[0])
	{
		case ADD :	
				add(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;
		case SUB :
				sub(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;
		case AND :
				and_(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;
		case OR  :
				or_(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;		
		case SLT :
				slt(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);	
				break;
		
		case LI	 : 	
				li(instr_encodee_inst[1],instr_encodee_inst[2]);
				break; 
		
		case SYSCALL : 
				syscall();
				break;
		case MOVE :
				move(instr_encodee_inst[1],instr_encodee_inst[2]);
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
		case BEQ  :
				beq(instr_encodee_inst[1],instr_encodee_inst[2],instr_encodee_inst[3]);
				break;
				
		default   :	printf("Unknown instruction");
				pc++;
				//exit(3);			// Exit code 3 => unknown instruction
	}
	return;
}


int label_pos(char*name)
{
	int i;
	
	for(i=0;i<=label_num;i++)
	{
		if(!strcmp(labels.label[i].name,name))
			return i;
	}
	
	// This means the reference has not been found yet. Store it in the array 
	strcpy(labels.label[label_num].name,name);
	labels.label[label_num].inst_num=-1;
	
	return label_num++;
	
}


void execute(struct instruct_mem*im,int fin,struct data_mem*dm, int modePas_A_Pas)
{
	pc = 0;			// Starts with the program counter at zero
	while(pc<=fin)
	{
		//printf("pc=%d\n",pc);
		decode(im->mem[pc].cod,dm);	
		
		
		afficher_registres();
		if((modePas_A_Pas == 1) && (pc<=fin)){
			printf("\nPour executer l'instruction suivant appuyez sur la touche entree \n");
			getchar();
		}
		//scanf("%c",&a);
	}
}

uint32_t instrCode(int *instr_encodee)
{
	uint32_t hexa = 0;
	
	// set special (6 first bits)
	switch(instr_encodee[0])
	{
		// R-Type
		case ADD :
		case AND :
		case SUB :
		case SLT :
		case OR  :
			hexa = 0;
			hexa |= ((int)instr_encodee[1]) << 11;
			hexa |= ((int)instr_encodee[2]) << 21;
			hexa |= ((int)instr_encodee[3]) << 16;
			break;
		
		/*case SW :
			hexa |= ((int)0b101011) << 26;
			break;*/
			
		/*case LW :
			hexa |= ((int)0b100011) << 26;
			break;*/
			
		/*case J :
			hexa |= ((int)0b000010) << 26;
			break;*/
			
		/*case BEQ  :
			hexa |= ((int)0b000100) << 26;
			break;*/
	}
	
	switch(instr_encodee[0])
	{
		// R-Type
		case ADD :
			hexa |= 0b100000;
			break;
			
		case AND :
			hexa |= 0b100100;
			break;
			
		case SUB :
			hexa |= 0b100010;
			break;
			
		case SLT :
			hexa |= 0b101010;
			break;
			
		case OR  :
			hexa |= 0b100101;
			break;
	}
	
	return hexa;
}

void convDecToHex(int decimal, char *hexa)
{
    int value, i = 0;
    char car[] = "0123456789ABCDEF";

    hexa[0] = '\0';
    // le digit de poids faible a l'indice le plus eleve (situe juste avant '\0')
    do
    {
        value = decimal % 16;
        decimal /= 16;

        // on ajoute la valeur a la chaine hexa
        hexa[i+1] = hexa[i];
        hexa[i] = car[value];

        i++;
    }while(decimal > 0);
}

// finalement non utilise car les instructions contiennent 8 caracteres hexa
int getSizeHexaStrFromDec(int decimal)
{
    // (log (base 16) decimal) + 1 + 1 (caractere fin de chaine)
    return (log(decimal) / log(16) + 2);
}
