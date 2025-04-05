#include "opcode_table.h"
#include <stdio.h>
#include <string.h>

#define NUM_OPCODES 16

/* Array of predefined opcodes */
static Opcode opcodes[NUM_OPCODES] = {
    {"mov", 0, 0, 2},
    {"cmp", 1, 0, 2},
    {"add", 2, 1, 2},
    {"sub", 2, 2, 2},
    {"lea", 3, 0, 2},
    {"clr", 5, 1, 1},
    {"not", 5, 2, 1},
    {"inc", 5, 3, 1},
    {"dec", 5, 4, 1},
    {"jmp", 9, 1, 1},
    {"bne", 9, 2, 1},
    {"jsr", 9, 3, 1},
    {"red", 12, 0, 1},
    {"prn", 13, 0, 1},
    {"rts", 14, 0, 0},
    {"stop", 15, 0, 0}
};

/* ---------------------------- */
/* Initialize the opcode table */
void init_opcode_table() {
    /* No dynamic allocation needed; opcodes are pre-defined */
}

/* ---------------------------- */
/* Get opcode struct by name */
const Opcode* get_opcode(const char *name) {
    int i;
    for (i = 0; i < NUM_OPCODES; i++) {
        if (strcmp(opcodes[i].name, name) == 0) {
            return &opcodes[i];
        }
    }
    return NULL;  /* Invalid opcode */
}

/* ---------------------------- */
/* Check if an opcode is valid */
int is_valid_opcode(const char *name) {
    return get_opcode(name) != NULL;
}

/* ---------------------------- */
/* Debug: Print opcode table */
void print_opcode_table() {
    int i;
    printf("----- Opcode Table -----\n");
    for (i = 0; i < NUM_OPCODES; i++) {
        printf("%s: opcode=%d, funct=%d, operands=%d\n",
            opcodes[i].name, opcodes[i].opcode, opcodes[i].funct, opcodes[i].num_operands);
    }
}

