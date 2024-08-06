

#ifndef M14_PARSER_H
#define M14_PARSER_H

#include "macro.h"
#include "symbols.h"

/* divides strings that have whitespace b/w them */
typedef struct {
	char *wString[20];
	int counter;
} sep_whitespace_t;


/* divides strings that have commas b/w them */
typedef struct {
	char *cString[20];
	int counter;
	int isError;
} sep_commas_t;

typedef struct {
	char *opcode_name;
	op_code op_num;
} opcode_arr_t;

typedef struct {
	char *direct_name;
	directive_enums dir_num;
} direct_arr_t;

void parse(symbol_table_t *);

void initParser_T();

/* fills parser and checks  if concurs with op_code (1) or directive(2) (0) failuter*/
int classify_line( char* cmd);

/* creates an array of separated strings using strpbrk*/
sep_whitespace_t string_sep_white_space(char *);

void classify_opcode(symbol_table_t *sym_tbl, char *buffer, int *pos);

sep_commas_t string_comma_seps(char *str);

void initEnumArr();
void initDirectiveArray();


#endif /*M14_PARSER_H*/
