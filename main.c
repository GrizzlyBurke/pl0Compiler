#include <stdio.h>
#include<stdlib.h>
#include "utilities.h"

typedef struct Stack
{
    int * data;
}Stack;

void init_stack(struct Stack * s);

int main(int argc, char *argv[]) 
{
    int code[MAX_CODE_LENGTH];
    int BP = 0, SP = 0, PC = 0;

    //create and initilize the stack
    struct Stack * stack;
    innit_stack(stack);

    if (argc != 2) 
    {
        printf("Error: Invalid number of arguments.\n");
        return 1;
    }
    char * filename = argv[1];

    // read the file and load the code into the code array
    FILE * fp = fopen(filename, "r");

    if (!fp) 
    {
        printf("Error: Failed to open file %s.\n", filename);
        return 1;
    }
    int i = 0;
    while(fscanf(fp, "%d", &code[i]) != EOF) {
        i++;
    }
    fclose(fp);

    while (code[PC] != 13) 
    {

            if (SP >= MAX_STACK_HEIGHT) {
                printf("Error: stack overflow\n");
            return 1;
            }
            if (SP < BP) {
                printf("Error: stack underflow\n");
            return 1;
            }

            switch (code[PC]) {
            case 1: // LIT
                SP++;
                stack->data[SP] = code[PC + 1];
                PC++;
                break;
            case 2: // RTN
                SP = BP - 1;
                PC = stack->data[SP + 3];
                BP = stack->data[SP + 2];
                break;
            case 3: // CAL
                stack->data[SP + 1] = BP;
                stack->data[SP + 2] = PC;
                BP = SP + 2;
                PC = code[PC + 1];
                break;
            case 4: // POP
                SP--;
                break;
            case 5: // PSI
                stack->data[SP] = stack->data[stack->data[SP]];
                break;
            case 6: // PRM
                SP++;
                stack->data[SP] = stack->data[BP - code[PC + 1]];
                PC++;
                break;
            case 7: // STO
                stack->data[stack->data[SP - 1] + code[PC + 1]] = stack->data[SP - 2];
                SP -= 2;
                PC++;
                break;
            case 8: // INC
                SP += code[PC + 1];
                PC++;
                break;
            case 9: // JMP
                PC = stack->data[SP - 1];
                SP--;
                break;
            case 10: // JPC
                if (stack->data[SP - 1] != 0) {
                    PC = code[PC + 1];
                }
                SP--;
                PC++;
                break;
            case 11: // CHO
                printf("%c", stack->data[SP - 1]);
                SP--;
                break;
            case 12: // CHI
                SP++;
                stack->data[SP] = getchar();
                break;
            case 14: // NDB
                // stop printing debugging output
                break;
            default:
                printf("Error: Invalid instruction %d at PC = %d\n", code[PC], PC);
                return 1;
        }
        PC++;
    }
}

void init_stack(struct Stack * s)
{
    //all values are initilzied to 0
    s->data = calloc(MAX_STACK_HEIGHT, sizeof(int));   

    //checks the return of calloc, if allocation has been done correctly
    if(s->data == NULL)
    {
        printf("Error: failed to allocate memory for the stack\n");
        exit(1);
    }
}