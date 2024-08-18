
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers/second_pass.h"
#include "headers/macro.h"
#include "headers/shared.h"
#include "headers/global_vars.h"
#include "headers/utils.h"
#include "headers/symbols.h"
#include "headers/parser.h"
#include "headers/first_pass.h"
#include "headers/assembler.h"


void second_pass(macro_table_t *macroTable, symbol_table_t *sym_tbl, word_table_t *wordTable, word_table_t *dataTable) {
	int idx;
	line_t *current_line;
	symbol_t *symbol1, *symbol2;
	char * boo;

	printTableToFile(wordTable ,boo );
	/*if (isError) {
		return;
	}

	for (idx = 0; idx < wordTable->size; ++idx) {
		current_line = &wordTable->lines[idx];
		symbol1 = current_line->symbol;

		if (symbol1 != NULL && symbol1->address == 0) {
			symbol2 = findSymbol(sym_tbl, symbol1->symbol_name);
			if (symbol2 == NULL) {
				report_error(ERR_SYMBOL_NOT_FOUND, line_count, CRIT);
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





/*check if there are duplicate symbols*/
void checkSymbolsUnique(macro_table_t *macroTable, symbol_table_t *sym_table) {
	symbol_t *head, *checker;


	for (head = sym_table->symbol_List; head != NULL; head = head->next_sym) {
		for (checker = head->next_sym; checker != NULL; checker = checker->next_sym) {
			if (strcmp(head->symbol_name, checker->symbol_name) == 0) {
				report_error(ERR_DUPLICATE_SYMBOL_NAME, line_count, CRIT);
				return;
			}
		}
	}
}

int int_to_octal(int num) {
	int ans = 0, y = 1;

	while (num != 0) {
		ans += (num % 8) * y;
		num = num / 8;
		y *= 10;
	}
	return ans;
}

void printTableToFile(word_table_t *table , char* file_name)
{
	int i = 0 , num = 0;


	if (table == NULL) {
		return;
	}

	for (i = 0; i < table->size; i++) {
		num =  int_to_octal(table->lines[i].word);
		printf("%04d\t", table->lines[i].line_num); /* Print line number with leading zeros */
		printf("%04d\t", num);
		printf("\n");
		/*printBinary(table->lines[i].word);      */   /* Print word in binary */
	}
}



