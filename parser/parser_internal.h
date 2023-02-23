// header file for parser functions

#include "parser.h"

// <var-decls> ::= <float-or-bool> <idents> ;
// <float-or-bool>  ::= float | bool
static AST_list parseVarDecls();

// <idents> ::= <ident> { <comma-ident> }
static AST_list parseIdents(var_type vt);

// can this token begin a statment?
static bool is_stmt_beginning_token(token t);

// <stmt> ::= <ident> = <expr> ; | ...
AST *parseStmt();

// <assignment> ::= <ident> = <expr> ;
static AST *parseAssignStmt();

// <begin-stmt> ::= '{' <stmt> { <stmt> } '}'
static AST_list parseBeginStmt();

// <if-stmt> ::= if ( <condition> ) <stmt>
static AST *parseIfStmt();

// <read-stmt> ::= read <ident> ;
static AST *parseReadStmt();

// <write-stmt> ::= write <expr> ;
static AST *parseWriteStmt();

// Is the given token type one of those that can be used
// as a relational operator?
static bool is_relational_op(token_type tt);

// Return the corresponding operator enum for the given token type
static oper tt2oper(token_type tt);

// <lterm> ::= ! <lfactor> | <lfactor>
static AST *parseLTerm();

// <rel-op-expr> ::= <rel-op> <lterm>
// This returns an op_expr AST
static AST *parseRelOpExpr();

// <lfactor> ::= <term> { <add-sub-term> }
static AST *parseLFactor();

// <add-sub-term> ::= <add-sub> <term>
static AST *parseAddSubTerm();

// <term> ::= <factor> { <mult-div-factor> }
// Return an AST with the operators (if any) associating to the left
static AST *parseTerm();

// <mult-div-factor> ::= <mult-div> <factor>
static AST *parseMultDivFactor();

// <factor> ::= <ident> | <paren-expr> | <signed-number>
static AST *parseFactor();

// Return an AST for an identifier use
static AST *parseIdentExpr();

// Return an AST for a numeric literal
static AST *parseNumberExpr();