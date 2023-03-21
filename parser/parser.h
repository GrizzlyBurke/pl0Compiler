#include "ast.h"

// externally visible parser functions

// initialize the parser to work on the given file
extern void parser_open(const char *filename);

// finish using the parser
extern void parser_close();

// Return an AST representing our pl0 program
// ⟨program⟩ ::= ⟨block⟩ .
// ⟨block⟩ ::= ⟨const-decls⟩ ⟨var-decls⟩ ⟨stmt⟩
extern AST *parseProgram();

// Return an AST using the below grammar
// ⟨stmt⟩ ::= ⟨ident⟩ := ⟨expr⟩
// | begin ⟨stmt⟩ {⟨semi-stmt⟩} end
// | if ⟨condition⟩ then ⟨stmt⟩ else ⟨stmt⟩
// | while ⟨condition⟩ do ⟨stmt⟩
// | read ⟨ident⟩
// | write ⟨expr⟩
// | skip.
extern AST *parseStmt();

// Return an AST with the operators (if any) associating to the left
// ⟨expr⟩ ::= ⟨term⟩ {⟨add-sub-term⟩}
extern AST *parseExpr();