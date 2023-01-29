#include<stdio.h>
#include"utilities.h"

typedef struct instruction
{
    int opcode;
    int m;
}instruction;



void initStack(int * stack);
void printCode(instruction * code, int count);
void printPointers( int BP, int SP, int PC,int NDB);
void printStack(int * stack, int SP, int NDB);
void printInstruction(int PC, int OP, int M, int NDB);

void initStack(int * stack)
{
    //all values are initilzied to 0
    for (int i = 0; i < MAX_STACK_HEIGHT; i++)
    {
        stack[i] = 0;
    }
}

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
    printf("Addr   OP    M\n");
    for (int i = 0; i < count; i++ )
    {
        printf("%-7d%-6s%d\n", i, codes[code[i].opcode - 1], code[i].m);
    }
}

void printPointers(int BP, int SP, int PC, int NDB)
{
    if( NDB == 0)
    {
        printf("PC: %d BP: %d SP: %d\n", PC, BP, SP);
    }
}

void printStack(int * stack, int SP, int NDB)
{
    if(NDB == 0)
    {
        printf("stack: ");
        for (int i = 0; i < SP; i++)
        {
            printf("S[%d]: %d ", i, stack[i]);
        }
        printf("\n");
    }
}

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
                printf("N\n");
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