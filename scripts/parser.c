#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>
#include "headers/macro.h"
#include "headers/macro_list.h"
#include "headers/shared.h"
#include "headers/global_vars.h"
#include "headers/utils.h"
#include "headers/symbols.h"
#include "headers/parser.h"

char *opcode_specs[16][3][1] = {
		{{"mov"},  {"_123"}, {"0123"}},
		{{"cmp"},  {"0123"}, {"0123"}},
		{{"add"},  {"_123"}, {"0123"}},
		{{"sub"},  {"_123"}, {"0123"}},
		{{"lea"},  {"_123"}, {"_1__"}},
		{{"clr"},  {"_123"}, {"____"}},
		{{"not"},  {"_123"}, {"____"}},
		{{"inc"},  {"_123"}, {"____"}},
		{{"dec"},  {"_123"}, {"____"}},
		{{"jmp"},  {"_12_"}, {"____"}},
		{{"bne"},  {"_12_"}, {"____"}},
		{{"red"},  {"_123"}, {"____"}},
		{{"prn"},  {"0123"}, {"____"}},
		{{"jsr"},  {"_12_"}, {"____"}},
		{{"rts"},  {"____"}, {"____"}},
		{{"stop"}, {"____"}, {"____"}}
};

parser_t parser_s;
static sep_commas_t sep_s;

void initParser_T() {
	parser_s.line_type = ERR;
}


void parse(symbol_table_t *sym_tbl) {
	char buffer[LINE_LENGTH];
	char *_line = calloc(LINE_LENGTH, sizeof(char));
	line_count = 0;


	if (sym_tbl == NULL) {
		report_error(ERR_FAIL_CREATE_SYMBOL, line_count);
		return;
	}
	while (fgets(buffer, LINE_LENGTH, fptr_after) != NULL) {
		line_count++;
		strcpy(_line, buffer);
		printf("shit");
		classify_line(_line, sym_tbl);


	}

	/*function dividing into sentences*/

	/*function that classify which operation is involved*/
}

int classify_line(char *buffer, symbol_table_t *sym_tbl) {
	symbol_t *head = sym_tbl->symbol_List;
	char cmd[MAX_SYMBOL_NAME];/*support macro name with colon*/

	int j = 0;
	int len = 0;
	int pos = 0;

	if (sscanf(buffer, "%s%n:", cmd, pos) == 1) {
		printf("cmd %s, pos %d", cmd, pos);
		while (head != NULL) {
				if (strcmp(cmd, head->symbol_name) == 0) {
				strcpy(parser_s.label_name, cmd);
				break;
			}
			head = head->next_sym;
		}
		buffer += pos;
		/*case no label was found*/
		if(head == NULL)

		return 0;
	}
}
/*

	for ( j = 0; j < 16; ++j) {
		if(strcmp(macro_name,opcode_names[j]) == 0)
			return 1;
	}
	for ( j = 0; j < 4; ++j) {
		if(strcmp(macro_name,directives[j]) == 0)
			return 1;
	}*/

/*in case a colon appears in macro_name*/




