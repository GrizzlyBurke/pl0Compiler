#include <stdio.h>
#include<stdlib.h>
#include "utilities.h"

typedef struct Stack
{
    int * data;
}Stack1;

typedef struct instruction
{
    int opcode;
    int m;
}instruction;

void initStack(int * stack);
void printInstructions(instruction * code, int count);
void printPointers( int BP, int SP, int PC);
void printStack(int * stack, int SC);

int main(int argc, char *argv[]) 
{
    instruction code[MAX_CODE_LENGTH];
    int stack[MAX_STACK_HEIGHT];
    int BP = 0, SP = 0, PC = 0;

    //initilize the stack
    initStack(stack);

    if (argc != 2) 
    {
        printf("Error: Invalid number of arguments.\n");
        return 1;
    }
    char * filename = argv[1];

    // read the file and load the code into the code array
    FILE * fp = fopen(filename, "r");

    if (fp == NULL) 
    {
        printf("Error: Failed to open file %s.\n", filename);
        return 1;
    }
    int i = 0;
    while(fscanf(fp, "%d %d", &code[i].opcode, &code[i].m) != EOF) 
    {
        i++;
    }
    fclose(fp);

    printInstructions(code, i);
    printf("Tracing...\n");
    printPointers(BP, SP, PC);
    printStack(stack, SP);

    while (1) 
    {
        

            if (SP >= MAX_STACK_HEIGHT) {
                printf("Error: stack overflow\n");
            return 1;
            }
            if (SP < BP) {
                printf("Error: stack underflow\n");
            return 1;
            }

            switch (code[PC].opcode) {
            case 1: // LIT
                stack[SP] = code[PC + 1].m;
                SP++;
                PC++;
                break;
            case 2: // RTN
                SP = BP - 1;
                PC = stack[SP + 3];
                BP = stack[SP + 2];
                break;
            case 3: // CAL
                stack[SP + 1] = BP;
                stack[SP + 2] = PC;
                BP = SP + 2;
                PC = code[PC + 1].m;
                break;
            case 4: // POP
                SP--;
                break;
            case 5: // PSI
                stack[SP] = stack[stack[SP]];
                break;
            case 6: // PRM
                SP++;
                stack[SP] = stack[BP - code[PC + 1].m];
                PC++;
                break;
            case 7: // STO
                stack[stack[SP - 1] + code[PC + 1].m] = stack[SP - 2];
                SP -= 2;
                PC++;
                break;
            case 8: // INC
                SP += code[PC].m;
                PC++;
                break;
            case 9: // JMP
                PC = stack[SP - 1];
                SP--;
                break;
            case 10: // JPC
                if (stack[SP - 1] != 0) {
                    PC = code[PC + 1].m;
                }
                SP--;
                PC++;
                break;
            case 11: // CHO
                printf("%c", stack[SP - 1]);
                SP--;
                break;
            case 12: // CHI
                SP++;
                stack[SP] = getchar();
                break;
            case 13: //HLT
                PC++;
                break;
            case 14: // NDB
                // stop printing debugging output
                break;
            case 15: // NEG
                stack[SP - 1] = -stack[SP - 1];
                break;
            case 16: // ADD
                stack[SP - 2] = stack[SP - 1] + stack[SP - 2];
                SP -= 1;
                break;
            case 17: // SUB
                stack[SP - 2] = stack[SP - 1] - stack[SP - 2];
                SP -= 1;
                break;
            case 18: // MUL
                stack[SP - 2] = stack[SP - 1] * stack[SP - 2];
                SP -= 1;
                break;
            case 19:  // DIV
                if(stack[SP-2] == 0){
                    fprintf(stderr, "Error: Division by zero\n");
                    exit(EXIT_FAILURE);
                }
                stack[SP-2] = stack[SP-1] / stack[SP-2];
                SP--;
                break;
            case 20:  // MOD
                if(stack[SP-2] == 0){
                    fprintf(stderr, "Error: Division by zero\n");
                    exit(EXIT_FAILURE);
                }
                stack[SP-2] = stack[SP-1] % stack[SP-2];
                SP--;
                break;
            case 21: // EQL
                stack[SP - 2] = stack[SP - 1] == stack[SP - 2] ? 1 : 0;
                SP -= 1;
                break;
            case 22: // NEQ
                stack[SP - 2] = stack[SP - 1] != stack[SP - 2] ? 1 : 0;
                SP -= 1;
                break;
            case 23: // LSS
                stack[SP - 2] = stack[SP - 1] < stack[SP - 2] ? 1 : 0;
                SP -= 1;
                break;
            case 24: // LEQ
                stack[SP - 2] = stack[SP - 1] <= stack[SP - 2] ? 1 : 0;
                SP -= 1;
                break;
            case 25: // GTR
                stack[SP - 2] = stack[SP - 1] > stack[SP - 2] ? 1 : 0;
                SP -= 1;
                break;
            case 26: // GEQ
                stack[SP - 2] = stack[SP - 1] >= stack[SP - 2] ? 1 : 0;
                SP -= 1;
                break;
            case 27: // PSP
                stack[SP] = SP;
                SP += 1;
                break;
            default:
                printf("Error: Invalid instruction %d at PC = %d\n", code[PC].opcode, PC);
                return 1;
        }

        printPointers(BP, SP, PC);
        printStack(stack, SP);
        if(code[PC].opcode == 13)
            {
                PC++;
                printPointers(BP, SP, PC);
                printStack(stack, SP);
                break;
            }

        
    }
    
}

void initStack(int * stack)
{
    //all values are initilzied to 0
    for (int i = 0; i < MAX_STACK_HEIGHT; i++)
    {
        stack[i] = 0;
    }
}

void printInstructions(instruction * code, int count)
{
    printf("ADR\t OP\t m\n");
    for (int i = 0; i < count; i++ )
    {
        printf("%d\t %d\t %d\n", i, code[i].opcode, code[i].m);
    }
}

void printPointers(int BP, int SP, int PC)
{
    printf("PC: %d BP: %d SP: %d\n", PC, BP, SP);
}

void printStack(int * stack, int SP)
{
    printf("stack: ");
    for (int i = 0; i < SP; i++)
    {
        printf("s[%d]: %d ", i, stack[i]);
    }
    printf("\n");
}