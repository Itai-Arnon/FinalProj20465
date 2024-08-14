


#ifndef M14_GLOBAL_VARS_H
#define M14_GLOBAL_VARS_H

#include<stdio.h>
#include "shared.h"
#include "word_table.h"


typedef struct {
	char symbol_name[MAX_SYMBOL_NAME];
	enum {
		OP_CODE,
		DIRECTIVE,
		ERR,
		TBD
	} line_type;    /*classifies typof line*/
	op_code_t op; /*the 16 op codes*/
	/*first strct meant for directives */
	struct {
		directive_cmd_t cmd;
		int symbol_memory;
		struct {/*characterizes the directives*/
			char *symbol;
			int *data; /*todo:chcek if malloc*/
			char *str;
			int data_len;
		} operand; /*singular oper*/
	} directive;
	struct {

		type_of_register_t type_of_register; /*4 type by order*/
		int symbol_memory;
		union {   /* 4 types of  operands*/
			int num;/*immediate 0*/
			char symbol[MAX_SYMBOL_NAME];/*direct   - aka memory - usually  alphanumeric string */
			int registry;/* 3- indirect operand ptr to another reg , 4- direct registet */
		} operand;
	} operands[2];
} parser_t;

extern char *opcode_names[16];
extern char *directives[4];
 /*extern char *directives[4] = {".data", ".string", ".extern", ".entry"};*/

extern word_table_t *wordTable;
extern int IC;
extern int DC;
extern parser_t parser_s;
extern FILE *fptr_before;
extern FILE *fptr_after;
extern int line_count;

#endif /*M14_GLOBAL_VARS_H*/
