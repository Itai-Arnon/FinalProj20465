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
#include "headers/error.h"


void collect_symbol_names(symbol_table_t *sym_tbl) {

	char *buffer = malloc(sizeof(char) * LINE_LENGTH);
	char *first_word = (char *) calloc(10, sizeof(char));
	char *first_word_cut = (char *) calloc(10, sizeof(char));
	char *str = (char *) calloc(10, sizeof(char));
	int *pos = calloc(1, sizeof(int));
	int idx = 0;
	int len = 0;
	line_count = 0;


	if (sym_tbl == NULL) {
		report_error(ERR_FAIL_CREATE_SYMBOL_TBL, line_count, NON_CRIT);
		return;
	}
	while (fgets(buffer, LINE_LENGTH, fptr_before) != NULL) {
		line_count++;
		idx = 0;

		if (isRestOfLineEmpty(buffer)) /*checks case of empty line*/
			continue;

		/*removes whitespace*/
		removeFrontalWhitespace(buffer, pos);
		buffer += *pos; /*advances the pointer to end of whitespace*/
		if (*buffer == ';' || *buffer == '\0')
			continue;


		if (sscanf(buffer, "%s", first_word) == 1) {
			len = strlen(first_word);
			if (first_word[len - 1] == ':') {
				printf("POSSIBLE LABEL %s\n", first_word);

				strncpy(first_word_cut, first_word, len - 1);
				loadSymbolTable(sym_tbl, first_word_cut, 0);

			}

		}
	}
}

/* todo 0 failure 1:success*/
int loadSymbolTable(symbol_table_t *sym_tbl, char symbol_name[], int address) {
	int res = 0;
	symbol_t *end = sym_tbl->symbol_List;
	symbol_t *node = create_symbol(symbol_name, address);/*create symbols takes care of error*/

	if (node == NULL) {
		return 0;
	}

	if (isDuplicateSymbol(sym_tbl, symbol_name)) {
		report_error(ERR_DUPLICATE_SYMBOL_NAME, line_count,
		             NON_CRIT);/*todo divide into duplicate and needed duplicate*/
		return 0;
	}
	if (end != NULL) {
		while (end->next_sym != NULL) {
			end = end->next_sym;
		}
		end->next_sym = node;
	} else
		sym_tbl->symbol_List = node;

	return 1;
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
		report_error(ERR_FAIL_CREATE_SYMBOL_TBL, line_count, CRIT);
	return NULL;
}

symbol_t *create_symbol(char symbol_name[], int address) {
	symbol_t *node = NULL;
	int LEN = strlen(symbol_name);
	if(symbol_name[LEN-1 == ':'])
		LEN-=1;

	if (node = malloc(sizeof(symbol_t))) {
		strncpy(node->symbol_name, symbol_name,LEN);
		node->address = 0;
		node->next_sym = NULL;
		printf("%s\n", symbol_name);
		return node;
	}
	report_error(ERR_FAIL_CREATE_SYMBOL, line_count, CRIT);
	return NULL;
}

/*-1 symbol list is empty, 0-no duplicants , 1 it is a duplicate*/
int isDuplicateSymbol(symbol_table_t *sym_tbl, char symbol_name[]) {
	symbol_t *head = sym_tbl->symbol_List;
	int LEN = strlen(symbol_name);


	while (head != NULL) {
		if (strncmp(symbol_name, head->symbol_name, LEN) == 0 || strncmp(symbol_name, head->symbol_name, LEN -1) == 0) {
			/*if symbol already exist returns 1*/
			return 1;
		}
		head = head->next_sym;
	}
	return 0; /*no duplicate*/
}

int checkForAddress(symbol_table_t *sym_tbl, char *symbol_name, int address, isUpdate n) {
	symbol_t *head = sym_tbl->symbol_List;

	while (head != NULL) {
		if ((strcmp(symbol_name, head->symbol_name)) == 0) {
			/*if symbol already exist returns 1*/
			if (head->address != 0)
				if (n == 0)
					return 1;
			if (n == 1) {
				head->address = address;
				return 1;
			}
		}
		head = head->next_sym;
	}
	return 0; /*no duplicate*/
}






/*pre processor errors are not treated  */
/*meant to create the symbol table for the purpose of the preprocessor scan */

/*TODO: I used sscanf to remove white space , create util that removes whitspace*/
void findLabel_n_load(symbol_table_t *sym_tbl, char *buffer, char ch) {
	char **arr = calloc(5, sizeof(char *));
	char *s;
	char no_whites[MAX_SYMBOL_NAME];
	int idx = 0;
	int length = 0;
	if (*buffer == '\0') return;
	do {
		arr[idx] = calloc(MAX_SYMBOL_NAME, sizeof(char));
		arr[idx] = buffer;
		idx++;
		s = strchr(buffer, ch);
		if (s) {
			*s = '\0';
			s++;
		}
		buffer = s;
	} while (s);
	length = idx;
	for (idx = 0; idx < length; idx++) {
		sscanf(arr[idx], "%s", no_whites);
		loadSymbolTable(sym_tbl, no_whites, 0);
		printf("%s\n", no_whites);
	}
}


