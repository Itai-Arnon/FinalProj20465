

#ifndef M14_SYMBOLS_H
#define M14_SYMBOLS_H

#include "shared.h"
#include "symbols.h"


typedef struct symbol_t{
	char symbol_name[MAX_SYMBOL_NAME];
	int address;
	struct symbol_t *next_sym;
}symbol_t;

typedef struct {
	symbol_t *symbol_List;
	int size;
}symbol_table_t;


void collect_symbol_names(symbol_table_t *sym_tbl);

void loadSymbolTable(symbol_table_t *, char [] , int);

void print_symbol_table(symbol_table_t *sym_tbl);

symbol_table_t * init_symbol_table(symbol_table_t *sym_tbl);

symbol_t *create_symbol(char symbol_name[] ,int address);





#endif /*M14_SYMBOLS_H*/
