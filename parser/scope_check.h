#ifndef _SCOPE_CHECK_H
#define _SCOPE_CHECK_H
#include "ast.h"

// Build the symbol table for the given program AST
// and Check the given program AST for duplicate declarations
// or uses of identifiers that were not declared
extern void scope_check_program(AST *prog);

// build the symbol table and check the declarations in cds
extern void scope_check_constDecls(AST * cds);

// check the const declaration cd
// and add it to the current scope's symbol table
// or produce an error if the name has already been declare
extern void scope_check_constDecl(AST * cd);

// build the symbol table and check the declarations in vds
extern void scope_check_varDecls(AST *vds);

// check the var declaration vd
// and add it to the current scope's symbol table
// or produce an error if the name has already been declared
extern void scope_check_varDecl(AST *vd);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
extern void scope_check_stmt(AST *stmt);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
extern void scope_check_assignStmt(AST *stmt);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
extern void scope_check_beginStmt(AST *stmt);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
extern void scope_check_ifStmt(AST *stmt);

extern void scope_check_whileStmt(AST *stmt);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
extern void scope_check_readStmt(AST *stmt);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
extern void scope_check_writeStmt(AST *stmt);

// check the condition to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
extern void scope_check_cond(AST * cond);

// check the condition to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
extern void scope_check_oddCond(AST * cond);

// check the condition to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
extern void scope_check_binCond(AST * cond);

// check the expression (exp) to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
extern void scope_check_expr(AST *exp);

// check that the given name has been declared,
// if not, then produce an error using the file_location (floc) given.
extern void scope_check_ident(file_location floc, const char *name);

// check the expression (exp) to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
extern void scope_check_bin_expr(AST *exp);

// check the expression (exp) to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
extern void scope_check_op_expr(AST *exp);

#endif
