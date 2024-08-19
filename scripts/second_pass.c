
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

	printTableToFile(wordTable , dataTable , boo);

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

/*check if there are duplicate symbols*/
	void checkSymbolsUnique(macro_table_t *macro_table, symbol_table_t *sym_table) {
		macro_node_t *macro;
		symbol_t *symbol;
		int i = 0;

		for ( i = 0; i < macro_table->size; ++i) {
			macro = &macro_table->slot[i];
			for (symbol = sym_table->symbol_List; symbol != NULL; symbol = symbol->next_sym) {
				if (strcmp(macro->macro_name, symbol->symbol_name) == 0) {
					report_error(ERR_MACRO_NAME_OP_DIRECT_SYMBOL , line_count ,SECOND , CRIT);
					free(macro);
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

void printTableToFile(word_table_t *wTable ,word_table_t *dTable, char* file_name)
{
	int i = 0 , num = 0;
	if (wTable == NULL) {
		return;
	}
	printf("\n");

	printf("  %d  %d\n" ,wTable->size , dTable->size);
	for (i = 0; i < wTable->size; i++) {
		num =  convertToOctal(wTable->lines[i].word);
		printf("%05d %05d\n", wTable->lines[i].line_num , num); /* Print line number with leading zeros */

	}
	for (i = 0; i < dTable->size; i++) {
		num =  convertToOctal(dTable->lines[i].word);
		printf("%05d %05d\n", dTable->lines[i].line_num , num);
	}
	printf("\n");
}



