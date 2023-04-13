/* $Id: gen_code_stubs.c,v 1.8 2023/03/29 15:42:08 leavens Exp leavens $ */
#include "utilities.h"
#include "gen_code.h"

// Initialize the code generator
void gen_code_initialize()
{
    // Replace the following with your implementation
    //bail_with_error("gen_code_initialize not implemented yet!");
}

// Generate code for the given AST
code_seq gen_code_program(AST *prog)
{
    code_seq ret = code_seq_singleton(code_inc(3));

    ret = code_seq_concat(ret, gen_code_block(prog));

    ret = code_seq_add_to_end(ret, code_hlt());

    return ret;
}

// generate code for blk
code_seq gen_code_block(AST *blk)
{
    code_seq ret = code_seq_singleton(gen_code_constDecls(blk->data.program.cds));
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
    ret = code_seq_add_to_end(ret, code_rtn());
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
    ret = code_seq_add_to_end(ret, code_rtn());
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
    // while condition
    // do this
    // otherwise do this

    //gen seq for condition
    //JPC around failing (+2)
    //JMP to end past last JMP (bodysize + 2)
    //gen code stmt
    //JMP to condition (-(bodysize + condsize +2))

    code_seq ret = gen_code_cond(stmt->data.while_stmt.cond);
    unsigned int condSize = code_seq_size(ret);
    code_seq_add_to_end(ret, code_jpc(2));
    
    code_seq body = gen_code_stmt(stmt->data.while_stmt.stmt);
    unsigned int bodySize = code_seq_size(body);

    code_seq_add_to_end(ret, code_jmp(bodySize + 2));
    ret = code_seq_concat(ret, body);
    code_seq_add_to_end(ret, code_jmp(-(bodySize + condSize + 2)));
    
    return ret;
}

// generate code for the statement
code_seq gen_code_readStmt(AST *stmt)
{
    id_use *idu = stmt->data.read_stmt.ident->data.ident.idu;
    code_seq ret = code_compute_fp(idu->levelsOutward);
    ret = code_seq_add_to_end(ret, code_chi());
    ret = code_seq_add_to_end(ret, code_sto(idu->attrs->loc_offset));

    return ret;
}

// generate code for the statement
code_seq gen_code_writeStmt(AST *stmt)
{
    code_seq ret = gen_code_expr(stmt->data.write_stmt.exp);
    ret = code_seq_add_to_end(ret, code_cho());
}

// generate code for the statement
code_seq gen_code_skipStmt(AST *stmt)
{
    code_seq ret = code_seq_singleton(code_nop());

    return ret;
}

// generate code for the condition
code_seq gen_code_cond(AST *cond)
{
    switch (cond->type_tag)
    {
        case odd_cond_ast:
            return gen_code_odd_cond(cond);
            break;
        case bin_cond_ast:
            return gen_code_bin_cond(cond);
            break;
        default:
            bail_with_error("gen_code_cond passed bad AST!");
            return code_seq_empty();
            break;
    }
}

// generate code for the condition
code_seq gen_code_odd_cond(AST *cond)
{
    // expr to stack
    // mod

    code_seq ret = gen_code_expr(cond->data.odd_cond.exp);
    code_seq_concat(ret, code_mod());

    return ret;
}

// generate code for the condition
code_seq gen_code_bin_cond(AST *cond)
{
    // left expr to stack
    // right expr to stack
    // relation op

    code_seq ret = gen_code_expr(cond->data.bin_cond.leftexp);
    ret = code_seq_concat(ret, gen_code_expr(cond->data.bin_cond.rightexp));

    switch(cond->data.bin_cond.relop)
    {
        case 0: // =
            return code_seq_add_to_end(ret, code_eql());
            break;
        case 1: // <>
            return code_seq_add_to_end(ret, code_neq());
            break;
        case 2: // <
            return code_seq_add_to_end(ret, code_lss());
            break;
        case 3:// <=
            return code_seq_add_to_end(ret, code_leq());
            break;
        case 4: // >
            return code_seq_add_to_end(ret, code_gtr());
            break;
        case 5: // >=
            return code_seq_add_to_end(ret, code_geq());
            break;
        default:
            bail_with_error("gen_code_bin_cond passed AST with bad relop!");
            return code_seq_empty(); 
    }
}

// generate code for the expresion
code_seq gen_code_expr(AST *exp)
{
    switch(exp->type_tag)
    {
        case bin_expr_ast:
            return gen_code_bin_expr(exp);
            break;
        case ident_ast:
            return gen_code_ident_expr(exp);
            break;
        case number_ast:
            return gen_code_number_expr(exp);
            break;
        default:
            bail_with_error("gen_code_expr passed bad AST!");
            break;
    }
}

// generate code for the expression (exp)
code_seq gen_code_bin_expr(AST *exp)
{
    // left exp to stack
    // right exp to stack
    // operation code

    code_seq ret = gen_code_expr(exp->data.bin_expr.leftexp);
    ret = code_seq_concat(ret, gen_code_expr(exp->data.bin_expr.rightexp));
    switch(exp->data.bin_expr.arith_op)
    {
        case 0:
            return code_seq_add_to_end(ret, code_add());
            break;
        case 1:
            return code_seq_add_to_end(ret, code_sub());
            break;
        case 2:
            return code_seq_add_to_end(ret, code_mul());
            break;
        case 3:
            return code_seq_add_to_end(ret, code_div());
            break;
        default:
            bail_with_error("gen_code_bin_expr passed AST with bad op!");
            return code_seq_empty();
    }
}

// generate code for the ident expression (ident)
code_seq gen_code_ident_expr(AST *ident)
{
    // load fp
    // load offset to var

    id_use *idu = ident->data.ident.idu;
    lexical_address *la = lexical_address_create(idu->levelsOutward, idu->attrs->loc_offset);

    return code_load_from_lexical_address(la);
}

// generate code for the number expression (num)
code_seq gen_code_number_expr(AST *num)
{
    return code_seq_singleton(code_lit(num->data.number.value));
}
