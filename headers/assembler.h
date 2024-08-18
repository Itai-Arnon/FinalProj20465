
#ifndef M14_ASSEMBLER_H
#define M14_ASSEMBLER_H

#include "macro_list.h"
#include "symbols.h"
#include "first_pass.h"

/*sends files to pre processor and returns an am file*/
void manage_files(int num_files ,char ** file_list, macro_table_t*, symbol_table_t*, word_table_t *,word_table_t *dataTable);

void move_one_directory_up(char *path);
FILE *initSourceFiles(int _argc, char **_argv, FILE *fptr, int index);
FILE *initDestinationPointer(FILE *fptr, char *filename, char mode[]);

void report_error(char *err, int line_count, err_type_t type);
void freeWordTable(word_table_t *wordTable);
void freeWordTable(word_table_t *table);
void freeSymbolTable(symbol_table_t *symbolTable);
void freeMacroTable(macro_table_t *macroTable);
void freeMacroLList(macro_node_t *head);
void freeAllTables(macro_table_t *macroTable, symbol_table_t *symbolTable, word_table_t *wordTable, word_table_t *dataTable);
#endif /*M14_ASSEMBLER_H*/
