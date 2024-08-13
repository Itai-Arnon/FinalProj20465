#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>
#include "headers/macro.h"
#include "headers/macro_list.h"
#include "headers/shared.h"
#include "headers/global_vars.h"
#include "headers/utils.h"
#include "headers/symbols.h"
#include "headers/parser.h"
#include "headers/error.h"
#include "headers/first_pass.h"
int *IC;
int *ID;

int setOPCODE(symbol_table_t *, int);

line_t *create_line(int  ,symbol_t * , line_t *);

void set_opcode_into_word(word_t *word, op_code_t op_code);
void set_ARE_into_word(word_t *word, ARE are);

void insert_operand_into_word(word_t *word, int value);
void set_value_to_word(word_t *word, int value);
void pass_one(symbol_table_t *sym_tbl);

void pass_one(symbol_table_t *sym_tbl) {
	int initVal = 100;
	line_count = 0;
	IC = &initVal;
	int result = 0;


	switch (parser_s.line_type) {
		case OP_CODE:
			setOPCODE(IC);


		case DIRECTIVE:
			if ((result = parser_s.directive.cmd) == DATA) {


			} else if (result == STRING) {

			} else if (result == ENTRY || result == EXTERN) {}

		case ERR:
			report_error(ERR_LINE_UNRECOGNIZED, line_count, CRIT);
			break;
		case TBD:
			report_error(ERR_DIRECTIVE_RECOGNITION, line_count, CRIT);
			break;
		default:
			break;
	}


}

int setOPCODE(symbol_table_t *sym_tbl, int address) {
	symbol_t *symbol = findSymbol(sym_tbl,parser_s.symbol_name);

	line_t *line = create_line(*IC,symbol,NULL);


}

line_t *create_line(int line_num ,symbol_t *symbol , line_t *next){
	line_t *node = NULL;


	if (node = malloc(sizeof(line_t))) {
		node->word = 0;
		node->line_num = line_num;
		node->next = next;
		return node;
	}
	return NULL;

}
/*set the opcode into word*/
void set_opcode_into_word(word_t *word, op_code_t op_code){
	*word |= op_code<<opcode_shift;
}
/*insert operand type into instruction - otype:direct/indirect |type_of_register immdediate/direct/.. */
void insert_operand_type_into_word(word_t *word, operand_t otype, type_of_register_t type){
	set_value_to_word(word, (1<<type)<<(operand_shift+operand_size*otype));
}

void set_ARE_into_word(word_t *word, ARE are){
	set_value_to_word(word, 1<<are);
}
/*insert operand into new empty word*/
void insert_operand_into_word(word_t *word, int value){
	set_value_to_word(word, value<<operand_shift);
}
/*elementry handling of the shift*/
void set_value_to_word(word_t *word, int value){
	*word |= value & 0x7FFF;
}












