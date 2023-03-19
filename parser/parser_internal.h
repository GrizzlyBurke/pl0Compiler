// header file for parser functions

#include "parser.h"

// <var-decls> ::= <float-or-bool> <idents> ;
// <float-or-bool>  ::= float | bool
static AST_list parseVarDecls();


static AST_list parseIdents();

static void add_AST_to_end(AST_list *head, AST_list *last, AST_list lst);


// <stmt> ::= <ident> = <expr> ; | ...
AST * parseStmt();

static AST * parseConstDecls();

static AST_list parseConsts();

// <assignment> ::= <ident> = <expr> ;
static AST *parseAssignStmt();

// <begin-stmt> ::= '{' <stmt> { <stmt> } '}'
static AST_list parseBeginStmt();

// <if-stmt> ::= if ( <condition> ) <stmt>
static AST *parseIfStmt();

// <while-stmt> ::=
static AST * parseWhileStmt();

// <read-stmt> ::= read <ident> ;
static AST *parseReadStmt();

// <write-stmt> ::= write <expr> ;
static AST *parseWriteStmt();

static AST * parseSkipStmt();

static AST * parseCondition();

static AST * parseAddSubTerm();

static AST * parseTerm();

static AST * parseFactor();

static AST * parseIdentExpr();

static AST * parseNumberExpr();

static AST * parseParenExpr();

static AST * parseMultDivFactor();
