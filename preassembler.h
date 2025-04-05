#include "macro_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 80
#define MAX_FILENAME_LENGTH 100
void process_file(const char *input_filename, const char *output_filename);
void get_am_filename(const char *source_filename, char *am_filename);
void preassemble(const char *filename);
