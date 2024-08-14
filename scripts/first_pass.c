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
word_table_t *dataTable;


void pass_one(symbol_table_t *sym_tbl) {
	int initVal = 100;
	int result = 0;

	line_count = 0;
	IC = initVal;
	DC = 0;

	wordTable = initTable(wordTable);
	dataTable = initTable(dataTable);


	switch (parser.line_type) {
		case OP_CODE:
			setOPCODE_INSTRUCTION(sym_tbl);

		case DIRECTIVE:
			if ((result = parser.directive.cmd) == DATA) {


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
void set_number_data_word(word_t *word,  int value ){
	value = convertToTwoComp(value);
	set_value_to_word(word,value);
}

void set_char_string_word(word_t *word,  char value ){
	int ascii_value = (int)value;
	set_value_to_word(word, ascii_value);

}

void set_DIRECTIVES_WORDS(symbol_table_t *){

	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);
	line_t *line = NULL;
	int i = 0;


		line = add_line(wordTable, DC++, symbol);

		for(i = 1 ; i < parser.directive.operand.data_len ; i++){}

}


/*sets the instruction word and the send to othre func to set other words*/
void setOPCODE_INSTRUCTION(symbol_table_t *sym_tbl) {
	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);
	line_t *line1 = add_line(wordTable, IC, symbol);;
	IC++;

	switch (register_count_by_op(parser.op)) {
		case 0:
			if (symbol != NULL) {
				symbol->address = IC;
			}
			set_opcode_into_word(line1->word, parser.op);
			set_ARE_into_word(line1->word, A);
			break;
		case 1:
			if (symbol != NULL)
				symbol->address = IC;
			set_opcode_into_word(line1->word, parser.op);
			set_ARE_into_word(line1->word, A);
			set_operand_type_into_word(line1->word, DESTINATION, parser.operands[1].type_of_register);
			setOPCODE_WORDS(sym_tbl, 1);
			break;
		case 2:
			if (symbol != NULL)
				symbol->address = IC;
			set_opcode_into_word(line1->word, parser.op);
			set_ARE_into_word(line1->word, A);
			set_operand_type_into_word(line1->word, DESTINATION, parser.operands[0].type_of_register);
			setOPCODE_WORDS(sym_tbl, 0);
			set_operand_type_into_word(line1->word, SOURCE, parser.operands[1].type_of_register);
			setOPCODE_WORDS(sym_tbl, 1);
	}
}

	void setOPCODE_WORDS(symbol_table_t *sym_tbl, int idx) {
		symbol_t *symbol2 = NULL;
		line_t *line2 = add_line(wordTable, IC, NULL);
		int num;

		IC++;
		switch (parser.operands[idx].type_of_register) {
			case _IMMEDIATE:
				num = parser.operands[idx].operand.num;
				/*conversion to two complement*/
				num = convertToTwoComp(num);
				set_immediate_into_empty_word(line2->word, num);
				set_ARE_into_word(line2->word, A);
				break;

			case _DIRECT:
				symbol2 = findSymbol(sym_tbl, parser.operands[idx].operand.symbol);
				(symbol2 != NULL) ?: report_error(ERR_SYMBOL_NOT_FOUND, line_count, CRIT);
				set_label_into_empty_word(line2->word, symbol2->address);
				set_ARE_into_word(line2->word, A);
				break;

			case _INDIRECT:/*here a check is made if there are two regiters or not*/
			case _REGULAR:

				/*sign there is only one registry*/
				if (parser.operands[0].type_of_register == _TBD) {
					set_register_into_empty_word(line2->word, parser.operands[1].operand.registry, 0);
					set_ARE_into_word(line2->word, A);
				} else {
					set_register_into_empty_word(line2->word, parser.operands[0].operand.registry, 1);
					set_register_into_empty_word(line2->word, parser.operands[1].operand.registry, 0);
					set_ARE_into_word(line2->word, A);
				}
				break;
		}
	}



line_t *add_line(word_table_t *table, int ic_num, symbol_t *symbol) {
	line_t *new_ptr = NULL;
	int word_t_size = sizeof(word_t);

	if (table == NULL)
		return NULL;

	if (table->size == 1) {
		return table->lines;
	}
	table->size++;
	if (!(new_ptr = realloc(table->lines, table->size * sizeof(line_t)))) {
		report_error(ERR_WORD_TABLE_ALLOCATION, line_count, CRIT);
		return NULL;
	}
	if (new_ptr != table->lines)
		table->lines = new_ptr;

	/*data assignmet to new member */
	table->lines[table->size - 1].line_num = ic_num;
	memset(table->lines[table->size - 1].word, 0, word_t_size);
	table->lines[table->size - 1].symbol = symbol;/*pointer to symbol*/

	return &(table->lines[table->size - 1]);
}

int convertToTwoComp(int num) {
	if (num > 0) return num;
	return (~num) + 1;
}



/*set the opcode into word*/
void set_opcode_into_word(word_t *word, op_code_t op_code) {
	*word |= op_code << opcode_shift;
}

/*insert operand type into instruction - otype:direct/indirect |type_of_register immdediate/direct/.. */
void set_operand_type_into_word(word_t *word, operand_t otype, type_of_register_t type) {
	set_value_to_word(word, (1 << type) << (operand_shift + operand_size * otype));
}

/* Set ARE in first 3 bits */
void set_ARE_into_word(word_t *word, ARE_T ARE) {
	set_value_to_word(word, 1 << ARE);
}

/*insert operand into new empty word*/
void set_immediate_into_empty_word(word_t *word, int value) {
	set_value_to_word(word, value << immediate_shift);
}

void set_label_into_empty_word(word_t *word, int value) {
	set_value_to_word(word, value << immediate_shift);
}

void set_register_into_empty_word(word_t *word, int value, int isSource) {
	set_value_to_word(word, value << (register_shift + isSource * register_size));
}


/*elementry handling of the shift*/
void set_value_to_word(word_t *word, int value) {
	/*0x7FFF allows 15 zeroes*/
	*word |= value & 0x7FFF;
}

word_table_t *initTable(word_table_t *table) {
	table->size = 1;
	table->lines = (line_t *) calloc(wordTable->size, sizeof(line_t));
	table->next = NULL;
	return table;
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








