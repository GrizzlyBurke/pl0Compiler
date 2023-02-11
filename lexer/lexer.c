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
static const FILE * filePtr;
static token new_token;
unsigned int row, col;

int main(int argc, char * argv[])
{
    // setting global filename
    fname = argv[1];

    // opening file 
    lexer_open(fname);

    lexer_ouput();

}

// checking if the string read from input is a valid reserved word
// otherwise, pass it to is_valid_ident()
bool is_reserved_word(char * string)
{
    char resWords[][10] = { "const", "var", "procedure", "call", "begin", "end", "if",
                        "then", "else", "while", "do", "read", "write", "skip", "odd"};
    
    for (int i = 0; i < 14; i++)
    {
        if (strcmp(string, resWords[i]))
        {
            return true;
        }
        else
        {
            return false;
        }
    }   
}

bool is_valid_short(unsigned int x)
{
    if ( (x >= SHRT_MIN) && (x <= SHRT_MAX) )
        return true;
    else
    {
        fprinf(stderr, "Invalid short integer size.");
        lexer_close();
        exit(1);
    }
}

void comments()
{
    char c;
    while (fgetc(filePtr) != '\n')
    {
        col++;
    }
    col = 0;
    row++;
}

char * string_builder()
{
    char c = fgetc(filePtr);
    char string[100];
    int i = 0;

    while (isalpha(c) == 0 || isdigit(c) == 0)
    {
        string[i] = c;
        col++;
        i++;
        c = fgetc(filePtr);
    }
    if(c == '\n')
    {
        row++;
        col = 0;
        string[i] = '\0';
        return string;
    }
    ungetc(c, stdin);
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
        new_token.typ = 33;
        new_token.column = lexer_column();
        new_token.line = lexer_line();
        return true;
    }
    else
    {
        unputc(check, stdin);
        return false;
    }
}

// primary logic of the lexical analyzer
// checks isalpha, isdigit, ispunct with various sub cases
token lexer_next()
{   
    char c = fgetc(filePtr);

    if(isalpha(c) == 0) 
    {
        ungetc(c, stdin);
        string_builder();


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
    }  
    if(isdigit(curr_string[0]))
    {
        short converter = (short) strtol(curr_string, NULL, 10);
        if(is_valid_short(converter))
            new_token.typ = 22;
            new_token.value = converter;
    }
    if(ispunct(curr_string[0]))
    {        
        if(strcmp(curr_string, "<>") == 0) {
            new_token.typ = 24;
        }
        else if(strcmp(curr_string, "<=") == 0) {
            new_token.typ == 26;
        }
        else if(strcmp(curr_string, ">=") == 0) {
            new_token.typ = 28;
        }
         else if(strcmp(curr_string, "<") == 0) {
            new_token.typ = 25;
        }
         else if(strcmp(curr_string, ">") == 0) {
            new_token.typ = 27;
        }
        else if(strcmp(curr_string, "=") == 0) {
            new_token.typ = 23;
        }
        else if(strcmp(curr_string, "+") == 0) {
            new_token.typ = 29;
        }
        else if(strcmp(curr_string, "-") == 0) {
            new_token.typ = 30;
        }
        else if(strcmp(curr_string, "*") == 0) {
            new_token.typ = 31;
        }
         else if(strcmp(curr_string, "/") == 0) {
            new_token.typ = 32;
        }
        else {
                fprintf(stderr, "Err: Illegal token format...\n");
        }
    }

            /*
             -> ispunct() -> check next char with fgetc() -> if single, fputc() back into stream
            \                                              \
            \                                              \-> if <=, etc -> tokenize
            \                                              \
            \                                              \-> if # -> read until newline
            \
            \
             -> is newline -> reset col count, increment row count
            */
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