
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


void second_pass(macro_table_t *macroTable, symbol_table_t *sym_tbl, symbol_table_t *entryTable ,word_table_t *wordTable, word_table_t *dataTable) {
	line_t *current_line;
	symbol_t *symbol1, *symbol2;
	char *boo;
	int idx, result = 0, n = 0;
	if(isError == 1){
		return;

	}

	n = wordTable->lines[wordTable->size - 1].line_num + 1;
	printf("n: %d\n", n);
	addConstantToSymbols(sym_tbl,_DATA, n);
	addNumberToWordTable(dataTable, n );



	IC = 0;
	DC = 0;


	switch (parser.line_type) {
		case OP_CODE:

			printf("registry%d\n", parser.operands[1].operand.registry);
			checkOPCODE_INSTRUCTION(sym_tbl, wordTable);

			if (parser.op == stop) {
				printf("Stop Occured\n");
				printTable(wordTable);
				printTable(dataTable);

			}
			break;
		case DIRECTIVE:
			if ((result = parser.directive.cmd) == DATA) {
				checkDATA_WORDS(sym_tbl, dataTable);

			} else if (result == STRING) {
				checkSTRING_WORDS(sym_tbl, dataTable);

			} else if (result == EXTERN) {
				checkEXTERN(sym_tbl,wordTable);
			/*entry*/
			}else{
				printEntrySymbolTable(entryTable, "" , 0 );
			}

			break;
		case ERR:
			report_error(ERR_GENERAL_FIRST_PASS_ERROR, line_count, SECOND, CRIT, 0);
			break;
		case TBD:
			report_error(ERR_LINE_UNRECOGNIZED, line_count, SECOND, CRIT, 0);
			break;
		default:
			break;
	}


	print_symbol_table(sym_tbl);

}


void checkOPCODE_INSTRUCTION(symbol_table_t *sym_tbl, word_table_t *table) {
	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);

	line_t *line1 = &table->lines[IC];

	type_of_register_t type0 = parser.operands[0].type_of_register;
	type_of_register_t type1 = parser.operands[1].type_of_register;
	IC++;
	printf("IC: %d\n", IC);
	if (symbol != NULL) {
		/*notifies about address problem*/
		if (symbol->address < 100)
			report_error(WAR_MEMORY_NOT_CONFIGURED, line_count, SECOND, NON_CRIT, symbol->address);

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


			printBinary(line1->word);

			/*INDIRECT and DIRECT Register are set only once to checkOPCODE_WORDS register selection decides*/


			if (registerSelection() == 2) {
				/*all combinations except INDIRECT and Regular registry(4th type) type is zero*/
				checkOPCODE_WORDS(sym_tbl, table, 0, 0);
				checkOPCODE_WORDS(sym_tbl, table, 1, 0);
			} else
				/*INDIRECT and Regular registry, type is one*/
				checkOPCODE_WORDS(sym_tbl, table, 1, 1);
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

	IC++;
	switch (parser.operands[idx].type_of_register) {
		case _IMMEDIATE:

			printf("2nd Pass INSIDE WORDS CASE: _immediate ||ptr address  :%p   ", line2);
			printBinary(line2->word);
			break;

		case _DIRECT:
			symbol2 = findSymbol(sym_tbl, parser.operands[idx].operand.symbol);
			if (symbol2 != NULL) {
				if (symbol2->address < 100)
					report_error(WAR_MEMORY_NOT_CONFIGURED, line_count, SECOND, NON_CRIT, symbol2->address);
			}
			else
					report_error(ERR_SYMBOL_NOT_FOUND, line_count, SECOND, CRIT, 0);

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
			report_error(ERR_REGISTRY_ILLEGAL, line_count, SECOND, CRIT, 0);
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
		printf("2nd pass Symbol Address %d\n", symbol->address);
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
		printf("String Symbol Address %d\n", symbol->address);
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

void checkEXTERN(symbol_table_t *sym_tbl, word_table_t *table) {
	line_t *line = &table->lines[DC];
	symbol_t *symbol = line->symbol;

/* unlike the rest |symbol is a registry*/
	if (symbol != NULL) {

		DC++;
	}else
		report_error(ERR_EXTERN_SYMBOL, line_count, SECOND, CRIT, DC);
	printf("Extern object ptr address  :%p\n", line);
	printf("CASE EXTERN we are done word is zeroes \n ");
	printf("CASE ENTRY if we don't have the address we'll look at it in the 2nd pass  \n ");
	printBinary(line->word);
}


/*check if there are duplicate symbols with macro table*/
void checkSymbolsUnique(macro_table_t *macro_table, symbol_table_t *sym_table) {
	macro_node_t *m;
	symbol_t *symbol;
	int i = 0;

	for (i = 0; i < macro_table->size; ++i) {
		m = &macro_table->slot[i];
		for (symbol = sym_table->symbol_List; symbol != NULL; symbol = symbol->next_sym) {
			if (strcmp(m->macro_name, symbol->symbol_name) == 0) {
				report_error(ERR_MACRO_NAME_OP_DIRECT_SYMBOL, line_count, SECOND, CRIT, 0);
				free(m);
				free(symbol);
				return;
			}
		}

	}

}

/*int (all machine language) to octal */
int convertToOctal(int num) {
	int ans = 0, y = 1;

	while (num != 0) {
		ans += (num % 8) * y;
		num = num / 8;
		y *= 10;
	}
	return ans;
}

/*printing table in octal */
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
symbol_t * firstSymbolMissingValue(symbol_table_t *table) {
	symbol_t *head = NULL;
	int i = 0;

	if (table == NULL) {
		return 0;
	}
	head = table->symbol_List;

	while (head != NULL) {
		if (head->address == 0) {
			 return head;
		}
		head = head->next_sym;
	}
	return NULL;
}


/*check for faults w/ extern symbols*/
int checkExternSymbols(symbol_table_t *table) {
	symbol_t *symbol1, *symbol2;

	if (table == NULL) {
		return 0;
	}


			for (symbol1 = table->symbol_List; symbol1 != NULL; symbol1 = symbol1->next_sym) {
				if (symbol1->type == _EXTERN) {
					for (symbol2 = table->symbol_List; symbol2 != NULL; symbol2 = symbol2->next_sym) {
						if ((symbol2->type == _INSTRUCTION || symbol2->type== _DATA) && strcmp(symbol1->symbol_name, symbol2->symbol_name) == 0)
							return 1;

					}
				}
			}
	return 0;
}

/*entry has a specific table , all entry are moved to it*/
int moveSymbolsToEntry(symbol_table_t *sym_tbl ,symbol_table_t *entrySTable) {
	symbol_t *head = NULL;
	int i = 0, counter = 0;

	if (sym_tbl == NULL || entrySTable == NULL) {
		return 0;
	}

	for (head = sym_tbl->symbol_List; head != NULL; head = head->next_sym) {
		if (head->type == _ENTRY) {
			addSymbolToTable(entrySTable, head);
			counter++;
		}
	}
	return counter;
}

/*adds an existatn symbol to a table*/
int addSymbolToTable(symbol_table_t *table, symbol_t *_symbol) {
		symbol_t *head = NULL;

		if (table == NULL) {
			return 0;
		}
		head = table->symbol_List;

		if (head == NULL) {
			table->symbol_List = _symbol;
			return 1;
		}else {
			while (head->next_sym != NULL) {
				head = head->next_sym;
			}
			head->next_sym = _symbol;
		}
		return 1;
}



void print_symbol_table(symbol_table_t *sym_tbl) {
	symbol_t *head = NULL;

	printf("Symbol Table:\n");

	if (sym_tbl == NULL || sym_tbl->symbol_List == NULL) {
		printf("SYMBOL_TABLE_EMPTY\n");
		return;
	}

	head = sym_tbl->symbol_List;

	while (head != NULL) {
		printf("name: %s , address: %d\n", head->symbol_name, head->address);
		head = head->next_sym;
	}
	printf("\n");
	return;
}
/*print to file ps.ext or stdout  extern information */
void printExternTable(word_table_t *table, FILE *file, int outputType) {
	line_t *line;
	int i;

	if (table == NULL) {
		return;
	}
	switch (outputType) {
		case 1: /* Print to stdout */
			for (i = 0; i < table->size; i++) {
				line = &table->lines[i];
				if (line->_ARE == E) {
					printf("%s  %4d\n", line->symbol->symbol_name, line->line_num);
				}
			}
			break;
		case 2: /* Print to file */
			if (file == NULL) {
				return;
			}
			for (i = 0; i < table->size; i++) {
				line = &table->lines[i];
				if (line->_ARE == E) {
					fprintf(file, "%s  %4d\n", line->symbol->symbol_name, line->line_num);
				}
			}
			break;
		default:
			return;
	}
}

/*print to file ps.ent or stdout  entry information - use a specific symbol table */
void printEntrySymbolTable(symbol_table_t *table, FILE *file, int outputType) {
		symbol_t *symbol;

		if (table == NULL) {
			return;
		}

		switch (outputType) {
			case 1: /* Print to stdout */
				for (symbol = table->symbol_List; symbol != NULL; symbol = symbol->next_sym) {
					printf("%s %04d\n", symbol->symbol_name, symbol->address);
				}
				break;
			case 2: /* Print to file */
				if (file == NULL) {
					return;
				}
				for (symbol = table->symbol_List; symbol != NULL; symbol = symbol->next_sym) {
					fprintf(file, "%s %04d\n", symbol->symbol_name, symbol->address);
				}
				break;
			default:
				return;
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