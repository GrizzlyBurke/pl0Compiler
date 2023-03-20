// main runner functions for parsing and declaration checking

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "lexer_output.h"
#include "parser.h"
#include "ast.h"
#include "utilities.h"
#include "unparser.h"
#include "symbol_table.h"
#include "scope_check.h"

int main(int argc, char * argv[])
{
    parser_open(argv[1]);
    AST * progast = parseProgram();
    parser_close();
    unparseProgram(stdout, progast);

    //scope_initialize();
    //scope_check_program(progast);
}