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
int DC;

void printBinary(unsigned short num) {
	int i =0;
	int bits = sizeof(unsigned short) * 8; /* Number of bits in the unsigned short*/
	for ( i = bits - 1; i >= 0; i--) {
		unsigned short mask = 1 << i;
		printf("%d", (num & mask) ? 1 : 0);
	}
	printf("\n");
}
/**  out.txt ***/
void first_pass(symbol_table_t *sym_tbl, word_table_t *wordTable, word_table_t *dataTable) {
	int initVal = 100;
	int result = 0;
	IC = initVal;
	DC = 0;

	wordTable = initTable(wordTable);
	dataTable = initTable(dataTable);

	switch (parser.line_type) {
		case OP_CODE:
			setOPCODE_INSTRUCTION(sym_tbl, wordTable);
			break;
		case DIRECTIVE:
			if ((result = parser.directive.cmd) == DATA) {
				set_DATA_WORDS(sym_tbl,dataTable);

			} else if (result == STRING) {
				set_STRING_WORDS(sym_tbl,dataTable);


			} else if (result == ENTRY || result == EXTERN) {}
			break;
		case ERR:
			report_error(ERR_GENERAL_FIRST_PASS_ERROR, line_count, CRIT);
			break;
		case TBD:
			report_error(ERR_LINE_UNRECOGNIZED, line_count, CRIT);
			break;
		default:
			break;
	}

}

/*sets the instruction word and the send to othre func to set other words*/
void setOPCODE_INSTRUCTION(symbol_table_t *sym_tbl, word_table_t *wordTable) {
	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);

	line_t *line1 = add_line(wordTable, IC, symbol);
	type_of_register_t type0 = parser.operands[0].type_of_register;
	type_of_register_t type1 = parser.operands[1].type_of_register;
	IC++;
	printf("OPCODE LINE1 :%p\n",line1);
	switch (register_count_by_op(parser.op)) {
		case 0:
			if (symbol != NULL) {
				symbol->address = IC;
				symbol->type=_INSTRUCTION;
				symbol->are =A;
			}
			set_opcode_into_word(&(line1->word), parser.op);
			set_ARE_into_word(&(line1->word), A);
			break;
		case 1:
			if (symbol != NULL)
				symbol->address = IC;
			set_opcode_into_word(&(line1->word), parser.op);
			set_ARE_into_word(&(line1->word), A);
			set_operand_type_into_word(&(line1->word), DESTINATION, parser.operands[1].type_of_register);
			setOPCODE_WORDS(sym_tbl, wordTable, 1,0);
			break;
		case 2:
			if (symbol != NULL)
				symbol->address = IC;
			set_opcode_into_word(&(line1->word), parser.op);
			set_ARE_into_word(&(line1->word), A);
			set_operand_type_into_word(&(line1->word), DESTINATION, parser.operands[0].type_of_register);
			set_operand_type_into_word(&(line1->word), SOURCE, parser.operands[1].type_of_register);


			/*INDIRECT and DIRECT Register are set only once to setOPCODE_WORDS register selection decides*/


			if(registerSelection() == 2 ) {
				/*all combinations except INDIRECT and Regular registry(4th type) type is zero*/
				setOPCODE_WORDS(sym_tbl, wordTable, 0, 0);
				setOPCODE_WORDS(sym_tbl, wordTable, 1, 0);
			}else
				/*INDIRECT and Regular registry, type is one*/
				setOPCODE_WORDS(sym_tbl, wordTable, 1,1);
	}
	printf("register 0 %d\n",parser.operands[0].type_of_register);
	printf("register 1 %d\n",parser.operands[1].type_of_register);
	printf("registry 0 %d\n",parser.operands[0].operand.registry);
	printf("registry 1 %d\n",parser.operands[1].operand.registry);
	printf("first word:");
	printBinary(line1->word);

}
/*if type = 1  2 regists indirect and/or direct |type=0 only one */
void setOPCODE_WORDS(symbol_table_t *sym_tbl, word_table_t *wordTable, int idx ,int type) {
	symbol_t *symbol2 = NULL;
	line_t *line2 = add_line(wordTable, IC, NULL);
	printf("line2 :%p  registry %d\n",line2 ,parser.operands[idx].type_of_register);
	int num;

	IC++;
	switch (parser.operands[idx].type_of_register) {
		case _IMMEDIATE:
			num = parser.operands[idx].operand.num;
			/*conversion to two complement*/
			num = convertToTwoComp(num);
			set_immediate_into_empty_word(&(line2->word), num);
			set_ARE_into_word(&(line2->word), A);
			printf("line2 _immediate:");
			printBinary(line2->word);
			break;

		case _DIRECT:
			symbol2 = findSymbol(sym_tbl, parser.operands[idx].operand.symbol);
			(symbol2 != NULL) ?: report_error(ERR_SYMBOL_NOT_FOUND, line_count, CRIT);

			set_label_into_empty_word(&(line2->word), symbol2->address);
			set_ARE_into_word(&(line2->word), A);
			printf("empty word (line2) :%p DIRECT registry %d\n",line2 ,parser.operands[idx].type_of_register);
			printBinary(line2->word);
			break;

		case _INDIRECT:/*here a check is made if there are two regiters or not*/
		case _REGULAR:


			 if(type == 0) {
				 set_register_into_empty_word(&(line2->word), parser.operands[idx].operand.registry, 0);
				 set_ARE_into_word(&(line2->word), A);
				 printf("empty word (line2) :%p one registry indirect/regualar %d\n", line2, parser.operands[idx].type_of_register);
				 printBinary(line2->word);
			 }
			if(type == 1){
				set_register_into_empty_word(&(line2->word), parser.operands[0].operand.registry, 1);
				set_register_into_empty_word(&(line2->word), parser.operands[1].operand.registry, 0);
				set_ARE_into_word(&(line2->word), A);
				printf("empty word (line2) :%p indirect and regular registry %d\n",line2 ,parser.operands[idx].type_of_register);
				printBinary(line2->word);

			}
			break;
		case _ERROR:
			report_error(ERR_REGISTRY_ILLEGAL, line_count, CRIT);

		case _TBD:
			/*default value is TBD*/
			break;
	}
	/*printBinary(line2->word);*/
}

void set_DATA_WORDS(symbol_table_t *sym_tbl, word_table_t *dataTable) {

	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);
	line_t *line = NULL;
	int i = 0;

	for (i = 0; i <= parser.directive.operand.data_len; i++) {
		/*only the first line has a potential symbol*/
		line = add_line(dataTable, DC, (i == 0) ? symbol : NULL);
		set_number_data_word(&(line->word), parser.directive.operand.data[i]);
		DC++;
	}

}

void set_STRING_WORDS(symbol_table_t *sym_tbl, word_table_t *dataTable) {
	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);
	line_t *line = NULL;
	int i = 0;

	for (i = 0; i <= parser.directive.operand.data_len; i++) {
		/*only the first line has a potential symbol*/
		line = add_line(dataTable, DC, (i == 0) ? symbol : NULL);
		set_number_data_word(&(line->word), parser.directive.operand.str[i]);
		DC++;
	}
	line = add_line(dataTable, DC, NULL);
	set_number_data_word(&(line->word), '\0');
	DC++;
}



int registerSelection() {
	type_of_register_t type0 = parser.operands[0].type_of_register;
	type_of_register_t type1 = parser.operands[1].type_of_register;
	switch(type0){
		case _DIRECT:
		case _IMMEDIATE:
			return 2;
		case _REGULAR:
		case _INDIRECT:
		 if (type1 == _DIRECT ||type1 == _IMMEDIATE)
			 return 2;
		 else
			 return 1;
		break;
		defult:
			return 2;
			break;
	}
}

line_t *add_line(word_table_t *table, int ic_num, symbol_t *symbol) {
	line_t *new_ptr = NULL;
	int word_t_size = sizeof(word_t);

	if (table == NULL)
		return NULL;


	table->size++;
	if (!(new_ptr = (line_t *) realloc(table->lines, (table->size) * sizeof(line_t)))) {
		report_error(ERR_WORD_TABLE_ALLOCATION, line_count, CRIT);
		return NULL;
	}
	if (new_ptr != table->lines)
		table->lines =  new_ptr;

	/*data assignmet to new member */
	table->lines[table->size - 1].line_num = ic_num; /*address*/
	table->lines[table->size - 1].symbol = symbol;/*pointer to symbol*/
	table->lines[table->size - 1].word = 0;
	return &(table->lines[(table->size - 2)]);
}

int convertToTwoComp(int num) {
	if (num >= 0) return num;
	return (~num) + 1;
}

void set_number_data_word(word_t *word, char value) {
	int intVal = convertToTwoComp(value);
	set_value_to_word(word, intVal);
}

void set_char_string_word(word_t *word, char value) {
	int ascii_value = (int) value;
	set_value_to_word(word, ascii_value);

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
	set_value_to_word(word, value << label_shift);
}

void set_register_into_empty_word(word_t *word, int value, int isSource) {
	set_value_to_word(word, value << (register_shift + isSource * register_size));
}


/*elementry handling of the shift*/
void set_value_to_word(word_t *word, int value) {
	/*0x7FFF allows 15 zeroes*/
	*word |= value & 0x7FFF;
}


word_table_t *initTable(word_table_t *wordTable) {

	wordTable = calloc(1, sizeof(word_table_t));
	wordTable->lines = calloc(1, sizeof(line_t));
	wordTable->size = 1;
	wordTable->next = NULL;
	wordTable->lines->line_num = 0;
	wordTable->lines->symbol = NULL;

	return wordTable;
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








