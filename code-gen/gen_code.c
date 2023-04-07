/* $Id: gen_code_stubs.c,v 1.8 2023/03/29 15:42:08 leavens Exp leavens $ */
#include "utilities.h"
#include "gen_code.h"

// Initialize the code generator
void gen_code_initialize()
{
    // Replace the following with your implementation
    bail_with_error("gen_code_initialize not implemented yet!");
}

// Generate code for the given AST
code_seq gen_code_program(AST *prog)
{
    code_seq ret = code_seq_singleton(code_inc(2));

    ret = gen_code_block(prog);

    return ret;
}

// generate code for blk
code_seq gen_code_block(AST *blk)
{
    code_seq ret = code_seq_concat(ret, gen_code_constDecls(blk->data.program.cds));
    ret = code_seq_concat(ret, gen_code_varDecls(blk->data.program.vds));
    //ret = code_seq_concat(ret, gen_code_procDecls(blk->data.program.pds)); // ???
    ret = code_seq_concat(ret, gen_code_stmt(blk->data.program.stmt)); 
    return ret;
}

// generate code for the declarations in cds
code_seq gen_code_constDecls(AST_list cds)
{
    code_seq ret = code_seq_empty();
    while (!ast_list_is_empty(cds))
    {
        ret = code_seq_concat(ret, gen_code_constDecl(ast_list_first(cds)));

        cds = ast_list_rest(cds);
    }
    return ret;
}

// generate code for the const declaration cd
code_seq gen_code_constDecl(AST *cd)
{
    return code_seq_singleton(code_inc(1));
}

// generate code for the declarations in vds
code_seq gen_code_varDecls(AST_list vds)
{
    code_seq ret = code_seq_empty();

    while (!ast_list_is_empty(vds))
    {
        ret = code_seq_concat(ret, gen_code_varDecl(ast_list_first(vds)));
        vds = ast_list_rest(vds);
    }

    return ret;
}

// generate code for the var declaration vd
code_seq gen_code_varDecl(AST *vd)
{
    return code_seq_singleton(code_inc(1));
}

// generate code for the declarations in pds
void gen_code_procDecls(AST_list pds)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_procDecls not implemented yet!");
}

// generate code for the procedure declaration pd
void gen_code_procDecl(AST *pd)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_procDecl not implemented yet!");
}

// generate code for the statement
code_seq gen_code_stmt(AST *stmt)
{
    switch(stmt->type_tag)
    {
        case assign_ast:
            return gen_code_assignStmt(stmt);
            break;
        case call_ast:
            return gen_code_callStmt(stmt);
            break;
        case begin_ast:
            return gen_code_beginStmt(stmt);
            break;
        case if_ast:
            return gen_code_ifStmt(stmt);
            break;
        case while_ast:
            return gen_code_whileStmt(stmt);
            break;
        case read_ast:
            return gen_code_readStmt(stmt);
            break;
        case write_ast:
            return gen_code_writeStmt(stmt);
            break;
        case skip_ast:
            return gen_code_skipStmt(stmt);
            break;
        default:
            bail_with_error("Bad AST passed to gen_code_stmt!");
	        // The following should never execute
	        return code_seq_empty(); 
    }
}

// generate code for the statement
code_seq gen_code_assignStmt(AST *stmt)
{
    unsigned int outLevels = stmt->data.assign_stmt.ident->data.ident.idu->levelsOutward;

    code_seq ret = code_compute_fp(outLevels);
    ret = code_seq_concat(ret, gen_code_expr(stmt->data.assign_stmt.exp));

    unsigned int offset = stmt->data.assign_stmt.ident->data.ident.idu->attrs->loc_offset;
    ret = code_seq_add_to_end(ret, code_sto(offset));

    return ret;
}

// generate code for the statement
code_seq gen_code_callStmt(AST *stmt)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_callStmt not implemented yet!");
    return code_seq_empty();
}

// generate code for the statement
code_seq gen_code_beginStmt(AST *stmt)
{   
    code_seq ret = code_seq_singleton(code_pbp);

    // set the BP to SP-1
    ret = code_seq_add_to_end(ret, code_psp());
    ret = code_seq_add_to_end(ret, code_lit(1));
    ret = code_seq_add_to_end(ret, code_sub());
    ret = code_seq_add_to_end(ret, code_rbp());
    // allocate any declared variables
    AST_list vds = ast_list_first(stmt->data.begin_stmt.stmts); 
    int num_vds = ast_list_size(vds);
    ret = code_seq_concat(ret, gen_code_varDecls(vds));
    // add code for all the statements
    AST_list stmts = stmt->data.begin_stmt.stmts;
    while (!ast_list_is_empty(stmts)) {
	ret = code_seq_concat(ret, gen_code_stmt(ast_list_first(stmts)));
	stmts = ast_list_rest(stmts);
    }
    if (num_vds > 0) {
	// if there are variables, trim the variables
	ret = code_seq_add_to_end(ret, code_inc(- num_vds));
    }
    // restore the old BP
    ret = code_seq_add_to_end(ret, code_rbp());
    return ret;
}

// generate code for the statement
code_seq gen_code_ifStmt(AST *stmt)
{
    code_seq ret = gen_code_cond(stmt->data.if_stmt.cond);
    code_seq_add_to_end(ret, code_jpc(2));

    code_seq thenBody = gen_code_stmt(stmt->data.if_stmt.thenstmt);
    unsigned int thenSize = code_seq_size(thenBody);

    code_seq elseBody = gen_code_stmt(stmt->data.if_stmt.elsestmt);
    unsigned int elseSize = code_seq_size(elseBody);

    code_seq_add_to_end(ret, code_jpc(thenSize + 2));
    code_seq_concat(ret, thenBody);
    code_seq_add_to_end(ret, code_jmp(elseSize + 1));
    code_seq_concat(ret, elseBody);

    return ret;
}

// generate code for the statement
code_seq gen_code_whileStmt(AST *stmt)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_whileStmt not implemented yet!");
    return code_seq_empty();
}

// generate code for the statement
code_seq gen_code_readStmt(AST *stmt)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_readStmt not implemented yet!");
    return code_seq_empty();
}

// generate code for the statement
code_seq gen_code_writeStmt(AST *stmt)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_writeStmt not implemented yet!");
    return code_seq_empty();
}

// generate code for the statement
code_seq gen_code_skipStmt(AST *stmt)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_skipStmt not implemented yet!");
    return code_seq_empty();
}

// generate code for the condition
code_seq gen_code_cond(AST *cond)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_cond not implemented yet!");
    return code_seq_empty();
}

// generate code for the condition
code_seq gen_code_odd_cond(AST *cond)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_odd_cond not implemented yet!");
    return code_seq_empty();
}

// generate code for the condition
code_seq gen_code_bin_cond(AST *cond)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_bin_cond not implemented yet!");
    return code_seq_empty();
}

// generate code for the expresion
code_seq gen_code_expr(AST *exp)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_expr not implemented yet!");
    return code_seq_empty();
}

// generate code for the expression (exp)
code_seq gen_code_bin_expr(AST *exp)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_bin_expr not implemented yet!");
    return code_seq_empty();
}

// generate code for the ident expression (ident)
code_seq gen_code_ident_expr(AST *ident)
{
    // Replace the following with your implementation
    bail_with_error("gen_code_ident_expr not implemented yet!");
    return code_seq_empty();
}

// generate code for the number expression (num)
code_seq gen_code_number_expr(AST *num)
{
    return code_seq_singleton(code_lit(word2float(num->data.number.value)));
}
