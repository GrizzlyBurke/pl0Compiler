#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scope_check.h"
#include "id_attrs.h"
#include "file_location.h"
#include "ast.h"
#include "utilities.h"
#include "symbol_table.h"
#include "scope_check.h"

void scope_check_program(AST * prog)
{
    scope_check_constDecls(prog->data.program.cds);
    scope_check_varDecls(prog->data.program.vds);
    scope_check_stmt(prog->data.program.stmt);
}

static void add_ident_to_scope(const char * name, id_kind k, file_location floc)
{
    id_attrs * attrs = scope_lookup(name);
    if (attrs != NULL)
    {
        general_error(floc,
		      "%s \"%s\" is already declared as a %s",
		      kind2str(k), name, kind2str(attrs->kind));
    }
    scope_insert(name, create_id_attrs(floc, k, scope_size()));
}

void scope_check_constDecls(AST_list cds)
{
    while(!ast_list_is_empty(cds))
    {
        scope_check_constDecl(ast_list_first(cds));
        cds = ast_list_rest(cds);
    }
}

void scope_check_constDecl(AST *cd)
{
    const char * name = cd->data.const_decl.name;
    file_location floc = cd->file_loc;
    
    add_ident_to_scope(name, 0, floc);
    

}

void scope_check_varDecls(AST_list vds)
{
    while(!ast_list_is_empty(vds))
    {
        scope_check_varDecl(ast_list_first(vds));
        vds = ast_list_rest(vds);
    }
}

void scope_check_varDecl(AST * vd)
{
    const char * name = vd->data.var_decl.name;
    file_location floc = vd->file_loc;

    add_ident_to_scope(name, 1, floc);
}

void scope_check_stmt(AST* stmt)
{
    switch(stmt->type_tag)
    {
        case assign_ast:
            scope_check_assignStmt(stmt);
            break;
        case begin_ast:
            scope_check_beginStmt(stmt);
            break;
        case if_ast:
            scope_check_ifStmt(stmt);
            break;
        case while_ast:
            scope_check_whileStmt(stmt);
            break;
        case read_ast:
            scope_check_readStmt(stmt);
            break;
        case write_ast:
            scope_check_writeStmt(stmt);
            break;
        case skip_ast:
            break;
        default:
            bail_with_error("Call to scope_check_stmt with an AST that is not a statement!");
            break;
    }
}

void scope_check_assignStmt(AST *stmt)
{
    scope_check_ident(stmt->file_loc, stmt->data.assign_stmt.name);
    scope_check_expr(stmt->data.assign_stmt.exp);
}

void scope_check_beginStmt(AST *stmt)
{
    AST_list stmts = stmt->data.begin_stmt.stmts;

    while(!ast_list_is_empty(stmts))
    {
        scope_check_stmt(ast_list_first(stmts));
        stmts = ast_list_rest(stmts);
    }
}

void scope_check_ifStmt(AST *stmt)
{
    scope_check_cond(stmt->data.if_stmt.cond);
    scope_check_stmt(stmt->data.if_stmt.thenstmt);
    scope_check_stmt(stmt->data.if_stmt.elsestmt);
}

void scope_check_whileStmt(AST *stmt)
{
    scope_check_cond(stmt->data.while_stmt.cond);
    scope_check_stmt(stmt->data.while_stmt.stmt);
}

void scope_check_readStmt(AST *stmt)
{
    scope_check_ident(stmt->file_loc, stmt->data.read_stmt.name);
}

void scope_check_writeStmt(AST *stmt)
{
    scope_check_expr(stmt->data.write_stmt.exp);
}

void scope_check_cond(AST * cond)
{
    switch(cond->type_tag)
    {
        case odd_cond_ast:
            scope_check_oddCond(cond);
            break;
        case bin_cond_ast:
            scope_check_binCond(cond);
            break;
        default:
            bail_with_error("Unexpected type_tag (%d) in cond (for line %d, column %d)!",
                cond->type_tag, cond->file_loc.line, cond->file_loc.column);

    }
}

void scope_check_expr(AST * exp)
{
    switch(exp->type_tag)
    {
        case op_expr_ast:
            scope_check_op_expr(exp);
            break;
        case bin_expr_ast:
            scope_check_bin_expr(exp);
            break;
        case number_ast:
            break;
        case ident_ast:
            scope_check_ident(exp->file_loc, exp->data.ident.name);
            break;
        default:
            bail_with_error("Unexpected type_tag (%d) in scope_check_expr (for line %d, column %d)!",
                exp->type_tag, exp->file_loc.line, exp->file_loc.column);

    }
}

void scope_check_oddCond(AST * cond)
{
    scope_check_expr(cond->data.odd_cond.exp);
}

void scope_check_binCond(AST * cond)
{
    scope_check_expr(cond->data.bin_cond.leftexp);
    scope_check_expr(cond->data.bin_cond.rightexp);
}

void scope_check_op_expr(AST * exp)
{
    scope_check_expr(exp->data.op_expr.exp);
}

void scope_check_bin_expr(AST * exp)
{
    scope_check_expr(exp->data.bin_expr.leftexp);
    scope_check_expr(exp->data.bin_expr.rightexp);
}

void scope_check_ident(file_location floc, const char *name)
{
    if(!scope_defined(name))
    {
        general_error(floc, "identifer \"%s\" is not declared!", name);
    }
}
