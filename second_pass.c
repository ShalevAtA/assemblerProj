#include "second_pass.h"
#include "code_image.h"
#include "symbol_table.h"
#include "utils.h"
#include "opcode_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 81

/* ---------------------- */
/* Second pass over source code */
void second_pass(const char *filename) {
    char am_filename[100];
    char line[MAX_LINE_LENGTH + 2];
    FILE *file;
    int line_number = 0;
    char *token;

    sprintf(am_filename, "%s.am", filename);
    file = fopen(am_filename, "r");

    if (!file) {
        fprintf(stderr, "Error: Cannot open %s\n", am_filename);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        line_number++;

        token = strtok(line, " \t\n");
        if (!token || token[0] == ';') continue;

        /* Skip label if present */
        if (token[strlen(token) - 1] == ':') {
            token = strtok(NULL, " \t\n");
            if (!token) continue;
        }

        /* Handle directives */
        if (token[0] == '.') {
            if (strcmp(token, ".entry") == 0) {
                char *entry_label = strtok(NULL, " \t\n");
                if (!entry_label || !is_valid_label(entry_label) || !is_label_defined(entry_label)) {
                    fprintf(stderr, "Error in line %d: Invalid or undefined entry label '%s'\n", line_number, entry_label);
                    continue;
                }
                mark_symbol_as_entry(entry_label);
            }
            /* .data, .string, .extern already handled in first pass */
            continue;
        }

        /* Nothing else needed here for instructions — they're handled in resolve_operands() */
    }

    fclose(file);

    /* Resolve label references after all entries marked */
    resolve_operands(filename);

    printf("Second pass complete.\n");
}
void resolve_operands(const char *filename) {
    int i;
    Symbol *sym;
    CodeWord *cw;
    char ext_filename[100];
    FILE *ext;

    for (i = 0; i < get_code_image_size(); i++) {
        cw = get_code_word(i);

        if (cw->is_label_reference) {
            sym = get_symbol(cw->label_name);

            if (!sym) {
                fprintf(stderr, "Error: Undefined label '%s' at address %d\n", cw->label_name, cw->address);
                continue;
            }

            /* If it's external, write to .ext file */
            if (sym->type == SYMBOL_EXTERNAL) {
                cw->word = 0;
                cw->are = EXTERNAL;
                sprintf(ext_filename, "%s.ext", filename);
                ext = fopen(ext_filename, "a");
                if (ext) {
                    fprintf(ext, "%s %04d\n", cw->label_name, cw->address);
                    fclose(ext);
                }
            } else {
                cw->word = (sym->address << 3);  /*Shift value, then ARE bits*/
                cw->are = (cw->are == RELOCATABLE) ? RELOCATABLE : ABSOLUTE;
            }

            cw->is_label_reference = 0;  /*Done resolving*/
        }
    }
}



