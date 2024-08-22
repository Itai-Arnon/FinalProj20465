
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
FILE *initSourceFiles(char **_argv, FILE *fptr, int index ,int);

/* Creates a destination file & returns it file pointer. According to number of given files
 * can also change mode r/w/a */
FILE *initDestinationPointer( char **_argv, FILE *fptr, int index, char mode[] , int os);

/*add an extension to a given file name. identifies the current extension and removes it*/
char *addExtension(char *file_name, char *ext);
/*reports error to user. uses a data banks of predmade error, report in which source file the error occured|
 *choice b/w critical and non critical - critcal will exit the program and had an ability to identify error in the wordTables*/
void report_error(char *, int, file_t, err_type_t, int);

/*free word_table_t the contains the tranlation unit*/
void freeWordTable(word_table_t *table);

/*free symbols table*/
void freeSymbolTable(symbol_table_t *symbolTable);
/*free macro table*/
void freeMacroTable(macro_table_t *macroTable);

void freeAllTables(macro_table_t *macroTable, symbol_table_t *symbolTable, word_table_t *wordTable, word_table_t *dataTable);

#endif /*M14_ASSEMBLER_H*/
