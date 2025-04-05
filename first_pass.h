
#define MAX_LINE_LENGTH 80 
/* ---------------------------- */
/* Main first pass function */
/* Performs the first pass on the given filename (without extension) */
void first_pass(const char *filename);

/* ---------------------------- */
/* Get final IC value after the first pass */
/* ICF is the final value of the instruction counter */
int get_ICF();

/* Get final DC value after the first pass */
/* DCF is the final value of the data counter */
int get_DCF();

/* ---------------------------- */
/* Check if any errors were found in the first pass */
/* Returns 1 if errors were found, 0 otherwise */
int had_errors();




