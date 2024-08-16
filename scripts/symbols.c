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


char *opcode_names[16] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp",
                          "bne", "red", "prn", "jsr", "rts", "stop"};

char *directives[4] = {".data", ".string", ".extern", ".entry"};



/* todo 0 failure 1:success*/
int loadSymbolTable(symbol_table_t *sym_tbl, char symbol_name[], int address, memory_t type) {
	int res = 0;
	symbol_t *end = sym_tbl->symbol_List;
	symbol_t *node = create_symbol(sym_tbl , symbol_name, address, type);/*create symbols takes care of error*/

	if (node == NULL) {
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

symbol_t*  findSymbol(symbol_table_t *sym_tbl , char *symbol_name){
	symbol_t *head = sym_tbl->symbol_List;
	int LEN = strlen(symbol_name);

	if(sym_tbl != NULL){
		LEN = (symbol_name[LEN - 1] == ':' ) ? LEN - 1 : LEN;

		while (head != NULL) {
			if (strncmp(head->symbol_name, symbol_name, LEN) == 0)
				return head;
			else
				head = head->next_sym;
		}
	}
	return NULL;
}

symbol_t *create_symbol(symbol_table_t *sym_tbl , char symbol_name[], int address, memory_t type) {
	symbol_t *node = NULL;
	int LEN = strlen(symbol_name);
	if (symbol_name[LEN - 1] == ':')
		LEN -= 1;
	/*check similarity with opcodes and directives*/
	if (is_symbol_name_duplicate(sym_tbl, symbol_name) == 1) {
	report_error(ERR_DUPLICATE_SYMBOL_NAME, line_count, CRIT);
	return NULL;
}

	if (node = malloc(sizeof(symbol_t))) {
		strncpy(node->symbol_name, symbol_name, LEN);
		node->address = 0;
		node->type = type;
		node->are = A;
		node->next_sym = NULL;
		printf("%s\n", symbol_name);
		return node;
	}
	report_error(ERR_FAIL_CREATE_SYMBOL, line_count, CRIT);
	return NULL;
}

/*general func |-1 symbol list is empty, 0-no duplicants , 1 it is a duplicate*/
int isDuplicateSymbol(symbol_table_t *sym_tbl, char symbol_name[]) {
	symbol_t *head = sym_tbl->symbol_List;
	int LEN = strlen(symbol_name);

	if (symbol_name[LEN - 1] == ':')
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
/*identifies it's a symbols and reports if it's duplicate or not 1 -dup 2-not dup
 * isHeadOrMid selects between beginning label or those that are inside */
int if_Symbol_if_Duplicate(symbol_table_t *sym_tbl, char *cmd, symbol_loci_t isHEadOrMid) {
	int len = 0;
	len = strlen(cmd);
/*if isStartOrMid == 1 will not allow symbol w/o ':"*/
/*HEAD includes MIDDLE*/
	switch (isHEadOrMid) {
		case HEAD:
			if (cmd[len - 1] != ':') {
				parser.line_type = ERR;
				return 0;
			}
		case MIDDLE:
			if (isDuplicateSymbol(sym_tbl, cmd) == 0) {
				strncpy(parser.symbol_name, cmd, len);
				return 1; /*not dupilcate return 1 */
			} else {
				strncpy(parser.symbol_name, cmd, len);
				return 2;/* dupilcate return 2*/
			}
	}
}

/*meant to detect if name is a duplicate directive or opcode or prior labels are */
int is_symbol_name_duplicate(symbol_table_t *sym_tbl , char *symbol_name) {
	symbol_t *head = sym_tbl->symbol_List;
	int LEN = strlen(symbol_name);
	int j = 0;

	if(symbol_name[0] == '\0') {
		return 0;
	}

	for (j = 0; j < 16; ++j) {
		if (strcmp(symbol_name, opcode_names[j]) == 0)
			return 1;
	}
	for (j = 0; j < 4; ++j) {
		if (strcmp(symbol_name, directives[j]) == 0)
			return 1;
	}

		LEN = (symbol_name[LEN - 1] == ':' ) ? LEN - 1 : LEN;
		while (head != NULL) {
			if (strncmp(head->symbol_name, symbol_name, LEN)  == 0 )
				return 1;
			else
				head = head->next_sym;
		}

	return 0;
}

int delete_symbol(symbol_table_t *sym_tbl , char *symbol_name){
	symbol_t *head = sym_tbl->symbol_List ;
	symbol_t *prev = NULL;
	int LEN = strlen(symbol_name);

	if (symbol_name[LEN - 1] == ':')
		LEN -= 1;

	while (head!= NULL) {
		if (strncmp(symbol_name, head->symbol_name, (LEN)) == 0) {
			if(prev != NULL)
				prev->next_sym = head->next_sym;
			free(head);
			head = prev;
			/*if deletion successful returns 1*/
			return 1;
		}
		prev = head;
		head = head->next_sym;
	}

	return 0; /*not found*/



}





