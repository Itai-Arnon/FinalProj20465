
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


void second_pass(symbol_table_t *sym_tbl, word_table_t *wordTable, word_table_t *dataTable) {
	int idx;
	line_t *current_line;
	symbol_t *symbol1, *symbol2;


	if (isError) {
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
				if(symbol2->are == E)
					continue;

				if (symbol2->address != 0  ) {
					symbol1->address = symbol2->address;
				}
			}


				}
			}


/*check if there are duplicate symbols*/
void checkSymbolsUnique(symbol_table_t *sym_table) {
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



