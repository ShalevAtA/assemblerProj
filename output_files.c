#include "output_files.h"
#include "code_image.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>

/* Write the .ob file: object code and data */
void write_object_file(const char *filename, int ICF, int DCF) {
    char ob_filename[100];
    FILE *ob_file;
    int i;

    sprintf(ob_filename, "%s.ob", filename);
    ob_file = fopen(ob_filename, "w");
    if (!ob_file) {
        fprintf(stderr, "Error: Cannot open %s for writing\n", ob_filename);
        return;
    }

    /* Write header: ICF and DCF */
    fprintf(ob_file, "%d %d\n", ICF - 100, DCF);

    /* Write code image */
    for (i = 0; i < get_code_image_size(); i++) {
        CodeWord *cw = get_code_word(i);
        fprintf(ob_file, "%04d %06X\n", cw->address, cw->word);
    }

    /* Write data image (with address offset by ICF) */
    for (i = 0; i < get_data_image_size(); i++) {
        CodeWord *dw = get_data_word(i);
        fprintf(ob_file, "%04d %06X\n", dw->address + ICF, dw->word);
    }

    fclose(ob_file);
}

/* Write the .ent file: all entry labels with final addresses */
void write_entry_file(const char *filename) {
    char ent_filename[100];
    FILE *ent_file;

    sprintf(ent_filename, "%s.ent", filename);
    ent_file = fopen(ent_filename, "w");
    if (!ent_file) {
        fprintf(stderr, "Error: Cannot open %s for writing\n", ent_filename);
        return;
    }

    write_entry_labels(ent_file);
    fclose(ent_file);
}

/* Write the .ext file: all external label usages */
void write_extern_file(const char *filename) {
    char ext_filename[100];
    FILE *ext_file;

    sprintf(ext_filename, "%s.ext", filename);
    ext_file = fopen(ext_filename, "w");
    if (!ext_file) {
        fprintf(stderr, "Error: Cannot open %s for writing\n", ext_filename);
        return;
    }

    write_external_usages(ext_file);
    fclose(ext_file);
}
void write_entry_labels(FILE *ent_file) {
    int i;
    for (i = 0; i < get_symbol_count(); i++) {
        Symbol *sym = get_symbol_by_index(i);
        if (sym->is_entry) {
            fprintf(ent_file, "%s %04d\n", sym->name, sym->address);
        }
    }
}
void write_external_usages(FILE *ext_file) {
    int i;
    for (i = 0; i < get_code_image_size(); i++) {
        CodeWord *cw = get_code_word(i);
        if (cw->are == EXTERNAL) {
            fprintf(ext_file, "%s %04d\n", cw->label_name, cw->address);
        }
    }
}
void generate_output_files(const char *filename, int ICF, int DCF) {
    write_object_file(filename, ICF, DCF);

    if (has_entry_labels()) {
        write_entry_file(filename);
    }

    if (has_external_usages()) {
        write_extern_file(filename);
    }
}



