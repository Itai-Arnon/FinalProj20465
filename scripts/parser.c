#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>
#include "headers/macro.h"
#include "headers/macro_list.h"
#include "headers/shared.h"
#include "headers/global_vars.h"
#include "headers/utils.h"


const char* opcode_specs[16][3][1];
parser_t parser;
static string_separator_t separator;

void initParser_T();






void parse(symbol_table_t*sym_tbl){
	char buffer[LINE_LENGTH];
	line_count = 0;

	if (sym_tbl == NULL) {
		report_error(ERR_FAIL_CREATE_SYMBOL, line_count);
		return;
	}
	while (fgets(buffer,LINE_LENGTH,fptr_after) != NULL) {
		line_count++;


	}

	/*function dividing into sentences*/

	/*function that classify which operation is involved*/




}

