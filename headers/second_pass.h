

#ifndef M14_SECOND_PASS_H
#define M14_SECOND_PASS_H


#include "shared.h"
#include "symbols.h"
#include "first_pass.h"
/*main function| checks for symbol problems and other address*/
void second_pass(symbol_table_t *sym_tbl, word_table_t *wordTable, word_table_t *dataTable);

void moveEntryValues(word_table_t *table , symbol_table_t *symbolTable);

/*check if there are duplicate symbols*/
void checkSymbolsUnique(symbol_table_t *sym_table)


#endif /*M14_SECOND_PASS_H*/
