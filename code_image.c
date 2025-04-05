#include <stdio.h>            
#include <string.h>            
#include "code_image.h"   
     
static CodeWord code_image[MAX_CODE_WORDS];
static CodeWord data_image[MAX_DATA_WORDS];
static int code_image_size = 0;
static int data_image_size = 0;

void init_code_image() {
    code_image_size = 0;
    memset(code_image, 0, sizeof(code_image));
}

void add_code_word(unsigned int word, int address, AREType are) {
    if (code_image_size >= MAX_CODE_WORDS) {
        fprintf(stderr, "Code image overflow\n");
        return;
    }
    code_image[code_image_size].word = word;
    code_image[code_image_size].address = address;
    code_image[code_image_size].are = are;
    code_image[code_image_size].is_label_reference = 0;
    code_image_size++;
}

void add_label_reference(const char *label, int address, AREType are) {
    if (code_image_size >= MAX_CODE_WORDS) {
        fprintf(stderr, "Code image overflow\n");
        return;
    }
    code_image[code_image_size].word = 0; /* Placeholder */
    code_image[code_image_size].address = address;
    code_image[code_image_size].are = are;
    code_image[code_image_size].is_label_reference = 1;
    strncpy(code_image[code_image_size].label_name, label, MAX_LABEL_LENGTH);
    code_image[code_image_size].label_name[MAX_LABEL_LENGTH] = '\0';
    code_image_size++;
}

int get_code_image_size() {
    return code_image_size;
}

CodeWord* get_code_word(int index) {
    if (index >= 0 && index < code_image_size) {
        return &code_image[index];
    }
    return NULL;
}
/* -------------------- Data section -------------------- */
void add_data_word(unsigned int word, int address) {
    if (data_image_size >= MAX_DATA_WORDS) {
        fprintf(stderr, "Data image overflow\n");
        return;
    }
    data_image[data_image_size].word = word;
    data_image[data_image_size].address = address;
    data_image[data_image_size].are = ABSOLUTE;
    data_image[data_image_size].is_label_reference = 0;
    data_image_size++;
}

int get_data_image_size() {
    return data_image_size;
}

CodeWord* get_data_word(int index) {
    if (index >= 0 && index < data_image_size) {
        return &data_image[index];
    }
    return NULL;
}
int has_external_usages() {
    int i;
    for (i = 0; i < get_code_image_size(); i++) {
        CodeWord *cw = get_code_word(i);
        if (cw->are == EXTERNAL) return 1;
    }
    return 0;
}


