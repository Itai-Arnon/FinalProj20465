
#ifndef M14_ASSEMBLER_H
#define M14_ASSEMBLER_H

#include "macro_list.h"
#include "symbols.h"
#include "first_pass.h"

/*sends files to pre processor and returns an am file*/
void manage_files(int, char **, macro_table_t *, symbol_table_t *, word_table_t *, word_table_t *);

void move_one_directory_up(char *path);


/*returns a file ptr based and argv and argc, based on index the last param
 * require more management to increment the index*/
FILE *initSourceFiles(int _argc, char **_argv, FILE *fptr, int index ,int);

/* Creates a destination file & returns it file pointer. According to number of given files
 * can also change mode r/w/a */
FILE *initDestinationPointer(FILE *fptr, char *filename, char mode[] , int);

/*add an extension to a given file name*/
char *addExtension(char *file_name, char *ext);

void report_error(char *, int, file_t, err_type_t, int);

void freeWordTable(word_table_t *wordTable);

void freeWordTable(word_table_t *table);

void freeSymbolTable(symbol_table_t *symbolTable);

void freeMacroTable(macro_table_t *macroTable);

void
freeAllTables(macro_table_t *macroTable, symbol_table_t *symbolTable, word_table_t *wordTable, word_table_t *dataTable);

#endif /*M14_ASSEMBLER_H*/
