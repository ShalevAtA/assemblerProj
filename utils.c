#include "utils.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/* ---------------------------- */
/* Check if the label is valid */
int is_valid_label(const char *label) {
    int i;
    
    /* Must start with a letter */
    if (!isalpha(label[0])) {
        return 0;
    }

    /* Check remaining characters */
    for (i = 1; label[i]; i++) {
        if (!isalnum(label[i])) {
            return 0;
        }
    }

    /* Check label length */
    return (strlen(label) <= MAX_LABEL_LENGTH);
}

/* ---------------------------- */
/* Trim leading and trailing spaces/tabs from a string */
void trim_whitespace(char *str) {
    char *start;
    char *end;

    start = str;

    /* Skip leading whitespace */
    while (*start && isspace(*start)) {
        start++;
    }
    memmove(str, start, strlen(start) + 1);

    end = str + strlen(str) - 1;

    /* Remove trailing whitespace */
    while (end > str && isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';
}

/* ---------------------------- */
/* Check if a string is a valid number */
int is_valid_number(const char *str) {
    /* Allow + or - at the start */
    if (*str == '+' || *str == '-') {
        str++;
    }

    /* Check if remaining characters are digits */
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

/* ---------------------------- */
/* Check if a string is a valid register (r0 - r7) */
int is_register(const char *str) {
    /* Check length and valid register format */
    if (strlen(str) == 2 && str[0] == 'r' && str[1] >= '0' && str[1] <= '7') {
        return 1;
    }
    return 0;
}

/* ---------------------------- */
/* Get register number (0-7) from a string like r1, r2, etc. */
int get_register_number(const char *reg) {
    /* Check if it's a valid register */
    if (is_register(reg)) {
        return reg[1] - '0';  /* Convert char to int (r1 → 1) */
    }
    return -1;  /* Invalid register */
}

