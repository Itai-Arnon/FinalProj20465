

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
	char *cString[10];/*each comma separated string inserted by index to arr*/
	int counter;
	int isError;
} sep_commas_t;

typedef struct {/*array*/
	char *opcode_name;
	op_code_t op_enum;
} opcode_arr_t;

typedef struct {/*array*/
	char *direct_name;
	directive_cmd_t cmd;
} direct_arr_t;



void parse(symbol_table_t *);


void initParser();

/* fills parser and checks  if concurs with op_code_t (1) or directive(2) (0) failuter*/
int classify_line(char *cmd);

/* creates an array of separated strings using strpbrk*/
sep_whitespace_t string_sep_white_space(char *);

void classify_opcode(symbol_table_t *sym_tbl, char *buffer);

type_of_register_t classifyRegisters(char *str, int first_or_second_operand);

/*checks type of register assigned to register is correct */
int checkRegisterCompliance();

int checkRegisterCount(op_code_t op);

/*0 - converts string to num 1- check if str is num*/
int convertOrCheckStringToNum(char *str, convert_func_t);

/*check if there's data after the current cmd*/
char *advance_buffer_if_possible(char *buffer, char *cmd);


int register_count_by_op(op_code_t op);

/* identifies  directive returns the Enum and set it in the parser*/
directive_cmd_t identifyDirective(char *str);


sep_commas_t string_comma_seps(char *str);

void initEnumArr();

void initDirectiveArray();

void printParser();
/*check complaince of line to the oper code*/






/* process .string  string  0 : non printable asciis  1: success   */
int processString( char *str, char *sArr);

/* creates an array of separated strings using strpbrk*/
sep_whitespace_t string_sep_white_space(char *);


sep_commas_t string_comma_seps(char *str);

void initEnumArr();
void initDirectiveArray();


#endif /*M14_PARSER_H*/
