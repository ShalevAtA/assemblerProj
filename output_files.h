#include <stdio.h>
void write_object_file(const char *filename, int ICF, int DCF);
void write_entry_file(const char *filename);
void write_extern_file(const char *filename);
void write_entry_labels(FILE *ent_file);
void write_external_usages(FILE *ext_file);
void generate_output_files(const char *filename, int icf, int dcf);


