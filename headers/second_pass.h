

#ifndef M14_SECOND_PASS_H
#define M14_SECOND_PASS_H


#include "shared.h"
#include "symbols.h"
#include "first_pass.h"
#include "macro_list.h"
/*main function| checks for symbol problems and other address*/
void second_pass(macro_table_t*, symbol_table_t *, word_table_t *, word_table_t *);

void moveEntryValues(word_table_t *table , symbol_table_t *symbolTable);

/*check if there are duplicate symbols*/
void checkSymbolsUnique(macro_table_t*,  symbol_table_t *sym_table);
int convertToOctal(int num);
void printTableToFile(word_table_t *wTable ,word_table_t *dTable, char* file_name);
int missingValuesOfSymbols(symbol_table_t *table);

void checkOPCODE_INSTRUCTION(symbol_table_t *sym_tbl, word_table_t *table);
void checkOPCODE_WORDS(symbol_table_t *sym_tbl, word_table_t *table, int idx, int type);
void checkDATA_WORDS(symbol_table_t *sym_tbl, word_table_t *table);
void checkSTRING_WORDS(symbol_table_t *sym_tbl, word_table_t *table);
void checkEXTnEntry(symbol_table_t *sym_tbl, word_table_t *table);



#endif /*M14_SECOND_PASS_H*/
