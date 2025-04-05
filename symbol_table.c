#include "symbol_table.h"
#include <stdio.h>
#include <string.h>

static Symbol symbols[MAX_SYMBOLS];
static int symbol_count = 0;

/* ---------------------------- */
/* Initialize the symbol table */
void init_symbol_table() {
    symbol_count = 0;
}

/* ---------------------------- */
/* Reset the symbol table (clears all symbols) */
void reset_symbol_table() {
    symbol_count = 0;
}

/* ---------------------------- */
/* Add a symbol to the symbol table */
int add_symbol(const char *name, int address, SymbolType type) {
    /* Check for symbol table overflow */
    if (symbol_count >= MAX_SYMBOLS) {
        fprintf(stderr, "Symbol table overflow.\n");
        return 0;
    }

    /* Check if the label is already defined */
    if (is_label_defined(name)) {
        fprintf(stderr, "Error: Label '%s' already defined.\n", name);
        return 0;
    }

    /* Add the symbol to the table */
    strncpy(symbols[symbol_count].name, name, MAX_LABEL_LENGTH);
    symbols[symbol_count].name[MAX_LABEL_LENGTH] = '\0';  /* Ensure null-terminated */
    symbols[symbol_count].address = address;
    symbols[symbol_count].type = type;
    symbols[symbol_count].is_entry = 0;
    symbol_count++;
    return 1;
}

/* ---------------------------- */
/* Get a symbol from the symbol table by name */
Symbol* get_symbol(const char *name) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbols[i].name, name) == 0) {
            return &symbols[i];
        }
    }
    return NULL;
}

/* ---------------------------- */
/* Check if a label is already defined */
int is_label_defined(const char *name) {
    return get_symbol(name) != NULL;
}

/* ---------------------------- */
/* Mark a symbol as an entry */
void mark_symbol_as_entry(const char *name) {
    Symbol *sym;
    sym = get_symbol(name);
    if (sym) {
        sym->is_entry = 1;
    }
}

/* ---------------------------- */
/* Update addresses of data symbols after first pass */
void update_data_symbols(int icf) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (symbols[i].type == SYMBOL_DATA) {
            symbols[i].address += icf;
        }
    }
}

/* ---------------------------- */
/* Print the symbol table (for debugging purposes) */
void print_symbol_table() {
    int i;
    printf("----- Symbol Table -----\n");
    for (i = 0; i < symbol_count; i++) {
        printf("%s: %d (%s)%s\n", symbols[i].name, symbols[i].address,
            symbols[i].type == SYMBOL_CODE ? "code" :
            symbols[i].type == SYMBOL_DATA ? "data" :
            symbols[i].type == SYMBOL_EXTERNAL ? "external" : "entry",
            symbols[i].is_entry ? " [entry]" : "");
    }
}
int get_symbol_count() {
    return symbol_count;
}

Symbol* get_symbol_by_index(int index) {
    if (index >= 0 && index < symbol_count) {
        return &symbols[index];
    }
    return NULL;
}

int has_entry_labels() {
    int i;
    for (i = 0; i < get_symbol_count(); i++) {
        Symbol *sym = get_symbol_by_index(i);
        if (sym->is_entry) return 1;
    }
    return 0;
}


