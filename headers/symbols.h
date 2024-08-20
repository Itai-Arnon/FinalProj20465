

#ifndef M14_SYMBOLS_H
#define M14_SYMBOLS_H

#include "shared.h"
#include "symbols.h"



typedef struct symbol_t{
	char symbol_name[MAX_SYMBOL_NAME];
	unsigned short address;
	memory_t type;/*_INSTRUCTION OR DATA*/
	ARE_T  are;
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
int checkOrUpdateSymbolAddress(symbol_table_t *sym_tbl, char* symbol_name , int address , update_address_t n);

void print_symbol_table(symbol_table_t *sym_tbl);

symbol_t  *findSymbol(symbol_table_t *sym_tbl , char *symbol_name);

symbol_table_t * init_symbol_table(symbol_table_t *sym_tbl);

symbol_t *create_symbol(symbol_table_t *sym_tbl , char[] ,int ,  memory_t );


/*fills parser and checks if symbol- then returns 1  non duplicate , 2 for duplicate
 *  0 for fail ||if isStartOrMid == 1 will not allow symbol w/o ':"*/
int if_Symbol_if_Duplicate(symbol_table_t *, char * , symbol_loci_t );


void findLabel_n_load(symbol_table_t* sym_tbl, char* line , char ch);

/*-1 symbol list is empty, 0-no duplicants , 1 it is a duplicate*/
int isDuplicateSymbol(symbol_table_t *sym_tbl, char symbol_name[]);
/*check similarity of name with opcodes and directives*/
int is_symbol_name_duplicate(symbol_table_t * ,char *);

/*deletes symbol from list*/
int delete_symbol(symbol_table_t *sym_tbl , char *symbol_name);

/*adds value to memory adresses by memory type*/
void addConstantToSymbols(symbol_table_t *sym_tbl, memory_t type, int value);

 /* Option 1: Check if symbols of type ARE_T E have the same name as symbols of type ARE_T A.
 * Option 2: Check if symbols of type ARE_T E have the same name as symbols of type ARE_T R.*/
int checkExternSymbols(symbol_table_t *table, int option);
/*adds an existatn symbol to a table*/
int addSymbolToTable(symbol_table_t *table, symbol_t *_symbol);

#endif /*M14_SYMBOLS_H*/
