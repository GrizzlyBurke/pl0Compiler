#include <stdio.h>
#include<stdlib.h>

#define MAX_STACK_HEIGHT 2048
#define MAX_CODE_LENGTH 512
#define LIT 1
#define RTN 2
#define CAL 3
#define POP 4
#define PSI 5
#define PRM 6
#define STO 7
#define INC 8
#define JMP 9
#define JPC 10
#define CHO 11
#define CHI 12
#define HLT 13
#define NDB 14

int stack[MAX_STACK_HEIGHT];
int code[MAX_CODE_LENGTH];

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: Invalid number of arguments.\n");
        return 1;
    }
    char *filename = argv[1];

    // read the file and load the code into the code array
    FILE *fp = fopen(filename, "r");

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