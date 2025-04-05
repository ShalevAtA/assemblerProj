#include "first_pass.h"
#include "code_image.h"
#include "symbol_table.h"
#include "opcode_table.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DATA_VALUES 100

static int IC = 100;  /* Instruction Counter starts at 100 */
static int DC = 0;    /* Data Counter starts at 0 */
static int error_found = 0;

/* ---------------------------- */
/* ---------------------------- */
/* Helper: Encode .data directive */
void encode_data(char *operands) {
    char *token;
    int value;

    token = strtok(operands, ", \t\n");
    while (token != NULL) {
        trim_whitespace(token);
        if (!is_valid_number(token)) {
            fprintf(stderr, "Error: Invalid number in .data directive: %s\n", token);
            error_found = 1;
            return;
        }

        /* No need to store value for now */
	value = atoi(token);
	add_data_word(value, DC++);
        token = strtok(NULL, ", \t\n");
    }
}



/* ---------------------------- */
/* Helper: Encode .string directive */
void encode_string(char *str) {
    int i;

    if (str[0] != '"' || str[strlen(str) - 1] != '"') {
        fprintf(stderr, "Error: Invalid string format in .string directive.\n");
        error_found = 1;
        return;
    }

    /* Strip the quotes */
    str++;
    str[strlen(str) - 1] = '\0';

	for (i = 0; str[i] != '\0'; i++) {
    	add_data_word((unsigned int)str[i], DC++);
	}
	add_data_word(0, DC++);  /* Null terminator */

}

/* ---------------------------- */
/* Helper: Get addressing mode for an operand */
int get_addressing_mode(const char *operand) {
    if (!operand) return -1;

    if (operand[0] == '#') {
        return 0;  /* Immediate addressing */
    } else if (operand[0] == '&') {
        return 2;  /* Relative addressing */
    } else if (is_register(operand)) {
        return 3;  /* Direct register addressing */
    } else if (is_valid_label(operand)) {
        return 1;  /* Direct addressing */
    }

    return -1;  /* Invalid operand */
}

/* ---------------------------- */
/* Helper: Encode the first word of an instruction */
unsigned int encode_first_word(const Opcode *op, int src_mode, int src_reg, int dst_mode, int dst_reg) {
    unsigned int word;

    word = 0;
    /* Opcode (bits 18-23) */
    word |= (op->opcode << 18);

    /* Source Addressing Mode (bits 17-16) */
    word |= (src_mode << 16);

    /* Source Register (bits 15-13) */
    word |= (src_reg << 13);

    /* Destination Addressing Mode (bits 12-11) */
    word |= (dst_mode << 11);

    /* Destination Register (bits 10-8) */
    word |= (dst_reg << 8);

    /* Funct (bits 7-3) for relevant instructions */
    word |= (op->funct << 3);

    /* A R E bits (bits 2-0) - Set to 100 (Absolute) */
    word |= 4;  /* 100 in binary */

    return word;
}

/* ---------------------------- */
/* Main first pass function */
void first_pass(const char *filename) {
    char am_filename[100];
    char line[MAX_LINE_LENGTH + 2];
    FILE *file;
    int line_number;
    int has_label, L;
    char *token, *next_token, *src_operand, *dst_operand;
    int src_mode, dst_mode, src_reg, dst_reg;
    unsigned int first_word;
    const Opcode *op;

    sprintf(am_filename, "%s.am", filename);
    file = fopen(am_filename, "r");

    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", am_filename);
        return;
    }

    line_number = 0;
    init_symbol_table();  /* Reset symbol table before processing */
    init_code_image();

    while (fgets(line, sizeof(line), file)) {
	char label_name[MAX_LABEL_LENGTH] = {0};
        line_number++;
        token = strtok(line, " \t\n");
        has_label = 0;
        L = 0;

        /* Skip empty lines and comments */
        if (!token || token[0] == ';') continue;

        /* Check if token is a label */
	if (strchr(token, ':')) {
	    token[strlen(token) - 1] = '\0';  /* Remove trailing ':' */
	    if (!is_valid_label(token)) {
        	fprintf(stderr, "Error in line %d: Invalid label '%s'\n", line_number, token);
	        error_found = 1;
	        continue;
	    }
	    if (is_label_defined(token)) {
	        fprintf(stderr, "Error in line %d: Duplicate label '%s'\n", line_number, token);
	        error_found = 1;
	        continue;
	    }
	    has_label = 1;
	    strcpy(label_name, token);  /* ✅ Store actual label name */
	    next_token = strtok(NULL, " \t\n");
	    if (!next_token) continue;
	    token = next_token;  /* Use as opcode */
	}
        /* ---------------------------- */
        /* Handle Directives */
        if (token[0] == '.') {
            if (strcmp(token, ".data") == 0) {
                if (has_label) add_symbol(label_name, DC, SYMBOL_DATA);
                encode_data(strtok(NULL, "\n"));  /* Parse and encode .data */
                continue;
            } else if (strcmp(token, ".string") == 0) {
                if (has_label) add_symbol(label_name, DC, SYMBOL_DATA);
                encode_string(strtok(NULL, "\n"));  /* Parse and encode .string */
                continue;
            } else if (strcmp(token, ".extern") == 0) {
                next_token = strtok(NULL, " \t\n");
                printf("Parsed extern label: '%s'\n", next_token);
                if (!next_token || !is_valid_label(next_token)) {
                    fprintf(stderr, "Error in line %d: Invalid extern label\n", line_number);
                    error_found = 1;
                    continue;
                }
                add_symbol(next_token, 0, SYMBOL_EXTERNAL);
                continue;
            } else if (strcmp(token, ".entry") == 0) {
                /* .entry is handled in second pass, ignore here */
                continue;
            } else {
                fprintf(stderr, "Error in line %d: Unknown directive '%s'\n", line_number, token);
                error_found = 1;
                continue;
            }
        }

        /* ---------------------------- */
        /* Handle Instructions */
        op = get_opcode(token);
        if (!op) {
            fprintf(stderr, "Error in line %d: Unknown instruction '%s'\n", line_number, token);
            error_found = 1;
            continue;
        }

        if (has_label) {
            if (!add_symbol(label_name, IC, SYMBOL_CODE)) {
                fprintf(stderr, "Error in line %d: Duplicate label '%s'\n", line_number, token);
                error_found = 1;
            }
        }

        /* Parse operands */
        src_operand = strtok(NULL, ", \t\n");
        dst_operand = strtok(NULL, ", \t\n");
        src_reg = 0;
        dst_reg = 0;
	src_mode = get_addressing_mode(src_operand);
	dst_mode = get_addressing_mode(dst_operand);
	if (src_mode == 3) src_reg = get_register_number(src_operand);
	if (dst_mode == 3) dst_reg = get_register_number(dst_operand);

        L = 1;  /* Default: 1 word for first word */

        /* ---------------------------- */
        /* Check operand count */
        if (op->num_operands == 2) {
	    if (!src_operand || !dst_operand) {
        fprintf(stderr, "Error in line %d: Expected 2 operands.\n", line_number);
        error_found = 1;
        continue;
    }
	if (src_mode == 0) {  /* immediate */
	    int value = atoi(src_operand + 1); /* skip '#' */
	    unsigned int encoded = (value << 3) | ABSOLUTE;
	    add_code_word(encoded, IC + 1, ABSOLUTE);
	    L++;
	} else if (src_mode == 1 || src_mode == 2) {
	    add_label_reference(src_operand, IC + 1, (src_mode == 2) ? RELOCATABLE : ABSOLUTE);
	    L++;
	}
	
	if (dst_mode == 0) {
	    int value = atoi(dst_operand + 1);
	    unsigned int encoded = (value << 3) | ABSOLUTE;
	    int addr = IC + (src_mode != 3 ? 2 : 1);  /* depends if source needed a word */
	    add_code_word(encoded, addr, ABSOLUTE);
	    L++;
	} else if (dst_mode == 1 || dst_mode == 2) {
	    int addr = IC + (src_mode != 3 ? 2 : 1);
	    add_label_reference(dst_operand, addr, (dst_mode == 2) ? RELOCATABLE : ABSOLUTE);
	    L++;
	}

        } 
        else if (op->num_operands == 1) {
	    if (!src_operand) {
        fprintf(stderr, "Error in line %d: Expected 1 operand.\n", line_number);
        error_found = 1;
        continue;
    }
	if (dst_mode == 3) dst_reg = get_register_number(src_operand);
 		if (dst_mode == 0) {
	    int value = atoi(src_operand + 1);
	    unsigned int encoded = (value << 3) | ABSOLUTE;
	    add_code_word(encoded, IC + 1, ABSOLUTE);
	    L++;
	} else if (dst_mode == 1 || dst_mode == 2) {
	    add_label_reference(src_operand, IC + 1, (dst_mode == 2) ? RELOCATABLE : ABSOLUTE);
	    L++;
	}

        }

        /* Encode the first word */
        first_word = encode_first_word(op, src_mode, src_reg, dst_mode, dst_reg);
	add_code_word(first_word, IC, ABSOLUTE);
        printf("Encoded first word: %06X\n", first_word);  /* Debug print */

        /* Update IC by L words */
        IC += L;
    }

    fclose(file);

    /* Update data label addresses after first pass */
    update_data_symbols(IC);
    printf("First pass complete. IC=%d, DC=%d\n", IC, DC);
}

/* ---------------------------- */
/* Get final IC value (Instruction Counter Final) */
int get_ICF() {
    return IC;
}

/* Get final DC value (Data Counter Final) */
int get_DCF() {
    return DC;
}

/* Check if errors occurred during first pass */
int had_errors() {
    return error_found;
}

