// main file for determining scope and populating symbol table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symbol_table.h"
#include "scope_check.h"
#include "utilities.h"

typedef struct {
    const char *id;
    id_attrs *attrs;
} symtab_assoc_t;

typedef struct scope_symtab_s {
    unsigned int size;
    symtab_assoc_t *entries[MAX_SCOPE_SIZE];
} scope_symtab_t;

static scope_symtab_t *symtab = NULL;

// Allocate a fresh scope symbol table and return it.
// Issues an error message if there is no space
// and exits with a failure error code in that case.
static scope_symtab_t * scope_create()
{
    scope_symtab_t *new_scope
	= (scope_symtab_t *) malloc(sizeof(scope_symtab_t));
    if (new_scope == NULL) {
	bail_with_error("No space for new scope_symtab_t!");
    }
    new_scope->size = 0;
    for (int j; j < MAX_SCOPE_SIZE; j++) {
	new_scope->entries[j] = NULL;
    }
    return new_scope;
}

// initialize the symbol table for the current scope
void scope_initialize()
{
    // create the scope and assign it to the global symtab
    symtab = scope_create();
}

// Return the current scope's next offset to use for allocation,
// which is the size of the current scope.
unsigned int scope_size()
{
    return symtab->size;
}

// Checks if current scope is full
bool scope_full()
{
    return scope_size() >= MAX_SCOPE_SIZE;
}

// Add an association from the given name to the given id attributes
// in the current scope.
static void scope_add(symtab_assoc_t *assoc)
{
    
    symtab->entries[symtab->size] = assoc;
    symtab->size++;
}

// Modify the current scope symbol table to
// add an association from the given name to the given id_attrs attrs.
void scope_insert(const char *name, id_attrs *attrs)
{
    symtab_assoc_t *new_assoc = malloc(sizeof(symtab_assoc_t));
    if (new_assoc == NULL) {
	bail_with_error("No space for association!");
    }
    new_assoc->id = name;
    new_assoc->attrs = attrs;
    scope_add(new_assoc);
}

// Is the given name associated with some attributes in the current scope?
bool scope_defined(const char *name)
{
    return scope_lookup(name) != NULL;
}


// Return the attributes of the given name in the current scope
// or NULL if there is no association for name.
id_attrs *scope_lookup(const char *name)
{
    int i;

    for (i = 0; i < symtab->size; i++) {
	if (strcmp(symtab->entries[i]->id, name) == 0) {
	    return symtab->entries[i]->attrs;
	}
    }
    return NULL;
}