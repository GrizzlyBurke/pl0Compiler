#include "ast.h"

// externally visible parser functions

// initialize the parser to work on the given file
extern void parser_open(const char *filename);

// finish using the parser
extern void parser_close();

// <program> ::= <var-decls> <stmt>
extern AST *parseProgram();

// <stmt> ::= <ident> = <expr> ; | ...
extern AST *parseStmt();

// Return an AST with the operators (if any) associating to the left
// <expr> ::= <lterm> <rel-op-lterm>
// <rel-op-lterm> ::= <empty> | <rel=op-expr>
extern AST *parseExpr();