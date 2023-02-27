#include <stdio.h>
#include "lexer.h"
#include "id_attrs.h"
#include "symbol_table.h"
#include "utilities.h"
#include "parser_internal.h"

static token tok;
static unsigned int scope_offset;

//opening parser to begin lexing, parsing, checking
void parser_open()
{
    lexer_open(lexer_filename());
    tok = lexer_next();
    //scope_offset = 0;
}

// closing parser and lexer
void parser_close()
{
    lexer_close();
}

// grab next token for parsing if lexer is not done
static void advance()
{
    if(!lexer_done())
    {
        tok = lexer_next();
    }
}


// check if current token is of correct type, consume it
// advance if correct, error if false
static void eat(token_type tt)
{
    if (tok.typ == tt)
    {
        advance();
    }
    else
    {
        token_type expected[1] = {tt};
        parse_error_unexpected(expected, 1, tok);
    }
}



