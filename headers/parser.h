

#ifndef M14_PARSER_H
#define M14_PARSER_H

#include "macro.h"
#include "symbols.h"
/* divides strings that have whitespace b/w them */
typedef struct  {
	char *wString[20];
	int counter;
}sep_whitespace_t;


/* divides strings that have commas b/w them */
typedef struct{
	char* cString[20];
	int counter;
	int isError;
} sep_commas_t;



void parse(symbol_table_t*);

void initParser_T();

/* fills parser and checks  if concurs with op_code (1) or directive(2) (0) failuter*/
int classify_line ( char* buffer,  int *pos);

/*fills parser and checks if concurs with symbol returns 1 for succes 0 for fail */
int ifSymbol_n_Write(symbol_table_t *sym_tbl, char *buffer, int *pos);

/* creates an array of separated strings using strpbrk*/
sep_whitespace_t string_sep_white_space(char* );

void classify_opcode(symbol_table_t *sym_tbl , char *buffer ,int  *pos);

sep_commas_t string_comma_seps(char *str);

#endif /*M14_PARSER_H*/
