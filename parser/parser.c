#include <stdio.h>
#include "lexer.h"
#include "id_attrs.h"
#include "symbol_table.h"
#include "utilities.h"
#include "parser_internal.h"

static token tok;
static unsigned int scope_offset;

//opening parser to begin lexing, parsing, checking
void parser_open(const char *filename)
{
    lexer_open(filename);
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


// ⟨program⟩ ::= ⟨block⟩ .
// ⟨block⟩ ::= ⟨const-decls⟩ ⟨var-decls⟩ ⟨stmt⟩
AST * parseProgram()
{
    // ! REVISIT
    token programt = tok;
    AST_list cds = parseConstDecls();
    AST_list vds = parseVarDecls();
    AST * stmt = parseStmt();
    eat(periodsym);

    return ast_program(lexer_filename(), 0, 0, cds, vds, stmt);
}

static void add_AST_to_end(AST_list *head, AST_list *last, AST_list lst)
{
    if (ast_list_is_empty(*head))
    {
        *head = lst;
        *last = ast_list_last_elem(lst);
    }
    else 
    {
        ast_list_splice(*last, lst);
        *last = ast_list_last_elem(lst);
    }

}

// ⟨const-decls⟩ ::= {⟨const-decl⟩}
// ⟨const-decl⟩ ::= const ⟨const-def⟩ {⟨comma-const-def⟩} ;
// ⟨const-def⟩ ::= ⟨ident⟩ = ⟨number⟩

static AST * parseConstDecls()
{
    AST_list ret = ast_list_empty_list();
    AST_list last = ast_list_empty_list();
    while (tok.typ == constsym)
    {
        eat(constsym);
        AST_list cdasts = parseConsts();
        eat(semisym);
        add_AST_to_end(&ret, &last, cdasts);

    }
    return ret;
}

static AST_list parseConsts()
{
    token consttok = tok;
    eat(identsym);
    eat(eqsym);
    eat(numbersym);
    AST_list ret = ast_list_singleton(ast_const_def(consttok, consttok.text, consttok.value));
    while (tok.typ == commasym)
    {
        eat(commasym);
        consttok = tok;
        eat(identsym);
        eat(eqsym);
        eat(numbersym);

        AST * const_ast = ast_const_def(consttok, consttok.text, consttok.value);
        ast_list_splice(const_ast, ret);
    }

    return ret;
}

// ⟨var-decls⟩ ::= {⟨var-decl⟩}
// ⟨var-decl⟩ ::= var ⟨idents⟩ ;
// ⟨comma-ident⟩ ::= , ⟨ident⟩
static AST * parseVarDecls()
{
    AST_list ret = ast_list_empty_list();
    AST_list last = ast_list_empty_list();
    while(tok.typ == varsym)
    {
        eat(varsym);
        AST_list vdasts = parseIdents();
        eat(semisym);
        add_AST_to_end(&ret, &last, vdasts);
        
    }

    return ret;
}

// ⟨idents⟩ ::= ⟨ident⟩ {⟨comma-ident⟩}
static AST_list parseIdents()
{
    token idtok = tok;
    eat(identsym);

    AST_list ret = ast_list_singleton(ast_var_decl(idtok, idtok.text)); 

    while(tok.typ == commasym)
    {
        eat(commasym);
        idtok = tok;
        eat(identsym);
        AST * ident = ast_var_decl(idtok, idtok.text);
        ast_list_splice(ident, ret);
    }
    return  ret;
}

// <stmt> ::= <ident> := <expr> ; | ...
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
            return parseReadStmt();
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

// ⟨stmt⟩ ::= ⟨ident⟩ := ⟨expr⟩
static AST * parseAssignStmt()
{
    token assignt = tok;
    eat(identsym);
    eat(becomessym);
    AST * expr = parseExpr();
    eat(semisym);
    return ast_assign_stmt(assignt, assignt.text, expr);
}

// begin ⟨stmt⟩ {⟨semi-stmt⟩} end
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

// if ⟨condition⟩ then ⟨stmt⟩ else ⟨stmt⟩
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

// while ⟨condition⟩ do ⟨stmt⟩
static AST * parseWhileStmt()
{
    token whilet = tok;
    eat(whilesym);
    AST * cond = parseCondition();
    eat(dosym);
    AST * s1 = parseStmt();

    return ast_while_stmt(whilet, cond, s1);
}

// read ⟨ident⟩
static AST *parseReadStmt()
{
    token readt = tok;
    eat(readsym);
    const char * name = readt.text;
    eat(identsym);
    return ast_read_stmt(readt, name);
}

// write ⟨expr⟩
static AST *parseWriteStmt()
{
    token writet = tok;
    eat(writesym);
    AST * expr = parseExpr();

    return ast_write_stmt(writet, expr);

}

// skip
static AST * parseSkipStmt()
{
    token skipt = tok;
    eat(skipsym);

    return ast_skip_stmt(skipt);
}

static AST * parseCondition()
{
    token cond = tok;
    if(tok.typ == oddsym) {
        eat(oddsym);
        AST * exp = parseExpr();
         return ast_odd_cond(cond, exp);
    } else {
        AST * exp1 = parseExpr();
        rel_op rel;
        switch(tok.typ)
        {
            case eqsym:
                rel = eqop;
            break;
            case neqsym:
                rel = neqop;
            break;
            case lessym:
                rel = ltop;
            break;
            case leqsym:
                rel = leqop;
            break;
            case gtrsym:
                rel = gtop;
            break;
            case geqsym:
                rel = geqop;
            break;
            default:
                //error
            break;
        }
        AST * exp2 = parseExpr();
        return ast_bin_cond(cond, exp1, rel, exp2);
    }
}

/*



*/


AST * parseExpr()
{
    token fst = tok;
    AST * exp = parseTerm();
    while(tok.typ == plussym || tok.typ == minussym)
    {
        AST * rght = parseAddSubTerm();
        exp = ast_bin_expr(fst, exp, rght->data.op_expr.arith_op,
                rght->data.op_expr.exp);
    }
    return exp;
}

static AST * parseAddSubTerm()
{
    token opt = tok;
    switch(tok.typ)
    {
        case plussym:
            eat(plussym);
            AST * exp = parseTerm();
            return ast_op_expr(opt, addop, exp);
        break;
        case minussym:
            eat(minussym);
            AST * e = parseTerm();
            return ast_op_expr(opt, subop, exp);
        break;
        default:
            token_type expected[2] = {plussym, minussym};
            parse_error_unexpected(expected, 2, tok);
        break;
    }
    return (AST *) NULL;
}

static AST * parseTerm()
{
    token fst = tok;
    AST * fac = parseFactor();
    AST * exp = fac;
    while(tok.typ == multsym || tok.typ == divsym)
    {
        AST * rght = parseMultDivFactor();
        exp = ast_bin_expr(fst, exp, rght->data.op_expr.arith_op,
                rght->data.op_expr.exp);
    }
    return exp;
}

//this is setup for me to have a path to work on.

static AST * parseFactor()
{
    switch(tok.typ)
    {
        case identsym:
            return parseIdentExpr();
        break;
        case lparensym:
            return parseParenExpr();
        break;
        case numbersym:
            return parseNumberExpr();
        break;
        //we need a case for multiple expressions.
        default:
        break;
    }
}

static AST * parseIdentExpr()
{
    token idt = tok;
    eat(identsym);
    return ast_ident(idt, idt.text);
}

static AST * parseNumberExpr()
{
    token numbt = tok;
    eat(numbersym);
    double val = numbt.value;
    return ast_number(numbt, val);
}

static AST * parseParenExpr()
{
    token lpt = tok;
    eat(lparensym);
    AST * ret = parseExpr();
    eat(rparensym);
    ret->file_loc = token2file_loc(lpt);
    return ret;
}

static AST * parseMultDivFactor()
{
    token opt = tok;
    switch(tok.typ)
    {
        case multsym:
            eat(multsym);
            AST * exp = parseFactor();
            return ast_op_expr(opt, multop, exp);
        break;
        case divsym:
            eat(divsym);
            AST * e = parseFactor();
            return ast_op_expr(opt, divop, e);
        break;
        default:
            token_type expected[2] = {multsym, divsym};
            parse_error_unexpected(expected, 2, tok);
        break;
    }
    return (AST *) NULL;
}