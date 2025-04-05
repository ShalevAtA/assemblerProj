

#define MAX_SYMBOLS 100
#include "utils.h"
typedef enum {
    SYMBOL_CODE,
    SYMBOL_DATA,
    SYMBOL_EXTERNAL,
    SYMBOL_ENTRY
} SymbolType;

typedef struct {
    char name[MAX_LABEL_LENGTH + 1];
    int address;
    SymbolType type;
    int is_entry;
} Symbol;

void init_symbol_table();
int add_symbol(const char *name, int address, SymbolType type);
Symbol* get_symbol(const char *name);
void mark_symbol_as_entry(const char *name);
void update_data_symbols(int icf);
int is_label_defined(const char *name);
void print_symbol_table();  /* Debug*/
void reset_symbol_table();
int get_symbol_count();
Symbol* get_symbol_by_index(int index);
int has_entry_labels();




