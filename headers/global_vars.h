


#ifndef M14_GLOBAL_VARS_H
#define M14_GLOBAL_VARS_H

#include<stdio.h>

/*extern macro_table_t *tbl;*/


typedef enum {
	mov, cmp, add, sub, lea, clr, not, inc, dec, jmp,
	bne, red, prn, jsr, rts, stop
} op_code;

typedef struct {
	char line_err[200];
	char *label_name;
	enum {
		OP_CODE,
		DIRECT,
		ERR,
		BLANK
	} line_type;    /*classifies typof line*/

	/*first strct meant for directives */
	struct {
		enum {
			external,
			entry,
			data,
			string
		} directive_type;

		struct {
			const char *label;
			int *_data; /*todo:chcek if malloc*/
			char *_str;
			int data_len;
		} operand; /*singular oper*/
	} directive;
	struct {
		op_code n;
		enum {
			NO_OPER,
			NUMBER,
			LABEL,
			REGIS
		} type;
		union {
			char *label;
			int num;
			int regis;
		} operand;
	} operands[2];
} parser_t;

extern parser_t parser;
extern FILE *fptr_before;
extern FILE *fptr_after;
extern int line_count;

#endif /*M14_GLOBAL_VARS_H*/
