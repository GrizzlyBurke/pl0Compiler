// Mike Burke and Hagen Farrell
// Homework 2: Lexical Analyzer

#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include<string.h>
#include<stdlib.h>
#include "lexer.h"
#include "token.h"
#include "lexer_output.h"
#include "utilities.h"


// global variables for each function in lexer
static const char * fname;
static FILE * filePtr;
static token new_token;
unsigned int row = 1; 
unsigned int col = 1;

int main(int argc, char * argv[])
{
    // setting global filename
    fname = argv[1];

    // opening file 
    
    lexer_open(fname);
    lexer_output();
    lexer_close();

}

bool is_valid_short(int x)
{
    if ( (x >= SHRT_MIN) && (x <= SHRT_MAX) )
        return true;
    else
    {
        fprintf(stderr, "Invalid short integer size.\n");
        lexer_close();
        exit(1);
    }
}

char comments()
{
    char c = fgetc(filePtr);
    while (c != '\n')
        {
            c = fgetc(filePtr);
        }
        row++;
        col = 1;
    return fgetc(filePtr);
}

char * string_builder()
{
    printf("Hello!\n");
    char c = fgetc(filePtr);
    char * string = malloc(sizeof(char) * 50);
    int i = 0;

    while (isalpha(c) != 0 || isdigit(c) != 0)
    {
        string[i] = c;
        col++;
        i++;
        printf("i get here\n");
        c = fgetc(filePtr);
        
    }
    if(c == '\n')
    {
        row++;
        col = 1;
        string[i] = '\0';
        
        return string;
    }
    ungetc(c, filePtr);
    col--;
    string[i] = '\0';

    return string;
}

void lexer_open(const char* fname)
{
    filePtr = fopen(fname, "r");
    if (filePtr == NULL)
    {
        fprintf(stderr, "File Pointer Error\n");
        exit(1);
    }
}

// closing filePtr when lexer_done() is true
void lexer_close()
{
    fclose(filePtr);
}

bool lexer_done()
{
    // checking file stream for end or null

    char check = fgetc(filePtr);
    if ( check == EOF || filePtr == NULL )
    {
        return true;
    }
    else
    {
        ungetc(check, filePtr);
        return false;
    }
}

char * number_builder()
{
    int i = 0;
    char c = fgetc(filePtr);
    char * string = malloc(sizeof(char) * 50);

    while(isdigit(c) > 0)
    {
        string[i] = c;
        i++;
        col++;
        c = fgetc(filePtr);
    }

    if(c == '\n')
    {
        row++;
        col = 1;
        string[i] = '\0';
        return string;
    }

    ungetc(c, filePtr);
    col--;
    string[i] = '\0';

    return string;
}

// primary logic of the lexical analyzer
// checks isalpha, isdigit, ispunct with various sub cases
token lexer_next()
{   
    char c = fgetc(filePtr);
    char * curr_string;
   
   
    while (c == ' ')
    {
        c = fgetc(filePtr);
        col++;
    }
    while (c == '#')
    {
        c = comments();
    }
    if(isalpha(c) != 0) 
    {
        ungetc(c, filePtr);

        new_token.column = lexer_column();
        new_token.line = lexer_line();
        new_token.filename = fname;

        curr_string = string_builder();

        if (strcmp(curr_string, "const") == 0) { 
                new_token.typ = 1;
        }  
        else if (strcmp(curr_string, "var") == 0) {
                new_token.typ = 4;
            } 
        else if (strcmp(curr_string, "procedure") == 0) {
                new_token.typ = 5;
            } 
        else if (strcmp(curr_string, "call") == 0) {
                new_token.typ = 7;
            } 
        else if (strcmp(curr_string, "begin") == 0) {
                new_token.typ = 8;
            } 
        else if (strcmp(curr_string, "end") == 0) {
                new_token.typ = 9;
            } 
        else if (strcmp(curr_string, "if") == 0) {
                new_token.typ = 10;
            } 
        else if (strcmp(curr_string, "then") == 0) {
                new_token.typ = 11;
            } 
        else if (strcmp(curr_string, "else") == 0) {
                new_token.typ = 12;
            } 
        else if (strcmp(curr_string, "while") == 0) {
                new_token.typ = 13;
            } 
        else if (strcmp(curr_string, "do") == 0) {
                new_token.typ = 14;
            } 
        else if (strcmp(curr_string, "read") == 0) {
                new_token.typ = 15;
            } 
        else if (strcmp(curr_string, "write") == 0) {
                new_token.typ = 16;
            } 
        else if (strcmp(curr_string, "skip") == 0) {
                new_token.typ = 17;
            } 
        else if (strcmp(curr_string, "odd") == 0) {
                new_token.typ = 18;
            } 
        //if its an alpha char but not a reserved word, it must be an indentifier.
        else {
                new_token.typ = 21;
                new_token.text = realloc(new_token.text, (sizeof(char) * strlen(curr_string) + 1));
                strcpy(new_token.text, curr_string);
            }
        return new_token;
    }  
    if(isdigit(c) > 0) 
    {
        ungetc(c, filePtr);
        curr_string = number_builder();
        int converter = strtol(curr_string, NULL, 10);
        //printf("Converter: %d", converter);
        if(is_valid_short(converter))
        {
            new_token.typ = 22;
            new_token.value = converter;
            new_token.column = lexer_column();
            new_token.line = lexer_line();
            new_token.filename = fname;
            return new_token;
        }
    }
    if(ispunct(c) != 0)
    {       
        //ungetc(c, filePtr);
        new_token.column = lexer_column();
        new_token.line = lexer_line();
        new_token.filename = fname;

         if(c == '<') {

            c = fgetc(filePtr);

            if(c == '>') {
                new_token.typ = 24;
                col += 2;
            }
            else if(c == '=') {
                new_token.typ = 26;
                col += 2;
            }
            else {
                ungetc(c, filePtr);
                col++;
                new_token.typ = 25;
            }
        }
         else if(c == '>') {
            
            c = fgetc(filePtr);

            if(c == '=') {
                new_token.typ = 28;
                col += 2;
            }
            else {
                ungetc(c, filePtr);
                col++;
                new_token.typ = 25;
            }
        }
        else if(c == '.') {
            new_token.typ = 0;
            col++;
        }
        else if(c == ';') {
            new_token.typ = 2;
            col++;
        }
        else if(c == ',') {
            new_token.typ = 3;
            col++;
        }
        else if(c == ':') {

            c = fgetc(filePtr);
            if(c == '=') {
                new_token.typ = 6;
                col += 2;
            }
            else {
                //lexical_error();
            }
        }
        else if(c == '(') {
            new_token.typ = 19;
            col++;
        }  
        else if(c == ')') {
            new_token.typ = 20;
            col++;
        }
        else if(c == '=') {
            new_token.typ = 23;
            col++;
        }
        else if(c == '+') {
            new_token.typ = 29;
            col++;
        }
        else if(c == '-') {
            new_token.typ = 30;
            col++;
        }
        else if(c == '*') {
            new_token.typ = 31;
            col++;
        }
        else if(c == '/') {
            new_token.typ = 32;
            col++;
        }            
        else {
            fprintf(stderr, "Err: Illegal token format...\n");
        }
        return new_token;
    }
    return new_token;
}

const char* lexer_filename()
{
    return fname;
}

unsigned int lexer_line()
{
    return row;
}

unsigned int lexer_column()
{
    return col;
}