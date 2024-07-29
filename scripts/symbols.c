#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "headers/symbols.h"
#include "headers/global_vars.h"
#include "headers/macro_list.h"
#include "headers/shared.h"
#include "headers/global_vars.h"
#include "headers/utils.h"


void collect_symbol_names(symbol_table_t *sym_tbl) {

	char buffer[LINE_LENGTH];
	char *first_word = (char *) calloc(10, sizeof(char));
	char *first_word_cut = (char *) calloc(10, sizeof(char));
	int idx = 0;
	int len = 0;
	line_count = 0;

	if (sym_tbl == NULL) {
		report_error(ERR_FAIL_CREATE_SYMBOL, line_count);
		return;
	}
	while (fgets(buffer, LINE_LENGTH, fptr_before) != NULL) {
		line_count++;
		idx = 0;

		if (isRestOfLineEmpty(buffer)) /*checks case of empty line*/
			continue;

		/*removes whitespace*/
		while (isspace(buffer[idx]) && idx < LINE_LENGTH) ++idx;
		if (buffer[idx] == ';')
			continue;

		if (sscanf(buffer, "%s", first_word) == 1) {
			len = strlen(first_word);
			if (first_word[len - 1] == ':') {
				printf("POSSIBLE LABEL %s\n", first_word);

				strncpy(first_word_cut,first_word,len-1);
				loadSymbolTable(sym_tbl, first_word_cut, 0);

			}

		}
	}
}

void loadSymbolTable(symbol_table_t *sym_tbl, char symbol_name[], int address) {
	symbol_t *end = sym_tbl->symbol_List;
	symbol_t *node =  create_symbol(symbol_name, address);/*create symbols takes care of error*/

	if (node == NULL) {
		return;
	}
	if (end != NULL) {
		while (end->next_sym != NULL) {
			end = end->next_sym;
		}
		end->next_sym = node;
	} else
		sym_tbl->symbol_List = node;
	return;
}

void print_symbol_table(symbol_table_t *sym_tbl) {
	symbol_t *head = NULL;

	printf("Symbol Table:\n");

	if (sym_tbl == NULL || sym_tbl->symbol_List == NULL) {
		printf("SYMBOL_TABLE_EMPTY");
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

symbol_table_t *init_symbol_table(symbol_table_t *sym_tbl) {
	sym_tbl = calloc(1, sizeof(symbol_table_t));
	if (sym_tbl != NULL) {
		sym_tbl->size = 0;
		sym_tbl->symbol_List = NULL;
		return sym_tbl;
	} else
		report_error(ERR_FAIL_CREATE_SYMBOL_TBL, line_count);
	return NULL;
}

symbol_t *create_symbol(char symbol_name[], int address) {
	symbol_t *node = NULL;
	if (node = malloc(sizeof(symbol_t))) {
		strcpy(node->symbol_name, symbol_name);
		node->address = 0;
		node->next_sym = NULL;
		printf("%s\n",symbol_name);
		return node;
	}
		report_error(ERR_FAIL_CREATE_SYMBOL, line_count);
		return NULL;
	}

/*pre processor errors are not treated  */
/*meant to create the symbol table for the purpose of the preprocessor scan */



