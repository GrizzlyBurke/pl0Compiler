// declarations for symbol table functions
#ifndef _SCOPE_SYMTAB_H
#define _SCOPE_SYMTAB_H

#include <stdbool.h>
#include "token.h"
#include "ast.h"
#include "id_attrs.h"

// Maximum number of declarations that can be stored in a scope
#define MAX_SCOPE_SIZE 4096

// initialize the symbol table for the current scope
extern void scope_initialize();

// Return the current scope's next offset to use for allocation,
// which is the size of the current scope (number of declared ids).
extern unsigned int scope_size();

// Is the current scope full?
extern bool scope_full();

// Is the given name associated with some attributes in the current scope?
extern bool scope_defined(const char *name);

// Modify the current scope symbol table to
// add an association from the given name to the given id_attrs attrs.
extern void scope_insert(const char *name, id_attrs *attrs);

// Return the attributes of the given name in the current scope
// or NULL if there is no association for name.
extern id_attrs *scope_lookup(const char *name);

#endif