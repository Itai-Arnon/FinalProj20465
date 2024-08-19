#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers/macro.h"
#include "headers/shared.h"
#include "headers/global_vars.h"
#include "headers/utils.h"
#include "headers/symbols.h"
#include "headers/parser.h"
#include "headers/first_pass.h"
#include "headers/assembler.h"
#include "headers/second_pass.h"


int IC = 100; /*first address of the instruction table is preset in tabel init*/
int DC;

/**  out.txt ***/
void first_pass(macro_table_t* macroTable,  symbol_table_t *sym_tbl, word_table_t *wordTable, word_table_t *dataTable) {
	int result = 0, n = 0;
	if(isError) {
		return;
	}


	switch (parser.line_type) {
		case OP_CODE:
			printf("registry%d\n",parser.operands[1].operand.registry);
			setOPCODE_INSTRUCTION(sym_tbl, wordTable);
			if (parser.op == stop) {
				printf("Stop Occured\n");
				n  = wordTable->lines[wordTable->size - 1].line_num + 1;
				printf("n: %d\n", n);
				addAddressToSymbols(sym_tbl,_DATA, n);
				addNumberToWordTable(dataTable, n );
				printTable(wordTable);
				printTable(dataTable);
				second_pass(macroTable, sym_tbl, wordTable, dataTable);
			}
			break;
		case DIRECTIVE:
			if ((result = parser.directive.cmd) == DATA) {
				set_DATA_WORDS(sym_tbl, dataTable);

			} else if (result == STRING) {
				set_STRING_WORDS(sym_tbl, dataTable);


			} else if (result == ENTRY || result == EXTERN) {
				set_EXTnEntry(sym_tbl, dataTable);

			}
			break;
		case ERR:
			report_error(ERR_GENERAL_FIRST_PASS_ERROR, line_count ,FIRST , CRIT);
			break;
		case TBD:
			report_error(ERR_LINE_UNRECOGNIZED, line_count ,FIRST , CRIT);
			break;
		default:
			break;
	}
}

/*sets the instruction word and the send to othre func to set other words*/
void setOPCODE_INSTRUCTION(symbol_table_t *sym_tbl, word_table_t *table) {
	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);

	line_t *line1 = add_line(table, IC, symbol,_NO);

	type_of_register_t type0 = parser.operands[0].type_of_register;
	type_of_register_t type1 = parser.operands[1].type_of_register;
	IC++;
	printf("IC: %d\n", IC);
	if (symbol != NULL) {
		symbol->address = IC;
		symbol->type = _INSTRUCTION;
		symbol->are = A;
	}
	printf("INSIDE OPCODE LINE1 ptr VALUE :%p\n", line1);
	switch (register_count_by_op(parser.op)) {
		case 0:

			set_opcode_into_word(&(line1->word), parser.op);
			set_ARE_into_word(&(line1->word), A);
			printf("INSIDE OPCODE: 0 registries\n");
			printBinary(line1->word);
			break;
		case 1:

			set_opcode_into_word(&(line1->word), parser.op);
			set_ARE_into_word(&(line1->word), A);
			set_operand_type_into_word(&(line1->word), DESTINATION, parser.operands[1].type_of_register);
			setOPCODE_WORDS(sym_tbl, table, 1, 0);
			printf("INSIDE OPCODE  case: 1 registries");
			printBinary(line1->word);
			break;
		case 2:

			set_opcode_into_word(&(line1->word), parser.op);
			set_ARE_into_word(&(line1->word), A);
			set_operand_type_into_word(&(line1->word), DESTINATION, parser.operands[0].type_of_register);
			set_operand_type_into_word(&(line1->word), SOURCE, parser.operands[1].type_of_register);
			printf("INSIDE OPCODE  case 2 registries and opcode");
			printBinary(line1->word);

			/*INDIRECT and DIRECT Register are set only once to setOPCODE_WORDS register selection decides*/


			if (registerSelection() == 2) {
				/*all combinations except INDIRECT and Regular registry(4th type) type is zero*/
				setOPCODE_WORDS(sym_tbl, table, 0, 0);
				setOPCODE_WORDS(sym_tbl, table, 1, 0);
			} else
				/*INDIRECT and Regular registry, type is one*/
				setOPCODE_WORDS(sym_tbl, table, 1, 1);
	}
	printf("register 0 value: %d\n", parser.operands[0].type_of_register);
	printf("register 1 value:%d\n", parser.operands[1].type_of_register);
	printf("typeof registry in operand 0 value: %d\n", parser.operands[0].operand.registry);
	printf("typeof registry in operand 1 value: %d\n", parser.operands[1].operand.registry);


}


/*if type = 1  2 regists indirect and/or direct |type=0 only one */
void setOPCODE_WORDS(symbol_table_t *sym_tbl, word_table_t *table, int idx, int type) {
	symbol_t *symbol2 = NULL;
	line_t *line2 = add_line(table, IC, NULL, _NO);
	printf("line2 :%p  registry %d\n", line2, parser.operands[idx].type_of_register);
	unsigned short num;
	IC++;
	switch (parser.operands[idx].type_of_register) {
		case _IMMEDIATE:
			num = parser.operands[idx].operand.num;
			/*conversion to two complement*/
			num = convertToTwoComp(num);
			set_immediate_into_empty_word(&(line2->word), num);
			set_ARE_into_word(&(line2->word), A);
			printf(" INSIDE WORDS CASE: _immediate ||ptr address  :%p   ", line2);
			printBinary(line2->word);
			break;

		case _DIRECT:
			symbol2 = findSymbol(sym_tbl, parser.operands[idx].operand.symbol);
			(symbol2 != NULL) ?: report_error(ERR_SYMBOL_NOT_FOUND, line_count ,FIRST , CRIT);
			set_label_into_empty_word(&(line2->word), symbol2->address);
			set_ARE_into_word(&(line2->word), A);
			printf("(INSIDE WORDS |CASE: DIRECT  obj variable: line2||  ptr address  :%p  registry type %d\n", line2,
			       parser.operands[idx].type_of_register);
			printBinary(line2->word);
			break;

		case _INDIRECT:/*here a check is made if there are two regiters or not*/
		case _REGULAR:


			if (type == 0) {
				set_register_into_empty_word(&(line2->word), parser.operands[idx].operand.registry, 0);
				set_ARE_into_word(&(line2->word), A);
				printf("INSIDE WORDS |CASE:single registry indirect/regular  || line2-variable :%p   %d\n", line2,
				       parser.operands[idx].type_of_register);
				printBinary(line2->word);
			}
			if (type == 1) {
				set_register_into_empty_word(&(line2->word), parser.operands[0].operand.registry, 1);
				set_register_into_empty_word(&(line2->word), parser.operands[1].operand.registry, 0);
				set_ARE_into_word(&(line2->word), A);
				printf("INSIDE WORDS |CASE: (line2) :%p indirect and regular registry %d\n", line2,
				       parser.operands[idx].type_of_register);
				printBinary(line2->word);

			}
			break;
		case _ERROR:
			report_error(ERR_REGISTRY_ILLEGAL, line_count ,FIRST , CRIT);

		case _TBD:
			/*default value is TBD*/
			break;
	}

}


void set_DATA_WORDS(symbol_table_t *sym_tbl, word_table_t *table) {

	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);
	line_t *line = NULL;
	int i = 0;
	unsigned short num = 0;


	if (symbol != NULL) {
		symbol->address = DC + 1;
		symbol->type = _DATA;
		symbol->are = A;
	}

	for (i = 0; i < parser.directive.operand.data_len; i++) {
		/*only the first line has a potential symbol*/
		line = add_line(table, DC, (i == 0) ? symbol : NULL, _NO);
		num = parser.directive.operand.data[i];
		num = convertToTwoComp(num);
		set_number_data_word(&(line->word), num);
		printf(" LINE obj address :%p\n", line);
		printf("data - fills with numbers all 15 bit\n ");
		printBinary(line->word);
		DC++;
	}
}

void set_STRING_WORDS(symbol_table_t *sym_tbl, word_table_t *table) {
	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);
	line_t *line = NULL;
	int i = 0;
	if (symbol != NULL) {
		symbol->address = DC + 1;
		symbol->type = _DATA;
		symbol->are = A;
	}

	for (i = 0; i < parser.directive.operand.data_len; i++) {
		/*only the first line has a potential symbol*/
		line = add_line(table, DC, (i == 0) ? symbol : NULL, _NO);
		set_char_string_word(&(line->word), parser.directive.operand.str[i]);
		printf("string line object address :%p\n", line);
		printf("string -  char list and a null\t ");
		printBinary(line->word);
		DC++;
	}
	/*adding '\0' */
	line = add_line(table, DC, NULL, A);
	set_number_data_word(&(line->word), '\0');
	printf("string line object address  :%p\n", line);
	printf("string -  char list and a null ");
	printBinary(line->word);
	DC++;

}
/*sets the entry or extern word*/
void set_EXTnEntry(symbol_table_t *sym_tbl, word_table_t *table) {
	symbol_t *symbol = findSymbol(sym_tbl, parser.directive.operand.symbol);
	line_t *line = NULL;
	int i = 0;
	ARE_T are = (parser.directive.cmd == EXTERN) ? E : R;
	EXT_T _ARE = (parser.directive.cmd == EXTERN) ? _EX : _EN;

	if (symbol != NULL) {
		symbol->address = DC + 1; /*considers in advanced the creation of line*/
		symbol->type = _DATA;
		symbol->are = are;
	}
/* unlike the rest |symbol is a registry*/

	line = add_line(table, DC, symbol, _ARE);
	line->_ARE = _ARE;
	/*in case EXTERN we are done, no info goes to the word*/
	if (are == R) {
		set_label_into_empty_word(&(line->word), DC);
	}
	printf("Entry Or Extern object address  :%p\n", line);
	printf("CASE EXTERN we are done word is zeroes \n ");
	printf("CASE ENTRY if we don't have the address we'll look at it in the 2nd pass  \n ");
	printBinary(line->word);
}


int registerSelection() {
	type_of_register_t type0 = parser.operands[0].type_of_register;
	type_of_register_t type1 = parser.operands[1].type_of_register;
	switch (type0) {
		case _DIRECT:
		case _IMMEDIATE:
			return 2;
		case _REGULAR:
		case _INDIRECT:
			if (type1 == _DIRECT || type1 == _IMMEDIATE)
				return 2;
			else
				return 1;
			break;
		defult:
			return 2;
			break;
	}
}

unsigned short convertToTwoComp(unsigned short num) {
	if (num >= 0) return num;
	return (~num) + 1;
}

void set_number_data_word(word_t *word, char value) {
	int intVal = convertToTwoComp(value);
	set_value_to_word(word, intVal);
}

void set_char_string_word(word_t *word, char value) {
	char ascii_value = (int) value;
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
void set_immediate_into_empty_word(word_t *word, unsigned short value) {
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
	/*0x7FFF against times that 16th bit lights up*/
	*word |= value & 0x7FFF;
}

word_table_t *initTable(word_table_t *table , int memInit) {

	table = calloc(1, sizeof(word_table_t));
	table->lines = calloc(1, sizeof(line_t));
	table->size = 1;
	table->isFirst = 1;
	table->lines->line_num = memInit;
	table->lines->symbol = NULL;
	table->lines->_ARE = _NO;

	return table;
}


/*adds a line to wordTable/dataTable  the first line has an address 100*/
line_t *add_line(word_table_t *table, int ic_num, symbol_t *symbol , EXT_T _ARE) {
	line_t *new_ptr = NULL;
	int word_t_size = sizeof(word_t);

	if (table == NULL)
		return NULL;

	if(table->isFirst == 1) {
		table->isFirst = 0;
		return &(table->lines[0]);
	}
	table->size++;

	if (!(new_ptr = (line_t *) realloc(table->lines, (table->size) * sizeof(line_t)))) {
		report_error(ERR_WORD_TABLE_ALLOCATION, line_count ,FIRST , CRIT);
		return NULL;
	}
	if (new_ptr != table->lines)
		table->lines = new_ptr;

	/*data assignmet to new member */
	table->lines[table->size - 1].line_num = ic_num; /*address*/
	table->lines[table->size - 1].symbol = symbol;/*pointer to symbol*/
	table->lines[table->size - 1].word = 0;
	table->lines[table->size - 1]._ARE = _ARE;

		return &(table->lines[(table->size - 1)]);
}



/*adds a number to all the words in the table*/
void addNumberToWordTable(word_table_t *table, int number) {
	int i;
	if (table == NULL || table->lines == NULL) return;

	for (i = 0; i < table->size; i++) {
		table->lines[i].line_num += number;
	}
}


void printBinary(unsigned short num) {
	int i = 0;
	int bits = sizeof(unsigned short) * 8; /* Number of bits in the unsigned short*/
	for (i = bits - 1; i >= 0; i--) {
		unsigned short mask = 1 << i;
		printf("%d", (num & mask) ? 1 : 0);
	}
	printf("\n");
}


void printTable(word_table_t *table) {
	int i;

	if (table == NULL) {
		return;
	}

	for (i = 0; i < table->size; i++) {
		printf("%05d\t", table->lines[i].line_num); /* Print line number with leading zeros */
		printf("%05d\t", table->lines[i]._ARE); /* Print line number with leading zeros */
		printBinary(table->lines[i].word);         /* Print word in binary */
	}
}








