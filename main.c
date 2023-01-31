#include<stdlib.h>
#include "utilities.h"
#include "utilities.c"



int main(int argc, char *argv[]) 
{
    instruction code[MAX_CODE_LENGTH];
    int stack[MAX_STACK_HEIGHT];
    int BP = 0, SP = 0, PC = 0, NDB = 0;

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

    printCode(code, i);
    printf("Tracing...\n");
    printPointers(BP, SP, PC, NDB);
    

    while (1) 
    {
        printStack(stack, BP, SP, NDB);

            if (SP >= MAX_STACK_HEIGHT) {
                printf("Error: stack overflow\n");
            return 1;
            }
            if (SP < BP) {
                printf("Trying to pop an empty stack!\n");
            return 1;
            }

            switch (code[PC].opcode) {
            case 1: // LIT
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB);
                stack[SP] = code[PC].m;
                SP++;
                PC++;
                break;
            case 2: // RTN
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB);                
                PC = stack[SP - 1];
                if (PC < 0)
                {
                    printf("Trying to pop an empty stack!\n");
                    exit(1);
                }
                BP = stack[SP - 2];
                SP = SP - 2;
                break;
            case 3: // CAL
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP] = BP;
                stack[SP + 1] = PC + 1;
                BP = SP;
                SP +=2;
                PC = code[PC].m;
                break;
            case 4: // POP
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                SP--;
                break;
            case 5: // PSI
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB);; 
                stack[SP - 1] = stack[SP-2];
                PC++;
                break;
            case 6: // PRM
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP] = stack[BP - code[PC].m];
                SP++;
                PC++;
                break;
            case 7: // STO
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[stack[SP - 1] + code[PC + 1].m] = stack[SP - 2];
                SP -= 2;
                PC++;
                break;
            case 8: // INC
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                SP += code[PC].m;
                PC++;
                break;
            case 9: // JMP
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                PC = stack[SP - 1];
                SP--;
                break;
            case 10: // JPC
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                if (stack[SP - 1] != 0) {
                    PC = code[PC].m;
                }
                else
                    PC++;
                    SP--;
                break;
            case 11: // CHO
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                fprintf(stdout, "%c", stack[SP - 1]);
                SP--;
                PC++;
                break;
            case 12: // CHI
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP] = getchar();
                SP++;
                PC++;
                break;
            case 13: //HLT
                PC++;
                break;
            case 14: // NDB
                // sets teh NBD variable to 1 to prevent debug printing
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                NDB = 1;
                PC++;
                
                break;
            case 15: // NEG
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP - 1] = -stack[SP - 1];
                PC++;
                break;
            case 16: // ADD
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP - 2] = stack[SP - 1] + stack[SP - 2];
                stack[SP - 1] = 0;
                SP -= 1;
                PC++;
                break;
            case 17: // SUB
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP - 2] = stack[SP - 1] - stack[SP - 2];
                stack[SP - 1] = 0;
                SP -= 1;
                PC++;
                break;
            case 18: // MUL
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP - 2] = stack[SP - 1] * stack[SP - 2];
                stack[SP - 1] = 0;
                SP -= 1;
                PC++;
                break;
            case 19:  // DIV
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                if(stack[SP-2] == 0){
                    fprintf(stdout, "Divisor is zero in DIV instruction!\n");
                    exit(EXIT_FAILURE);
                }
                stack[SP-2] = stack[SP-1] / stack[SP-2];
                stack[SP - 1] = 0;
                SP--;
                PC++;
                break;
            case 20:  // MOD
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                if(stack[SP-2] == 0){
                    fprintf(stdout, "Modulus is zero in MOD instruction!\n");
                    exit(EXIT_FAILURE);
                }
                stack[SP-2] = stack[SP-1] % stack[SP-2];
                stack[SP - 1] = 0;
                SP--;
                PC++;
                break;
            case 21: // EQL
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP - 2] = stack[SP - 1] == stack[SP - 2] ? 1 : 0;
                stack[SP - 1] = 0;
                SP -= 1;
                PC++;
                break;
            case 22: // NEQ
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP - 2] = stack[SP - 1] != stack[SP - 2] ? 1 : 0;
                stack[SP - 1] = 0;
                SP -= 1;
                PC++;
                break;
            case 23: // LSS
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP - 2] = stack[SP - 1] < stack[SP - 2] ? 1 : 0;
                stack[SP - 1] = 0;
                SP -= 1;
                PC++;
                break;
            case 24: // LEQ
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP - 2] = stack[SP - 1] <= stack[SP - 2] ? 1 : 0;
                stack[SP - 1] = 0;
                SP -= 1;
                PC++;
                break;
            case 25: // GTR
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP - 2] = stack[SP - 1] > stack[SP - 2] ? 1 : 0;
                stack[SP - 1] = 0;
                SP -= 1;
                PC++;
                break;
            case 26: // GEQ
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP - 2] = stack[SP - 1] >= stack[SP - 2] ? 1 : 0;
                stack[SP - 1] = 0;
                SP -= 1;
                PC++;
                break;
            case 27: // PSP
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB); 
                stack[SP] = SP;
                SP += 1;
                PC++;
                break;
            default:
                printf("Error: Invalid instruction %d at PC = %d\n", code[PC].opcode, PC);
                return 1;
        }

            printPointers(BP, SP, PC, NDB);
            
        if(code[PC].opcode == 13)
            {
                printStack(stack, BP, SP, NDB);
                printInstruction(PC, code[PC].opcode, code[PC].m, NDB);
                PC++;
                printPointers(BP, SP, PC, NDB);
                printStack(stack, BP, SP, NDB);
                break;
            }
    }
    
}
