#include "preassembler.h"
#include "first_pass.h"
#include "second_pass.h"
#include "output_files.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    int i;

    if (argc < 2) {
        printf("Usage: %s <source files>\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; i++) {
        printf("Processing file: %s\n", argv[i]);
        preassemble(argv[i]);  /* Creates .am */

        first_pass(argv[i]);

        if (had_errors()) {
            printf("Errors found during first pass. Skipping second pass for %s.\n", argv[i]);
            continue;
        }

        second_pass(argv[i]);

        if (!had_errors()) {
            generate_output_files(argv[i], get_ICF(), get_DCF());
            printf("All output files for %s generated successfully.\n", argv[i]);
        } else {
            printf("Errors found during second pass. Skipping output file generation for %s.\n", argv[i]);
        }
    }

    return 0;
}


