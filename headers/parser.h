

#ifndef M14_PARSER_H
#define M14_PARSER_H

#include "macro.h"
#include "symbols.h"
void parse(symbol_table_t*);

void initParser_T();

void classify_line (char* buffer, symbol_table_t *sym_tbl, int *pos);
#endif /*M14_PARSER_H*/
