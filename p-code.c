#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int amax = 2047; // maximum address
const int levmax = 3;  // maximun depth of block nesting
const int cmax = 200;  // size of code array

#define C_MAX_INSTRUCTIONS 200 // Max Size of Code Array
// List of instructions
enum
{
	LIT,
	OPR,
	LOD,
	STO,
	CAL,
	INT,
	JMP,
	JPC
} instructionC;
char *instructionString[] = {"LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC"};

typedef struct
{
	int operation; // f
	int level;	   // l
	int argument;  // a
} instructionSTRUCT;

// Array of code-instructions >INPUT<
instructionSTRUCT instructions[C_MAX_INSTRUCTIONS];

#define stacksize 500

int program = 0, base = 1, topstack = 0; // pointer//Base-Register//topstack-register

// DATASTORE
int stack[stacksize] = {-1};

// PROTOTYPES
int getBase(int level);
int getInstructionCode(char *code);
void execute();
char *getInstructionName(int i);
// MAIN
int main()
{
	char function[3];					  // Function Buffer
	int l = 0, a = 0;					  // Level and Argument Buffers
	instructionSTRUCT instructionCurrent; // Stores the Current instruction
	int totalInstructions = 0;

	stack[0] = 0;
	stack[1] = 0;
	stack[2] = 0;

	printf("Please enter the instructions (Instruction END 0 0 to stopstack the input)\n");

	for (int i = 0; i < C_MAX_INSTRUCTIONS; i++)
	{

		scanf("\n%[^ ] %d %d", function, &l, &a);

		if (strcmp(function, "END") == 0)
		{
			printf("---End of Program---\n");
			break;
		}
		else
		{
			int code = getInstructionCode(function);
			if (code != -1)
			{
				instructionCurrent.operation = code;
				instructionCurrent.level = l;
				instructionCurrent.argument = a;

				// Store the instruction for Logs
				instructions[i] = instructionCurrent;
				totalInstructions++;
			}
			else
			{
				printf("Unknow Instruction\n");
				i--; // Rewind the program
			}
		}
	}
	printf("\n...Starting the P-code...\n");
	printf("%-10s %-15s %-15s %-15s %-15s %s\n", "Inst", "Level", "Arg", "topstack", "program", "Stack");
	do
	{
		execute();
	} while (program != 0);

	return 0;
}

int getBase(int level)
{
	int newBase;

	newBase = base;
	while (level > 0)
	{
		newBase = stack[newBase];
		level--;
	}
	return newBase;
}

int getInstructionCode(char *code)
{
	for (int i = 0; i < 8; i++)
	{
		if (strcmp(code, instructionString[i]) == 0)
		{ // If Input Code is Equal any Instruction Code Return 0(true)
			return i;
		}
	}
	return -1;
}

void execute()
{
	instructionSTRUCT instructionCurrent = instructions[program]; // Stores the Current instruction

	program++;
	switch (instructionCurrent.operation)
	{
	case LIT:
		topstack++;
		stack[topstack] = instructionCurrent.argument;
		break;
	case OPR:
		switch (instructionCurrent.argument)
		{
		case 0: // RTN
			topstack = base - 1;
			program = stack[topstack + 3];
			base = stack[topstack + 2];
			break;
		case 1: // NEG
			stack[topstack] = -1 * stack[topstack];
			break;
		case 2: // ADD
			topstack--;
			stack[topstack] += stack[topstack + 1];
			break;
		case 3: // SUB
			topstack--;
			stack[topstack] -= stack[topstack + 1];
			break;
		case 4: // MUL
			topstack--;
			stack[topstack] *= stack[topstack + 1];
			break;
		case 5: // DIV
			topstack--;
			stack[topstack] /= stack[topstack + 1];
			break;
		case 6: // MOD
			topstack--;
			stack[topstack] %= stack[topstack + 1];
			break;
		case 7: // ODD
			stack[topstack] = (stack[topstack] % 2 == 1);
			break;
		case 8: // EQL
			topstack--;
			stack[topstack] = (stack[topstack] == stack[topstack + 1]);
			break;
		case 9: // NEQ
			topstack--;
			stack[topstack] = (stack[topstack] != stack[topstack + 1]);
			break;
		case 10: // LSS
			topstack--;
			stack[topstack] = (stack[topstack] < stack[topstack + 1]);
			break;
		case 11: // GEQ
			topstack--;
			stack[topstack] = (stack[topstack] >= stack[topstack + 1]);
			break;
		case 12: // GTR
			topstack--;
			stack[topstack] = (stack[topstack] > stack[topstack + 1]);
			break;
		case 13: // LEQ
			topstack--;
			stack[topstack] = (stack[topstack] <= stack[topstack + 1]);
			break;
		default:
			printf("This OPR subfunction hasn't been implemented");
			break;
		}
		break;
	case LOD:
		topstack++;
		stack[topstack] = stack[getBase(instructionCurrent.level) + instructionCurrent.argument];
		break;
	case STO:
		stack[getBase(instructionCurrent.level) + instructionCurrent.argument] = stack[topstack];
		topstack--;
		break;
	case CAL:
		stack[topstack + 1] = getBase(instructionCurrent.level);
		stack[topstack + 2] = base;
		stack[topstack + 3] = program;
		base = topstack + 1;
		program = instructionCurrent.argument;
		break;
	case INT:
		topstack += instructionCurrent.argument;
		break;
	case JMP:
		program = instructionCurrent.argument;
		break;
	case JPC:
		if (stack[topstack] == 0)
		{
			program = instructionCurrent.argument;
		}
		topstack--;
		break;
	default:
		printf("This operation hasn't been implemented");
	}
	// PrintTrace
	printf("%-10s %-15d %-15d %-15d %-15d ", getInstructionName(instructionCurrent.operation), instructionCurrent.level, instructionCurrent.argument, stack[topstack], program);

	for (int aux = base; aux < topstack + 1; aux++)
		printf("%d ", stack[aux]);
	printf("\n");
}

char *getInstructionName(int i)
{
	return instructionString[i];
}
