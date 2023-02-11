// Mike Burke and Hagen Farrell
// Homework 2: Lexical Analyzer

#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "lexer.h"
#include "token.h"
#include "utilities.h"


// global variables for each function in lexer
const char* fname;
const FILE * filePtr;
const unsigned int row, col;

int main(int argc, char * argv[])
{
    // setting global filename
    fname = argv[1];

    // opening file 
    lexer_open(fname);

    lexer_ouput();

}

void lexer_open(const char* fname)
{
    filePtr = fopen(fname, "r");
    if (filePtr == NULL)
    {

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
        lexer_close();
        return true;
    }
    else
    {
        fputc(check, filePtr);
        return false;
    }
}

// primary logic of the lexical analyzer
// checks isalpha, isdigit, ispunct with various sub cases
token lexer_next()
{
    /*
    fgetc() ->  isalpha() -> collect chars until whitespace -> isreservedword() -> tokenize
            \                                               \
            \                                               \-> isident() -> needs punct after
            \                                               \
            \                                               \-> else might be error
            \
             -> isdigit() -> collect until whitespace -> convert int -> check size from limits.h
            \
            \
            \
             -> ispunct() -> check next char with fgetc() -> if single, fputc() back into stream
                                                          \
                                                          \-> if <=, etc -> tokenize
    
    
    
    */
}

const char* lexer_filename()
{
    return fname;
}

unsigned int lexer_line()
{

}

unsigned int lexer_column()
{

}