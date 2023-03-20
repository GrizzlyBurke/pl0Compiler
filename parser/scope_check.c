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

static void add_ident_to_scope(const char * name, file_location floc)
{
    id_attrs * attrs = scope_lookup(name)
}