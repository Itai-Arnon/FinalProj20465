


#ifndef M14_GLOBAL_VARS_H
#define M14_GLOBAL_VARS_H

#include<stdio.h>
#include "shared.h"

/*extern macro_table_t *tbl;*/




typedef struct {
	char symbol_name[MAX_SYMBOL_NAME];
	enum {
		OP_CODE,
		DIRECTIVE,
		ERR,
		BLANK
	} line_type;    /*classifies typof line*/

	/*first strct meant for directives */
	struct {
		directive_enums d_enum;
		struct {/*characterizes the directives*/

			char *symbol;
			int *data; /*todo:chcek if malloc*/
			char *str;
			int data_len;
		} operand; /*singular oper*/
	} directive;
	struct {
		op_code op; /*the 16 op codes*/
		enum { /*characterizes the op code*/
			NO_OPER,
			NUMBER,
			LABEL,
			REGISTR
		} type;
		union {    /* 4 types of  operands*/
			char *symbol;/*direct*/
			int num;/*immediate*/
			int registry;/*register indirect-e.g pointer or register direct*/
		} operand;
	} operands[2];
} parser_t;



extern int *IC;
extern int *DC;
extern parser_t parser_s;
extern FILE *fptr_before;
extern FILE *fptr_after;
extern int line_count;

#endif /*M14_GLOBAL_VARS_H*/
