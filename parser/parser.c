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

// <stmt> ::= <ident> = <expr> ; | ...
AST * parseStmt()
{
    switch(tok.typ)
    {
        case identsym:
            return parseAssignStmt();
            break;
        case beginsym:
            return parseBeginStmt();
            break;
        case ifsym:
            return parseIfStmt();
            break;
        case whilesym:
            return parseWhileStmt();
            break;
        case readsym:
            return pareReadStmt();
            break;
        case writesym:
            return parseWriteStmt();
            break;
        case skipsym:
            return parseSkipStmt();
            break;
        default:
            token expected [7] = {identsym, beginsym, ifsym, whilesym, readsym, writesym, skipsym};
            parse_error_unexpected(expected, 7, tok);
            break;
    }
    return (AST *) NULL;
}

// <assignment> ::= <ident> = <expr> ;
static AST * parseAssignStmt()
{
    token assignt = tok;
    eat(identsym);
    eat(becomessym);
    AST * expr = parseExpr();
    eat(semisym);
    return ast_assign_stmt(tok, tok.text, expr);
}

// <begin-stmt> ::= <stmt> { <stmt> }
static AST_list parseBeginStmt()
{
    token begint = tok;
    eat(beginsym);
    AST * s1 = parseStmt();
    AST_list stmts = ast_list_singleton(s1);
    while(tok.typ == semisym)
    {
        AST * s = parseStmt();
        ast_list_splice(stmts, ast_list_singleton(s));
    }
    eat(endsym);
    return ast_begin_stmt(begint, stmts);
}

// <if-stmt> ::= if <condition> <stmt>
static AST *parseIfStmt()
{
    token ift = tok;
    eat(ifsym);
    AST * cond = parseCondition();
    eat(thensym);
    AST * s1 = parseStmt();
    eat(elsesym);
    AST * s2 = parseStmt();

    return ast_if_stmt(ift, cond, s1, s2);

}

static AST * parseWhileStmt()
{
    token whilet = tok;
    eat(whilesym);
    AST * cond = parseCondition();
    eat(dosym);
    AST * s1 = parseStmt();

    return ast_while_stmt(whilet, cond, s1);
}

// <read-stmt> ::= read <ident> ;
static AST *parseReadStmt()
{
    token readt = tok;
    eat(readsym);
    const char * name = readt.text;
    eat(identsym);
    eat(semisym);
    return ast_read_stmt(readt, name);
}

// <write-stmt> ::= write <expr> ;
static AST *parseWriteStmt()
{
    token writet = tok;
    eat(writesym);
    AST * expr = parseExpr();
    eat(semisym);

    return ast_write_stmt(writet, expr);

}

//
static AST * parseSkipStmt()
{

}

