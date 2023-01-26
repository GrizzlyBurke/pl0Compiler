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
}

void init_stack(struct Stack * s)
{
    s->data = calloc(MAX_STACK_HEIGHT, sizeof(int));   
}