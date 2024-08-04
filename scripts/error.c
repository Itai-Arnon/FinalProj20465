#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "headers/macro.h"
#include "headers/macro_list.h"
#include "headers/shared.h"
#include "headers/global_vars.h"
#include "headers/utils.h"
#include "headers/symbols.h"
#include "headers/assembler.h"
#include "headers/error.h"


void report_error(char *err, int line_count, err_type type) {

	printf("%s at line %lu\n", err, line_count);
	if (type == CRIT) {
		printf("terminating: dev stage\n");
		printf("Critical Error, Exiting");
		exit(1);
	} else (printf("%s\n", "NON CRITICAL"));

}


void freeTables(macro_table_t *macro_tbl, symbol_table_t *sym_tbl) {
	symbol_t *sHead = sym_tbl->symbol_List;
	macro_node_t *mHead = macro_tbl->slot[0];

	/*fill here free table*/


}


