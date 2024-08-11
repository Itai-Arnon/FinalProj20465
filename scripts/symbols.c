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




		if (sscanf(buffer, "%s", first_word) == 1) {
			len = strlen(first_word);
			printf("%c\n",first_word[len-1]);
			if (first_word[len-1] == ':') {
				len -= 1;
				printf("POSSIBLE LABEL %s\n", first_word);
				strncpy(first_word_cut, first_word, len);
				/*set up as instruction memoery */
				loadSymbolTable(sym_tbl, first_word_cut, 0,0);

			}

		}
	}
}

/* todo 0 failure 1:success*/
int loadSymbolTable(symbol_table_t *sym_tbl, char symbol_name[], int address , memory_t type) {
	int res = 0;
	symbol_t *end = sym_tbl->symbol_List;
	symbol_t *node = create_symbol(symbol_name, address , type);/*create symbols takes care of error*/

	if (node == NULL) {
		return 0;
	}

	if (isDuplicateSymbol(sym_tbl, symbol_name)) {
		report_error(ERR_DUPLICATE_SYMBOL_NAME, line_count,
		             NON_CRIT);/*todo divide into duplicate and needed duplicate*/
		return 0;
	}
	sym_tbl->size++;
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

symbol_t *create_symbol(char symbol_name[], int address , memory_t type) {
	symbol_t *node = NULL;
	int LEN = strlen(symbol_name);
	if (symbol_name[LEN-1] == ':')
		LEN -= 1;

	if (node = malloc(sizeof(symbol_t))) {
		strncpy(node->symbol_name, symbol_name, LEN);
		node->address = 0;
		node->type = type;
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

	if (symbol_name[LEN - 1 ] == ':')
		LEN -= 1;

	while (head != NULL) {
		if (strncmp(symbol_name, head->symbol_name, (LEN)) == 0) {
			/*if symbol already exist returns 1*/
			return 1;
		}
		head = head->next_sym;
	}
	return 0; /*no duplicate*/
}
/*if YES it willl update override , NO just checck if addresss*/
int checkOrUpdateSymbolAddress(symbol_table_t *sym_tbl, char *symbol_name, int address, isUpdate_t yes_or_no) {
	symbol_t *head = sym_tbl->symbol_List;

	while (head != NULL) {
		if ((strcmp(symbol_name, head->symbol_name)) == 0) {
			/*if symbol already exist returns 1*/
			if (head->address != 0)
				if (NO == 0)
					return 1;
			if (YES == 1) {
				head->address = address;
				return 1;
			}
		}
		head = head->next_sym;
	}
	return 0; /*no duplicate*/
}
/*identifies it's a symbols and reports if it's duplicate or not 1 -dup 2-not dup*/
int if_Symbol_if_Duplicate(symbol_table_t *sym_tbl, char *cmd , int isStartOrMid) {
	int len = 0;
	len = strlen(cmd);
/*if isStartOrMid == 1 will not allow symbol w/o ':"*/
	if (cmd[len - 1] != ':' && isStartOrMid) {
		parser_s.line_type = ERR;
		return 0;
	}

	if (isDuplicateSymbol(sym_tbl, cmd) == 0) {
		strncpy(parser_s.symbol_name, cmd, len);
		return 1; /*not dupilcate return 1 */
	} else {
		strncpy(parser_s.symbol_name, cmd, len);
		return 2;/* dupilcate return 2*/
	}


}





/*pre processor errors are not treated  */
/*meant to create the symbol table for the purpose of the preprocessor scan */

/*TODO: I used sscanf to remove white space , create util that removes whitspace*/
void findLabel_n_load(symbol_table_t *sym_tbl, char *buffer, char ch) {
	char **arr = calloc(5, sizeof(char *));
	char *s;
	int x = 0;
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
	/*todo fix this*/
	/*for (idx = 0; idx < length; idx++) {
		strcpy(no_whites, arr[idx]);
		loadSymbolTable(sym_tbl, , 0. _INSTRUCTION);
		printf("%s\n", no_whites);
	}*/
}


