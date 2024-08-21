
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

static int IC = 0; /*first address of the instruction table is preset in tabel init*/
static int DC;


void second_pass( symbol_table_t *sym_tbl,  word_table_t *wordTable,
                  word_table_t *dataTable) {
	symbol_table_t *entryTable;
	line_t *current_line;
	symbol_t *symbol1, *symbol2;
	char *boo;
	int idx, result = 0, n = 0;
	if (isError == 1) {
		return;

	}
	entryTable = init_symbol_table(entryTable);

	if (parser.op == stop) {
		printf("Stop Occured\n");

		n = wordTable->size;
		printf(" DATA TABLE ADDRESS CHAGE :n: %d\n", n);
		addConstantToSymbols(sym_tbl, _DATA, n);
		addNumberToWordTable(dataTable, n);
		update_Reloc_Lines(wordTable);
		update_Reloc_Lines(dataTable);
		moveSymbolsToEntry(sym_tbl ,entryTable);
		printTable(wordTable);
		printf("-------------\n");
		printTable(dataTable);
		printf("------------\n");
		print_symbol_table(sym_tbl);
	}
}


void checkOPCODE_INSTRUCTION(symbol_table_t *sym_tbl, word_table_t *table) {


	line_t *line1 = &table->lines[IC];

	type_of_register_t type0 = parser.operands[0].type_of_register;
	type_of_register_t type1 = parser.operands[1].type_of_register;
	IC++;
	if (line1->symbol != NULL) {
		/*notifies about address problem*/
		if (line1->symbol->address < 100)
			report_error(WAR_MEMORY_NOT_CONFIGURED, line_count, SECOND, NON_CRIT, line1->symbol->address);

	}
	switch (register_count_by_op(parser.op)) {
		case 0:
			break;
		case 1:

			checkOPCODE_WORDS(sym_tbl, table, 1, 0);
			break;
		case 2:


			if (registerSelection() == 2) {
				/*all combinations except INDIRECT and Regular registry(4th type) type is zero*/
				checkOPCODE_WORDS(sym_tbl, table, 0, 0);
				checkOPCODE_WORDS(sym_tbl, table, 1, 0);
			} else
				/*INDIRECT and Regular registry, type is one*/
				checkOPCODE_WORDS(sym_tbl, table, 1, 1);
	}


}

void checkOPCODE_WORDS(symbol_table_t *sym_tbl, word_table_t *table, int idx, int type) {
	symbol_t *symbol2 = NULL;
	line_t *line2 = &table->lines[IC];

	IC++;
	/*the follow is type Direct Operand Check*/

	if(parser.operands[idx].type_of_register ==  _DIRECT) {
		if (line2->symbol != NULL) {
				report_error(ERR_SYMBOL_NOT_FOUND, line_count, SECOND, CRIT, 0);
				return;
		}
		if (symbol2->address < 100)
			report_error(WAR_MEMORY_NOT_CONFIGURED, line_count, SECOND, NON_CRIT, 0);

		set_label_into_empty_word(&(line2->word), symbol2->address);
	}
}


void checkDATA_WORDS(symbol_table_t *sym_tbl, word_table_t *table) {

	symbol_t *symbol = findSymbol(sym_tbl, parser.symbol_name);
	line_t *line = NULL;
	int i = 0;
	unsigned short num = 0;

	if (symbol != NULL && symbol->address == 0) {
		/*todo change to critical*/
		report_error(WAR_MEMORY_NOT_CONFIGURED, line_count, SECOND, NON_CRIT, line->line_num);
		return;
	}

	for (i = DC; i < parser.directive.operand.data_len; i++) {
		/*only the first line has a potential symbol*/
		line = &table->lines[i];
		if (i == DC && line->symbol != NULL && line->symbol->address < 100) {
			report_error(WAR_MEMORY_NOT_CONFIGURED, line_count, SECOND, NON_CRIT, line->line_num);
			DC++;
		}
		DC++;
	}
}

void checkSTRING_WORDS(symbol_table_t *sym_tbl, word_table_t *table) {

	int i = DC;
	line_t *line = NULL;

	for (; i < parser.directive.operand.data_len; i++) {
		line = &table->lines[i];
		if (i == DC && line->symbol != NULL && line->symbol->address <100 ) {
			report_error(WAR_MEMORY_NOT_CONFIGURED, line_count, SECOND, NON_CRIT, line->line_num);
		}
		DC++;/*raise count for the \0 inserted at the end of the command */

	}
}

void checkEXTERN(symbol_table_t *sym_tbl, word_table_t *table) {
	line_t *line = &table->lines[DC];
	symbol_t *symbol = line->symbol;

/* unlike the rest |symbol is a registry*/
	if (symbol != NULL) {
		DC++;
	} else
		report_error(ERR_EXTERN_SYMBOL, line_count, SECOND, CRIT, DC);
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
symbol_t *firstSymbolMissingValue(symbol_table_t *table) {
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
				if ((symbol2->type == _INSTRUCTION || symbol2->type == _DATA) &&
				    strcmp(symbol1->symbol_name, symbol2->symbol_name) == 0)
					return 1;

			}
		}
	}
	return 0;
}

/*entry has a specific table , all entry are moved to it*/


int moveSymbolsToEntry(symbol_table_t *sym_tbl ,symbol_table_t *entrySTable) {
	symbol_t *head = NULL;
	int counter = 0;

	if (sym_tbl == NULL || entrySTable == NULL) {
		printf("Symbol Table or Entry Symbol Table are Empty\n");
		return 0;
	}

	for (head = sym_tbl->symbol_List; head != NULL; head = head->next_sym) {
		if (head->type == _ENTRY) {
			addSymbolToTable(entrySTable, head);
			counter++;
		}
		printf("%d have been moved\n", counter);
	}
	return (counter == 0) ? 0:1;
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
	} else {
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
/*void printEntrySymbolTable(symbol_table_t *table, FILE *file, int outputType) {
		symbol_t *symbol;

		if (table == NULL) {
			return;
		}

		switch (outputType) {
			case 1: *//* Print to stdout *//*
				for (symbol = table->symbol_List; symbol != NULL; symbol = symbol->next_sym) {
					printf("%s %04d\n", symbol->symbol_name, symbol->address);
				}
				break;
			case 2: *//* Print to file *//*
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
	}*/



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

	if (table == NULL || table->size == 0) {
		return;
	}

	for (i = 0; i < table->size; i++) {
		printf("%05d\t", table->lines[i].line_num); /* Print line number with leading zeros */
		printf("%05d\t", table->lines[i]._ARE); /* Print line number with leading zeros */
		printBinary(table->lines[i].word);         /* Print word in binary */
	}
}



void update_Reloc_Lines(word_table_t *table) {
	int i, newSize = 0;
	line_t *line = NULL;

	if (table == NULL) {
		return;
	}
	for (i = 0; i < table->size; i++) {
		if (table->lines[i]._ARE == R) {
			line = &table->lines[i];
			if(line->symbol!=NULL){
				set_label_into_empty_word(&(line->word), line->symbol->address);
				set_ARE_into_word(&(line->word), R);
			}
			else
				report_error(ERR_SYMBOL_NOT_FOUND,line_count,SECOND,CRIT,0);
		}
	}
}
