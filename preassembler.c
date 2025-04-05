#include "preassembler.h"
/* Function to create the output file name (adds .am extension) */
void get_am_filename(const char *source_filename, char *am_filename) {
    strcpy(am_filename, source_filename);
    strcat(am_filename, ".am");  /* Append .am to original filename */
}

/* Main preassembler function */
void preassemble(const char *filename) {
    char as_filename[MAX_FILENAME_LENGTH];
    char am_filename[MAX_FILENAME_LENGTH];

    /* Add .as extension to the input file */
    sprintf(as_filename, "%s.as", filename);
    get_am_filename(filename, am_filename);

    printf("Processing file: %s -> %s\n", as_filename, am_filename);

    /* Process the file with .as and create .am */
    process_file(as_filename, am_filename);
}

void process_file(const char *input_filename, const char *output_filename) {
    FILE *input = fopen(input_filename, "r");
    FILE *output = fopen(output_filename, "w");
    char line[MAX_LINE_LENGTH + 2];
    int in_macro = 0;
    char macro_name[MAX_LINE_LENGTH];
    char *token;
    char *next_token;
    char *rest_of_line;

    if (!input || !output) {
        fprintf(stderr, "Error opening files.\n");
        return;
    }

    while (fgets(line, sizeof(line), input)) {
        char temp_line[MAX_LINE_LENGTH + 2];
        strcpy(temp_line, line);  /* Store the full original line */
        
        token = strtok(line, " \t\n");

        /* Handle empty lines or comments */
        if (!token || token[0] == ';') {
            fprintf(output, "\n");  /* Preserve empty or comment lines */
            continue;
        }

        /* Macro definition start */
        if (strcmp(token, "mcro") == 0) {
            in_macro = 1;
            token = strtok(NULL, " \t\n");
            if (!token) {
                fprintf(stderr, "Error: Missing macro name.\n");
                continue;
            }
            strcpy(macro_name, token);
            add_macro(macro_name);
            continue;
        }

        /* End of macro definition */
        if (strcmp(token, "mcroend") == 0) {
            in_macro = 0;
            continue;
        }

        /* If inside a macro definition, store the full line */
        if (in_macro) {
            add_macro_line(macro_name, temp_line);
            continue;
        }

        /* Check if token is a label (ends with ':') */
        if (token[strlen(token) - 1] == ':') {
            /* Write the label once */
            fprintf(output, "%s ", token);

            /* Get next token after the label */
            next_token = strtok(NULL, " \t\n");

            /* Skip empty lines with only a label */
            if (!next_token) {
                fprintf(output, "\n");
                continue;
            }

            /* Get the rest of the line after the label */
            rest_of_line = strtok(NULL, "\n");

            /* Handle macros after a label */
            if (is_macro(next_token)) {
                expand_macro(next_token, output);
                continue;
            }

            /* If there’s no more content, just print the next token */
            if (!rest_of_line) {
                fprintf(output, "%s\n", next_token);
            } else {
                /* Combine the next token and the rest of the line correctly */
                fprintf(output, "%s %s\n", next_token, rest_of_line);
            }
            continue;
        }

        /* If the token is a macro, expand it */
        if (is_macro(token)) {
            expand_macro(token, output);
        } else {
            /* Write the full line correctly */
            fprintf(output, "%s", temp_line);
        }
    }

    fclose(input);
    fclose(output);
}






