
#define MAX_CODE_SIZE 1000
#define MAX_CODE_WORDS 1000
#define MAX_DATA_WORDS 1000 
#include "utils.h"

typedef enum {
    ABSOLUTE = 0,
    EXTERNAL = 1,
    RELOCATABLE = 2
} AREType;

typedef struct {
    unsigned int word;  /* 24-bit word */
    int address;        /* Address in memory */
    AREType are;
    int is_label_reference;  /* 1 if needs label resolution */
    char label_name[31];     /* Used only if label operand */
} CodeWord;
int has_external_usages();
void init_code_image();
void add_code_word(unsigned int word, int address, AREType are);
void add_label_reference(const char *label, int address, AREType are);
void update_word_at(int address, unsigned int word, AREType are);
int get_code_image_size();
CodeWord* get_code_image();
CodeWord* get_code_word(int index);
/* Data image functions */
void add_data_word(unsigned int word, int address);
int get_data_image_size();
CodeWord* get_data_word(int index);



