// Mike Burke & Hagen Farrell
// COP3402 Systems Software - Leavens
// HW 1 - Stack Machine

#include<stdlib.h>
#include<stdio.h>

#define MAX_STACK_HEIGHT 2048
#define MAX_CODE_LENGTH 512

typedef struct instruction
{
    int opcode;
    int m;
}instruction;



void initStack(int * stack);
void printCode(instruction * code, int count);
void printPointers( int BP, int SP, int PC,int NDB);
void printStack(int * stack, int BP, int SP, int NDB);
void printInstruction(int PC, int OP, int M, int NDB);



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
                stack[stack[SP - 1] + code[PC].m] = stack[SP - 2];
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

// initializing all stack values to 0
void initStack(int * stack)
{
    //all values are initilzied to 0
    for (int i = 0; i < MAX_STACK_HEIGHT; i++)
    {
        stack[i] = 0;
    }
}

// printing instruction set from file
void printCode(instruction * code, int count)
{
    int maxWidth = 8;
    char codes[][4] = 
    {
        "LIT", "RTN", "CAL", "POP", "PSI", "PRM", "STO",
        "INC", "JMP", "JPC", "CHO", "CHI", "HLT", "NDB",
        "NEG", "ADD", "SUB", "MUL", "DIV", "MOD", "EQL",
        "NEQ", "LSS", "LEQ", "GTR", "GEQ", "PSP",
    
    };
    printf("Addr  OP    M\n");
    for (int i = 0; i < count; i++ )
    {
        printf("%-6d%-6s%d\n", i, codes[code[i].opcode - 1], code[i].m);
    }
}

// printing pointers at each PC loop
void printPointers(int BP, int SP, int PC, int NDB)
{
    if( NDB == 0)
    {
        printf("PC: %d BP: %d SP: %d\n", PC, BP, SP);
    }
}
// printing stack as it is updated by instructions
void printStack(int * stack, int BP, int SP, int NDB)
{
    if(NDB == 0)
    {
        printf("stack: ");
        for (int i = BP; i < SP; i++)
        {
            printf("S[%d]: %d ", i, stack[i]);
        }
        printf("\n");
    }
}

// printing individual instruction during runtime
void printInstruction(int PC, int OP, int M, int NDB)

{
    if(NDB == 0)
    {
        switch(OP)
        {
            case 1:
                printf("==> addr: %d     LIT   %d\n", PC, M);
                break;
            case 2:
                printf("==> addr: %d     RTN   %d\n", PC, M);
                break;
            case 3:
                printf("==> addr: %d     CAL   %d\n", PC, M);
                break;
            case 4:
                printf("==> addr: %d     POP   %d\n", PC, M);
                break;
            case 5:
                printf("==> addr: %d     PSI   %d\n", PC, M);
                break;
            case 6:
                printf("==> addr: %d     PRM   %d\n", PC, M);
                break;
            case 7:
                printf("==> addr: %d     STO   %d\n", PC, M);
                break;
            case 8:
                printf("==> addr: %d     INC   %d\n", PC, M);
                break;
            case 9:
                printf("==> addr: %d     JMP   %d\n", PC, M);
                break;
            case 10:
                printf("==> addr: %d     JPC   %d\n", PC, M);
                break;
            case 11:
                printf("==> addr: %d     CHO   %d\n", PC, M);
                break;
            case 12:
                printf("==> addr: %d     CHI   %d\n", PC, M);
                break;
            case 13:
                printf("==> addr: %d     HLT   %d\n", PC, M);
                break;
            case 14:
                printf("==> addr: %d     NDB   %d\n", PC, M);
                break;
            case 15:
                printf("==> addr: %d     NEG   %d\n", PC, M);
                break;
            case 16:
                printf("==> addr: %d     ADD   %d\n", PC, M);
                break;
            case 17:
                printf("==> addr: %d     SUB   %d\n", PC, M);
                break;
            case 18:
                printf("==> addr: %d     MUL   %d\n", PC, M);
                break;
            case 19:
                printf("==> addr: %d     DIV   %d\n", PC, M);
                break;
            case 20:
                printf("==> addr: %d     MOD   %d\n", PC, M);
                break;
            case 21:
                printf("==> addr: %d     EQL   %d\n", PC, M);
                break;
            case 22:
                printf("==> addr: %d     NEQ   %d\n", PC, M);
                break;
            case 23:
                printf("==> addr: %d     LSS   %d\n", PC, M);
                break;
            case 24:
                printf("==> addr: %d     LEQ   %d\n", PC, M);
                break;
            case 25:
                printf("==> addr: %d     GTR   %d\n", PC, M);
                break;
            case 26:
                printf("==> addr: %d     GEQ   %d\n", PC, M);
                break;
            case 27:
                printf("==> addr: %d     PSP   %d\n", PC, M);
                break;
        }
    }
}
