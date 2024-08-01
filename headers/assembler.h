
#ifndef M14_ASSEMBLER_H
#define M14_ASSEMBLER_H

#include "macro_list.h"
#include "symbols.h"

/*sends files to pre processor and returns an am file*/
void manage_files(int num_files ,char ** file_list, macro_table_t*, symbol_table_t*);

void move_one_directory_up(char *path);
FILE *initDestinationPointer(FILE *fptr, char *filename);
FILE *initSourceFiles(int _argc, char **_argv, FILE *fptr, int index);

#endif /*M14_ASSEMBLER_H*/
