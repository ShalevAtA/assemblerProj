#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_MACROS 100
#define MAX_MACRO_LINES 50
#define MAX_LINE_LENGTH 80
struct Macro {
    char name[MAX_LINE_LENGTH];
    char lines[MAX_MACRO_LINES][MAX_LINE_LENGTH];
    int line_count;
};
typedef struct Macro Macro;
void add_macro(const char *name);
void add_macro_line(const char *name, const char *line);
int is_macro(const char *name);
void expand_macro(const char *name, FILE *output);
Macro *find_macro(const char *name);

