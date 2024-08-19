
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers/second_pass.h"
#include "headers/macro.h"
#include "headers/macro_list.h"
#include "headers/shared.h"
#include "headers/global_vars.h"
#include "headers/utils.h"
#include "headers/symbols.h"
#include "headers/parser.h"
#include "headers/first_pass.h"
#include "headers/assembler.h"

IC = 0;
DC = 0;


void second_pass(macro_table_t *macroTable, symbol_table_t *sym_tbl, word_table_t *wordTable, word_table_t *dataTable) {
	word_table_t *eTable; /*entry table*/
	line_t *current_line;
	symbol_t *symbol1, *symbol2;
	char *boo;
	int idx, result = 0, n = 0;

	switch (parser.line_type) {
		case OP_CODE:
			printf("registry%d\n",parser.operands[1].operand.registry);
			checkOPCODE_INSTRUCTION(sym_tbl, wordTable);
			if (parser.op == stop) {
				printf("Stop Occured\n");
				n  = wordTable->lines[wordTable->size - 1].line_num + 1;
				printf("n: %d\n", n);
				printTable(wordTable);
				printTable(dataTable);
			}
			break;
		case DIRECTIVE:
			if ((result = parser.directive.cmd) == DATA) {
				checkDATA_WORDS(sym_tbl, dataTable);

			} else if (result == STRING) {
				checkSTRING_WORDS(sym_tbl, dataTable);


			} else if (result == ENTRY || result == EXTERN) {
				checkEXTnEntry(sym_tbl, dataTable);

			}
			break;
		case ERR:
			report_error(ERR_GENERAL_FIRST_PASS_ERROR, line_count ,SECOND , CRIT);
			break;
		case TBD:
			report_error(ERR_LINE_UNRECOGNIZED, line_count ,SECOND , CRIT);
			break;
		default:
			break;
	}





print_symbol_table(sym_tbl);
	printTableToFile(wordTable, dataTable, boo);

	/*if (isError) {
		return;
	}

	for (idx = 0; idx < wordTable->size; ++idx) {
		current_line = &wordTable->lines[idx];
		symbol1 = current_line->symbol;

		if (symbol1 != NULL && symbol1->address == 0) {
			symbol2 = findSymbol(sym_tbl, symbol1->symbol_name);
			if (symbol2 == NULL) {
				report_error(ERR_SYMBOL_NOT_FOUND, line_count ,SECOND , CRIT);
				return;
			}
			if (symbol2->are == E)
				continue;

			if (symbol2->address != 0) {
				symbol1->address = symbol2->address;
			}
		}*/


	/*checkSymbolsUnique(macroTable,sym_tbl);*/
}


void checkOPCODE_INSTRUCTION(symbol_table_t *sym_tbl, word_table_t *table) {
	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);

	line_t *line1 = &table->lines[IC];

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
			
			printf(" 2nd Pass OPCODE: 0 registries\n");
			printBinary(line1->word);
			break;
		case 1:

			
			checkOPCODE_WORDS(sym_tbl, table, 1, 0);
			printf("2nd PASS INSIDE OPCODE  case: 1 registries");
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
void checkOPCODE_WORDS(symbol_table_t *sym_tbl, word_table_t *table, int idx, int type) {
	symbol_t *symbol2 = NULL;
	line_t *line2 = &table->lines[IC];
	printf("line2 :%p  registry %d\n", line2, parser.operands[idx].type_of_register);
	unsigned short num;
	IC++;
	switch (parser.operands[idx].type_of_register) {
		case _IMMEDIATE:
			
			printf("2nd Pass INSIDE WORDS CASE: _immediate ||ptr address  :%p   ", line2);
			printBinary(line2->word);
			break;

		case _DIRECT:
			symbol2 = findSymbol(sym_tbl, parser.operands[idx].operand.symbol);
			(symbol2 != NULL) ?: report_error(ERR_SYMBOL_NOT_FOUND, line_count , SECOND , CRIT);
			set_label_into_empty_word(&(line2->word), symbol2->address);
			
			printf("(INSIDE WORDS |CASE: DIRECT  obj variable: line2||  ptr address  :%p  registry type %d\n", line2,
			       parser.operands[idx].type_of_register);
			printBinary(line2->word);
			break;

		case _INDIRECT:/*here a check is made if there are two regiters or not*/
		case _REGULAR:


			if (type == 0) {
				
				printf("INSIDE WORDS |CASE:single registry indirect/regular  || line2-variable :%p   %d\n", line2,
				       parser.operands[idx].type_of_register);
				printBinary(line2->word);
			}
			if (type == 1) {
				printf("INSIDE WORDS |CASE: (line2) :%p indirect and regular registry %d\n", line2,
				       parser.operands[idx].type_of_register);
				printBinary(line2->word);

			}
			break;
		case _ERROR:
			report_error(ERR_REGISTRY_ILLEGAL, line_count ,SECOND , CRIT);

		case _TBD:
			/*default value is TBD*/
			break;
	}

}

void checkDATA_WORDS(symbol_table_t *sym_tbl, word_table_t *table) {

	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);
	line_t *line = NULL;
	int i = 0;
	unsigned short num = 0;


	if (symbol != NULL) {
		printf("2nd pass Symbol Address %d\n",symbol->address);
	}

	for (i = DC; i < parser.directive.operand.data_len; i++) {
		/*only the first line has a potential symbol*/
		line = &table->lines[i];
		printBinary(line->word);
		DC++;
	}
}
void checkSTRING_WORDS(symbol_table_t *sym_tbl, word_table_t *table) {
	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);
	line_t *line = NULL;
	int i = 0;
	if (symbol != NULL) {
		printf("String Symbol Address %d\n",symbol->address);

	}

	for (i = DC; i < parser.directive.operand.data_len; i++) {
		line = &table->lines[i];
		printBinary(line->word);
		DC++;
	}
	/*adding '\0' */
	line = &table->lines[DC];

	printBinary(line->word);
	DC++;

}

void checkEXTnEntry(symbol_table_t *sym_tbl, word_table_t *table) {
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


/*check if there are duplicate symbols*/
void checkSymbolsUnique(macro_table_t *macro_table, symbol_table_t *sym_table) {
	macro_node_t *m;
	symbol_t *symbol;
	int i = 0;

	for (i = 0; i < macro_table->size; ++i) {
		m = &macro_table->slot[i];
		for (symbol = sym_table->symbol_List; symbol != NULL; symbol = symbol->next_sym) {
			if (strcmp(m->macro_name, symbol->symbol_name) == 0) {
				report_error(ERR_MACRO_NAME_OP_DIRECT_SYMBOL, line_count, SECOND, CRIT);
				free(m);
				free(symbol);
				return;
			}
		}

	}

}

int convertToOctal(int num) {
	int ans = 0, y = 1;

	while (num != 0) {
		ans += (num % 8) * y;
		num = num / 8;
		y *= 10;
	}
	return ans;
}

void printTableToFile(word_table_t *wTable, word_table_t *dTable, char *file_name) {
	int i = 0, num = 0;
	if (wTable == NULL) {
		return;
	}
	printf("\n");

	printf("  %d  %d\n", wTable->size, dTable->size);
	for (i = 0; i < wTable->size; i++) {
		num = convertToOctal(wTable->lines[i].word);
		printf("%05d %05d\n", wTable->lines[i].line_num, num); /* Print line number with leading zeros */

	}
	for (i = 0; i < dTable->size; i++) {
		num = convertToOctal(dTable->lines[i].word);
		printf("%05d %05d\n", dTable->lines[i].line_num, num);
	}
	printf("\n");
}

/*check for symbol with undefined address*/
int missingValuesOfSymbols(symbol_table_t *table) {
	symbol_t *head;

	if (table == NULL) {
		return 0;
	}
	head = table->symbol_List;

	while (head != NULL) {
		if (head->address == 0) {
			return 1;
		}
		head = head->next_sym;
	}

	return 0;
}
/*check for faults w/ extern symbols*/
int checkExternSymbols(symbol_table_t *table, int option) {
	symbol_t *symbol1, *symbol2;

	if (table == NULL) {
		return 0;
	}
	/*option 1:compare extern with absolute*/
	switch (option) {
		case 1:
			for (symbol1 = table->symbol_List; symbol1 != NULL; symbol1 = symbol1->next_sym) {
				if (symbol1->are == E) {
					for (symbol2 = table->symbol_List; symbol2 != NULL; symbol2 = symbol2->next_sym) {
						if (symbol2->are == A && strcmp(symbol1->symbol_name, symbol2->symbol_name) == 0) {
							return 1;
						}
					}
				}
			}
			break;
			/*option 2:compare extern with absolute*/
		case 2:
			for (symbol1 = table->symbol_List; symbol1 != NULL; symbol1 = symbol1->next_sym) {
				if (symbol1->are == E) {
					for (symbol2 = table->symbol_List; symbol2 != NULL; symbol2 = symbol2->next_sym) {
						if (symbol2->are == R && strcmp(symbol1->symbol_name, symbol2->symbol_name) == 0) {
							return 1;
						}
					}
				}
			}
			break;
		default:
			return 0;
	}
	return 0;
}



