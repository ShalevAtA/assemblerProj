#include "macro_table.h"
static Macro macros[MAX_MACROS];
static int macro_count = 0;

void add_macro(const char *name) {
    if (macro_count < MAX_MACROS) {
        strcpy(macros[macro_count].name, name);
        macros[macro_count].line_count = 0;
        macro_count++;
	    printf("Adding macro: %s\n", name);

    }
}

void add_macro_line(const char *name, const char *line) {
int i;
    for (i = 0; i < macro_count; i++) {
        if (strcmp(macros[i].name, name) == 0) {
            if (macros[i].line_count < MAX_MACRO_LINES) {
                strcpy(macros[i].lines[macros[i].line_count], line);
                macros[i].line_count++;
            }
            return;
        }
    }
}

int is_macro(const char *name) {
int i;
   printf("Checking if %s is a macro\n", name);
    for (i = 0; i < macro_count; i++) {
        if (strcmp(macros[i].name, name) == 0) {
            return 1;
        }
    }
    return 0;
}
void expand_macro(const char *name, FILE *output) {
    Macro *macro = find_macro(name);
    int i;
    if (!macro) {
        fprintf(stderr, "Error: Macro %s not found.\n", name);
        return;
    }

    printf("Expanding macro: %s\n", macro->name);  /* Debug*/

    for ( i = 0; i < macro->line_count; i++) {
        printf("Macro Line %d: %s", i, macro->lines[i]);  /*Debug*/
        fprintf(output, "%s", macro->lines[i]);  /* Write to output **without extra newlines** */
        if (macro->lines[i][strlen(macro->lines[i]) - 1] != '\n') {
            fprintf(output, "\n");  /*Add a newline only if not present*/
        }
    }
}



Macro *find_macro(const char *name) {
int i;
    for ( i = 0; i < macro_count; i++) {
        if (strcmp(macros[i].name, name) == 0) {
            return &macros[i];
        }
    }
    return NULL; /*Macro not found*/
}



