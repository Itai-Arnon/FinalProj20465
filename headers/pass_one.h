

#ifndef M14_PASS_ONE_H
#define M14_PASS_ONE_H

#include "shared.h"
#include "pass_one.h"


typedef struct symbol_t{
	char symbol_name[MAX_SYMBOL_NAME];
	int address;
	struct symbol_t *next_sym;
}symbol_t;

typedef struct {
	symbol_t *symbol_List;
	int size;
}symbol_table_t;

symbol_table_t * init_symbol_list(symbol_table_t *sym_tbl);

symbol_t *create_symbol_node(char symbol_name[] ,int address);

void collect_symbol_names(symbol_table_t *sym_tbl);

void loadSymbolTable(symbol_table_t *, char [] , int);



#endif /*M14_PASS_ONE_H*/
