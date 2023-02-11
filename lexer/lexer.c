// Mike Burke and Hagen Farrell
// Homework 2: Lexical Analyzer

#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "lexer.h"
#include "token.h"
#include "utilities.h"


// global variables for each function in lexer
static const char * fname;
static const FILE * filePtr;
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
        if ( strcmp(string, resWords[i]) )
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

bool is_whitespace(char c)
{
    if(isalpha(c) || isdigit(c) || ispunct(c))
        return false;
    else
        return true;
}

// unfinished string reader
// reading string from inputStream once an new word is found
char * read_string(FILE * filePtr)
{
    char c = 'a';
    char word [50];
    int i = 0;
    while (c != " ")
    {
        c = getc(filePtr);
        if(!is_whitespace(c))
            {
                word[i] = c;
                i++;
            }
        col++;
    }
    word[i] = '\0';

    return word;

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