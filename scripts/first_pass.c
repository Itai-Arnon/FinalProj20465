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
#include "headers/word_table.h"
int IC;
int ID;
word_table_t *wordTable;

int setOPCODE(symbol_table_t *);

line_t *create_line(int  ,symbol_t * , line_t *);

void set_opcode_into_word(word_t *word, op_code_t op_code);
/*insert operand into new empty word*/
void insert_operand_into_word(word_t *word, int value);

void set_ARE_into_word(word_t *word, ARE_T are);

void set_value_to_word(word_t *word, int value);
void pass_one(symbol_table_t *sym_tbl);

void pass_one(symbol_table_t *sym_tbl) {
	int initVal = 100;
	line_count = 0;
	IC = initVal;
	DC = 0;
	int result = 0;


	switch (parser_s.line_type) {
		case OP_CODE:
			setOPCODE(sym_tbl);


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

 IC++;
}



int setOPCODE(symbol_table_t *sym_tbl) {
	symbol_t *symbol = findSymbol(sym_tbl,parser_s.symbol_name);
	line_t *line = NULL;

 switch(register_count_by_op(parser_s.op)){
	 case 0:
		 if(symbol!=NULL)
			 symbol->address = IC;
		 line = (IC, symbol);
		 set_opcode_into_word(line->word,parser_s.op);
		 set_ARE_into_word(line->word,A);

		 break;
	 case 1:
		 break;
	 case 2:
		 break;
	 default:
		 break;
 }




}

line_t *add_line(int ic_num ,symbol_t *symbol){
	line_t *new_ptr = NULL;
	int word_t_size = sizeof(word_t );
	wordTable->size++;
	wordTable->lines;
	if(!(new_ptr = realloc(wordTable->lines,wordTable->size * sizeof(line_t)))) {
		report_error(ERR_WORD_TABLE_ALLOCATION,line_count,CRIT);
		return NULL;
	}
	if(new_ptr != wordTable->lines )
		wordTable->lines = new_ptr;

	/*data assignmet to new member */
	wordTable->lines[wordTable->size -1 ].line_num = ic_num;
	memset(wordTable->lines[wordTable->size -1 ].word, 0 ,word_t_size );
	wordTable->lines[wordTable->size -1 ].symbol = symbol;

	return &(wordTable->lines[wordTable->size -1]);
}

void initWordTable(){
	wordTable->size = 5;
	wordTable->lines = calloc(wordTable->size,sizeof(line_t));
	wordTable->next = NULL;
}


/*set the opcode into word*/
void set_opcode_into_word(word_t *word, op_code_t op_code){
	*word |= op_code<<opcode_shift;
}
/*insert operand type into instruction - otype:direct/indirect |type_of_register immdediate/direct/.. */
void insert_operand_type_into_word(word_t *word, operand_t otype, type_of_register_t type){
	set_value_to_word(word, (1<<type)<<(operand_shift+operand_size*otype));
}

/* Set ARE in first 3 bits */
void set_ARE_into_word(word_t *word, ARE_T ARE){
	set_value_to_word(word, 1<<ARE);
}
/*insert operand into new empty word*/
void insert_operand_into_word(word_t *word, int value){
	set_value_to_word(word, value<<operand_shift);
}
/*elementry handling of the shift*/
void set_value_to_word(word_t *word, int value){
	/*0x7FFF allows 15 zeroes*/
	*word |= value & 0x7FFF;
}

/*

static int get_bit(set* s, int n)
{
	int mod_n = (n % 8);
	int set_sequence = n / 8;

	return (((s->arr[set_sequence]) & (1<< mod_n) ) > 0 ? 1 : 0);
}


void print_set(set* s)
{
	int i = 0;

	if (is_empty(s) == 1)
	{
		return;
	}
	for (i = 0; i < 128; i++)
	{
		printf("bit %2d=%2d  ", i, (get_bit(s, i)));
		if (i % 20 == 0)
			printf("\n");
	}
	printf("\n");
}

*/








