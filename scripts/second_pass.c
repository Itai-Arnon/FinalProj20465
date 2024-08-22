
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
#include "headers/assembler.h"

static int IC = 0; /*first address of the instruction table is preset in tabel init*/
static int DC;


void second_pass(symbol_table_t *sym_tbl, word_table_t *wordTable,
                 word_table_t *dataTable, char *filename) {
	symbol_table_t *entryTable = NULL;
	char *boo, *filename1, *filename2, *filename3, *filename4;

	int idx, result = 0, n = 0;
	if (isError == 1) {
		return;
	}
	entryTable = init_symbol_table(entryTable);
	filename1 = "..\\ps.ob.txt";
	filename2 = "..\\ps.ent.txt";
	filename3 = "..\\ps.ext.txt";
	filename4 = "..\\ps.ob.txt";

	if (parser.op == stop) {
		printf("Stop Occured\n");
		if (wordTable->size + dataTable->size > MAX_WORDS) {
			report_error(ERR_TOO_MANY_WORDS, line_count, SECOND, CRIT, 0);
			return;
		}
		n = wordTable->size + _offset;
		printf(" DATA TABLE ADDRESS CHAGE :n: %d\n", n);
		addNumberToWordTable(dataTable, n);
		addConstantToSymbols(sym_tbl, _DATA, n);
		addConstantToSymbols(sym_tbl, _EXTERN, n);
		addConstantToSymbols(sym_tbl, _ENTRY, n);
		updateRelocatableLines(wordTable);
		updateRelocatableLines(dataTable);
		moveSymbolsToEntry(sym_tbl, entryTable);
		printTable(wordTable);
		printf("-------------\n");
		printTable(dataTable);
		printf("------------\n");
		print_symbol_table(sym_tbl);
		/*printTableToFile(wordTable, dataTable, filename1);*/
		printEntryTable(entryTable, filename2);
		printExternTable(wordTable,dataTable, filename3);
		printTableToFile(wordTable, dataTable, filename4);
	}
	freeSymbolTable(sym_tbl);
	freeSymbolTable(entryTable);
	freeWordTable(wordTable);
	freeWordTable(dataTable);
	free(filename1);
	free(filename2);
	free(filename3);
	free(filename4);

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

/*moves all entry symbols to a new table - */
int moveSymbolsToEntry(symbol_table_t *sym_tbl, symbol_table_t *entryTable) {
	symbol_t *head = NULL;
	int counter = 0;

	if (sym_tbl == NULL || entryTable == NULL) {
		printf("Symbol Table or Entry Symbol Table are Empty\n");
		return 0;
	}

	for (head = sym_tbl->symbol_List; head != NULL; head = head->next_sym) {
		if (head->type == _ENTRY) {
			addSymbolToTable(entryTable, head);
			counter++;
		}
		printf("%d have been moved\n", counter);
	}
	return (counter == 0) ? 0 : 1;
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


/*print to file ps.ent or stdout  entry information - use a specific symbol table */
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


/*assures all relocatables have and address*/
void updateRelocatableLines(word_table_t *table) {
	int i, newSize = 0;
	line_t *line = NULL;

	if (table == NULL) {
		return;
	}
	for (i = 0; i < table->size; i++) {
		if (table->lines[i]._ARE == R) {
			line = &table->lines[i];
			if (line->symbol != NULL) {
				set_label_into_empty_word(&(line->word), line->symbol->address);
				set_ARE_into_word(&(line->word), R);
			} else
				report_error(ERR_SYMBOL_NOT_FOUND, line_count, SECOND, CRIT, 0);
		}
	}
}

/*count the number of Extern element in a table */
int countExternInTables(word_table_t *table) {
	int count = 0;
	int i;

/* Check if the table is not NULL its size is not zero */
	if (table != NULL && table->size > 0) {
		for (i = 0; i < table->size; i++) {
			if (table->lines[i]._ARE == E) {
				count++;
			}
		}
	}
	return count;
}


void printEntryTable(symbol_table_t *table, char *filename) {
	FILE *exPtr = NULL;
	symbol_t *symbol;

	if (table == NULL || filename[0] == '\0') {
		report_error("Entry Table Empty or File Name Invalid\n\n",line_count,SECOND,CRIT,0);
		return;
	}
	if(table->size == 0) {
		printf("Entry Symbol Table Is Empty\n");
		return;
	}
	if (!(exPtr = fopen(filename, "w+"))) {
		report_error(ERR_FILE_DESTINATION, line_count, SECOND, CRIT, 0);
		return;
	}

	for (symbol = table->symbol_List; symbol != NULL; symbol = symbol->next_sym) {
		printf("%s  %04d\n", symbol->symbol_name, symbol->address);
		fprintf(exPtr, "%s  %04d\n", symbol->symbol_name, symbol->address);
	}
	fclose(exPtr);
	return;
}

/*print to file ps.ext or stdout  extern information */
void printExternTable(word_table_t *table1, word_table_t *table2, char *filename) {
	FILE *exPtr = NULL;
	line_t *line;
	int i;

	if ( table1 == NULL || table2 == 0 || filename[0] == '\0') {
		report_error("Extern Table Empty Tables are Invalid or File Invalid\n\n" , line_count,SECOND,CRIT,0);
		return;
	}
	if (!(exPtr = fopen(filename, "w+"))) {
		report_error(ERR_FILE_DESTINATION, line_count, SECOND, CRIT, 0);
		return;
	}

	if(countExternInTables(table1) > 0 ) {
		for (i = 0; i < table1->size; i++) {
			line = &table1->lines[i];
			if (line->_ARE == E) {
				printf("%s  %4d\n", line->symbol->symbol_name, line->line_num);
				fprintf(exPtr, "%s  %4d\n", line->symbol->symbol_name, line->line_num);
			}
		}
	}

	if(countExternInTables(table2) > 0 ) {
		for (i = 0; i < table2->size; i++) {
			line = &table2->lines[i];
			if (line->_ARE == E) {
				printf("%s  %4d\n", line->symbol->symbol_name, line->line_num);
				fprintf(exPtr, "%s  %4d\n", line->symbol->symbol_name, line->line_num);
			}
		}
	}
	fclose(exPtr);
	return;
}


void print_symbol_table(symbol_table_t *sym_tbl) {
	symbol_t *head = NULL;

	printf("Symbol Table:\n");

	if (sym_tbl == NULL || sym_tbl->symbol_List == NULL) {
		printf("SYMBOL_TABLE_EMPTY\n\n");
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


void printTableToFile(word_table_t *wTable, word_table_t *dTable, char *filename) {
	FILE *exPtr = NULL;
	int i = 0, num = 0;

	if (wTable == NULL || dTable == NULL || filename[0] == '\0') {
		report_error("Word Table or Data Table Empty or File Invalid\n\n",line_count,SECOND,CRIT,0);
		return;
	}
	if (!(exPtr = fopen(filename, "w+"))) {
		report_error(ERR_FILE_DESTINATION, line_count, SECOND, CRIT, 0);
		return;
	}

	printf("\n");
	fprintf(exPtr, "  %d  %d\n", wTable->size, dTable->size);/*the size of the table*/
	for (i = 0; i < wTable->size; i++) {
		num = convertToOctal(wTable->lines[i].word);
		printf("%05d %05d\n", wTable->lines[i].line_num, num); /* Print line number with leading zeros */
		fprintf(exPtr, "%05d %05d\n", wTable->lines[i].line_num, num); /* Print line number with leading zeros */

	}
	for (i = 0; i < dTable->size; i++) {
		num = convertToOctal(dTable->lines[i].word);
		printf("%05d %05d\n", dTable->lines[i].line_num, num);
		fprintf(exPtr, "%05d %05d\n", dTable->lines[i].line_num, num);
	}
	printf("\n");
	fclose(exPtr);
}


	
