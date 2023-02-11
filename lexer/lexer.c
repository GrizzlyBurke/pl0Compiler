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
    char c = fgetc(filePtr);
    char * string = malloc(sizeof(char) * 50);
    int i = 0;

    while (isalpha(c) != 0 || isdigit(c) != 0)
    {
        string[i] = c;
        col++;
        i++;
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

    while (isspace(c) != 0)
    {
        if (c == '\n')
        {
            row++;
            col = 1;
        }
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
                new_token.text = "const";
        }  
        else if (strcmp(curr_string, "var") == 0) {
                new_token.typ = 4;
                new_token.text = "var";
            } 
        else if (strcmp(curr_string, "procedure") == 0) {
                new_token.typ = 5;
                new_token.text = "procedure";
            } 
        else if (strcmp(curr_string, "call") == 0) {
                new_token.typ = 7;
                new_token.text = "call";
            } 
        else if (strcmp(curr_string, "begin") == 0) {
                new_token.typ = 8;
                new_token.text = "begin";
            } 
        else if (strcmp(curr_string, "end") == 0) {
                new_token.typ = 9;
                new_token.text = "end";
            } 
        else if (strcmp(curr_string, "if") == 0) {
                new_token.typ = 10;
                new_token.text = "if";
            } 
        else if (strcmp(curr_string, "then") == 0) {
                new_token.typ = 11;
                new_token.text = "then";
            } 
        else if (strcmp(curr_string, "else") == 0) {
                new_token.typ = 12;
                new_token.text = "else";
            } 
        else if (strcmp(curr_string, "while") == 0) {
                new_token.typ = 13;
                new_token.text = "while";
            } 
        else if (strcmp(curr_string, "do") == 0) {
                new_token.typ = 14;
                new_token.text = "do";
            } 
        else if (strcmp(curr_string, "read") == 0) {
                new_token.typ = 15;
                new_token.text = "read";
            } 
        else if (strcmp(curr_string, "write") == 0) {
                new_token.typ = 16;
                new_token.text = "write";
            } 
        else if (strcmp(curr_string, "skip") == 0) {
                new_token.typ = 17;
                new_token.text = "skip";
            } 
        else if (strcmp(curr_string, "odd") == 0) {
                new_token.typ = 18;
                new_token.text = "odd";
            } 
        //if its an alpha char but not a reserved word, it must be an indentifier.
        else {
                new_token.typ = 21;
                new_token.text = NULL;
                new_token.text = realloc(new_token.text, (sizeof(char) * strlen(curr_string) + 1));
                new_token.text = curr_string;
            }
        return new_token;
    }  
    if(isdigit(c) > 0) 
    {
        ungetc(c, filePtr);
        curr_string = number_builder();
        int converter = strtol(curr_string, NULL, 10);
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
         if(c == '<') {

            c = fgetc(filePtr);

            if(c == '>') {
                new_token.typ = 24;
                new_token.text = "<>";
                col += 2;
            }
            else if(c == '=') {
                new_token.typ = 26;
                new_token.text = "<=";
                col += 2;
            }
            else {
                ungetc(c, filePtr);
                col++;
                new_token.typ = 25;
                new_token.text = "<";
            }
        }
         else if(c == '>') {
            
            c = fgetc(filePtr);

            if(c == '=') {
                new_token.typ = 28;
                new_token.text = ">=";
                col += 2;
            }
            else {
                ungetc(c, filePtr);
                col++;
                new_token.typ = 25;
                new_token.text = ">";
            }
        }
        else if(c == '.') {
            new_token.typ = 0;
            new_token.text = ".";
            col++;
        }
        else if(c == ';') {
            new_token.typ = 2;
            new_token.text = ";";
            col++;
        }
        else if(c == ',') {
            new_token.typ = 3;
            new_token.text = ",";
            col++;
        }
        else if(c == ':') {

            c = fgetc(filePtr);
            if(c == '=') {
                new_token.typ = 6;
                new_token.text = ":=";
                col += 2;
            }
            else {
                //lexical_error();
            }
        }
        else if(c == '(') {
            new_token.typ = 19;
            new_token.text = "(";
            col++;
        }  
        else if(c == ')') {
            new_token.typ = 20;
            new_token.text = ")";
            col++;
        }
        else if(c == '=') {
            new_token.typ = 23;
            new_token.text = "=";
            col++;
        }
        else if(c == '+') {
            new_token.typ = 29;
            new_token.text = "+";
            col++;
        }
        else if(c == '-') {
            new_token.typ = 30;
            new_token.text = "-";
            col++;
        }
        else if(c == '*') {
            new_token.typ = 31;
            new_token.text = "*";
            col++;
        }
        else if(c == '/') {
            new_token.typ = 32;
            new_token.text = "/";
            col++;
        }            
        else {
            fprintf(stderr, "Err: Illegal token format...\n");
        }
        new_token.column = lexer_column();
        new_token.line = lexer_line();
        new_token.filename = fname;
        return new_token;
    }
    if ((int)c == EOF)
    {
        new_token.typ = 33;
        col = 1;
        new_token.column = lexer_column();
        new_token.line = lexer_line();
        new_token.filename = fname;
        new_token.text = NULL;
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