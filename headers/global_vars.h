


#ifndef M14_GLOBAL_VARS_H
#define M14_GLOBAL_VARS_H

#include<stdio.h>
#include "shared.h"


typedef struct {
	char symbol_name[MAX_SYMBOL_NAME];
	enum {
		OP_CODE,
		DIRECTIVE,
		ERR,
		TBD
	} line_type;    /*classifies typof line*/

	/*first strct meant for directives */
	struct {
		directive_enums d_enum;
		int symbol_memory;
		struct {/*characterizes the directives*/

			char *symbol;
			int *data; /*todo:chcek if malloc*/
			char *str;
			int data_len;
		} operand; /*singular oper*/
	} directive;
	struct {
		op_code_t op; /*the 16 op codes*/
		type_of_register_t type_of_register; /*4 type by order*/
		int symbol_memory;
		union {   /* 4 types of  operands*/
			int num;/*immediate 0*/
			char *symbol;/*direct  -1 - aka memory - usually arrays & strings*/
			int registry;/* 3- indirect operand ptr to another reg , 4- direct registet */
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
