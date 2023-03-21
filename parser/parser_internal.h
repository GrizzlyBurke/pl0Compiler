// header file for parser functions

#include "parser.h"



static void add_AST_to_end(AST_list *head, AST_list *last, AST_list lst);

// Return an AST for all const decls based off the below grammar
// ⟨const-decls⟩ ::= {⟨const-decl⟩}
static AST * parseConstDecls();

// Return an AST_list for const declarations based off the below grammar
// ⟨const-decl⟩ ::= const ⟨const-def⟩ {⟨comma-const-def⟩} ;
// ⟨const-def⟩ ::= ⟨ident⟩ = ⟨number⟩
static AST_list parseConsts();

// Return an AST for all var decls based off the below grammar
// ⟨var-decls⟩ ::= {⟨var-decl⟩}
static AST * parseVarDecls();

//Return an AST_list of identifier or variables based off below grammar
// ⟨var-decl⟩ ::= var ⟨idents⟩ ;
// ⟨comma-ident⟩ ::= , ⟨ident⟩
// ⟨idents⟩ ::= ⟨ident⟩ {⟨comma-ident⟩}
static AST_list parseIdents();

// Return AST for statments based off below grammar
// ⟨stmt⟩ ::= ⟨ident⟩ := ⟨expr⟩
// | begin ⟨stmt⟩ {⟨semi-stmt⟩} end
// | if ⟨condition⟩ then ⟨stmt⟩ else ⟨stmt⟩
// | while ⟨condition⟩ do ⟨stmt⟩
// | read ⟨ident⟩
// | write ⟨expr⟩
// | skip
AST * parseStmt();

// Return AST for assignment statement based off below grammar
// ⟨stmt⟩ ::= ⟨ident⟩ := ⟨expr⟩
static AST *parseAssignStmt();

// Return AST for begin statement based off below grammar
// begin ⟨stmt⟩ {⟨semi-stmt⟩} end
static AST_list parseBeginStmt();

// Return AST for if statement based off below grammar
// if ⟨condition⟩ then ⟨stmt⟩ else ⟨stmt⟩
static AST *parseIfStmt();

// Return AST for while statement based off below grammar
// while ⟨condition⟩ do ⟨stmt
static AST * parseWhileStmt();

// Return AST for read statement based off below grammar
// read ⟨ident⟩
static AST *parseReadStmt();

// Return AST for write statement based off below grammar
// write ⟨expr⟩
static AST *parseWriteStmt();

// Return AST for skip statement based off below grammar
// skip
static AST * parseSkipStmt();

// Return AST for condition based off below grammar
// ⟨condition⟩ ::= odd ⟨expr⟩
// | ⟨expr⟩ ⟨rel-op⟩ ⟨expr⟩
static AST * parseCondition();

// Return AST for add-subterm based of below grammar
// ⟨add-sub-term⟩ ::= ⟨add-sub⟩ ⟨term⟩
// ⟨add-sub⟩ ::= ⟨plus⟩ | ⟨minus⟩
static AST * parseAddSubTerm();

// Return AST for term based off below grammar
// ⟨term⟩ ::= ⟨factor⟩ {⟨mult-div-factor⟩}
// ⟨mult-div-factor⟩ ::= ⟨mult-div⟩ ⟨factor⟩
static AST * parseTerm();

// Return AST for factor based off below grammar
// ⟨factor⟩ ::= ⟨ident⟩ | ⟨sign⟩ ⟨number⟩ | ( ⟨expr⟩ )
static AST * parseFactor();

// parses solo identifier in expr
static AST * parseIdentExpr();

// parses solo number in expr
static AST * parseNumberExpr();

// parses possible parentheticals
static AST * parseParenExpr();

// Return mult-div factor based off below grammar
// ⟨mult-div-factor⟩ ::= ⟨mult-div⟩ ⟨factor⟩
static AST * parseMultDivFactor();
