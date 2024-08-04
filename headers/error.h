

#ifndef M14_ERROR_H
#define M14_ERROR_H

#include "macro_list.h"
#include "symbols.h"

void report_error(char* err , int linecount, err_type type );
void freeTables(  macro_table_t *macro_tbl , symbol_table_t *sym_tbl);


#endif
