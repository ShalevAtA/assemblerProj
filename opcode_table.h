

#define MAX_OPCODE_LENGTH 5
#define MAX_OPERANDS 2

typedef struct {
    char name[MAX_OPCODE_LENGTH + 1];  /* Opcode name (mov, add, etc.)*/
    int opcode;                         /* Bits 18-23*/
    int funct;                          /* Bits 7-3 (for specific instructions)*/
    int num_operands;                   /* 0, 1, or 2 operands*/
} Opcode;

void init_opcode_table();
const Opcode* get_opcode(const char *name);
int is_valid_opcode(const char *name);
void print_opcode_table();


