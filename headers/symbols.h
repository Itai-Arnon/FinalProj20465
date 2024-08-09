

#ifndef M14_SYMBOLS_H
#define M14_SYMBOLS_H

#include "shared.h"
#include "symbols.h"


typedef struct symbol_t{
	char symbol_name[MAX_SYMBOL_NAME];
	int address;
	memory_t type;
	struct symbol_t *next_sym;
}symbol_t;

typedef struct {
	symbol_t *symbol_List;
	int size;
}symbol_table_t;


void collect_symbol_names(symbol_table_t *sym_tbl);

int loadSymbolTable(symbol_table_t *, char [] , int , memory_t);

/*check addressYES will update NO will not
 * returns  0 if zero and 1 if diff from zero but no update 2 if diff and update*/
int checkOrUpdateSymbolAddress(symbol_table_t *sym_tbl, char* symbol_name , int address , isUpdate n);

void print_symbol_table(symbol_table_t *sym_tbl);

symbol_table_t * init_symbol_table(symbol_table_t *sym_tbl);

symbol_t *create_symbol(char symbol_name[] ,int address ,  memory_t type);

void findLabel_n_load(symbol_table_t* sym_tbl, char* line , char ch);

/*-1 symbol list is empty, 0-no duplicants , 1 it is a duplicate*/
int isDuplicateSymbol(symbol_table_t *sym_tbl, char symbol_name[]);

#endif /*M14_SYMBOLS_H*/
