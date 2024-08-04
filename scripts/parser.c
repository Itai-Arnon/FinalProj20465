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
#include "headers/error.h"
static const char *directives[4] = {".data", ".string", ".extern", ".entry"};

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
	int *pos = NULL;
	line_count = 0;
	initParser_T();


	if (sym_tbl == NULL) {
		report_error(ERR_FAIL_CREATE_SYMBOL, line_count, CRIT);
		return;
	}
	while (fgets(buffer, LINE_LENGTH, fptr_after) != NULL) {
		line_count++;
		strcpy(_line, buffer);
		printf("shit");
		classify_line(sym_tbl,_line,  pos);


	}

	/*function dividing into sentences*/

	/*function that classify which operation is involved*/
}

void classify_line(symbol_table_t *sym_tbl, char *buffer, int *pos) {
	symbol_t *head = sym_tbl->symbol_List;
	char cmd[MAX_SYMBOL_NAME];/*support macro name with colon*/
	char cmd_no_colon[MAX_SYMBOL_NAME];
	int j = 0;
	int len = 0;
	*pos = 0;
/*todo check premise of having all/some labels in label LL*/
	if (sscanf(buffer, "%s%n:", cmd, pos) == 1) {
		printf("cmd %s, pos %d", cmd, pos);
		for (j = 0; j < 16; ++j) {
			if (strcmp(cmd, opcode_specs[j][0][0]) == 0) {
				parser_s.line_type = OP_CODE;
				return;
			}
		}
		for (j = 0; j < 4; ++j) {
			if (strcmp(cmd, directives[j]) == 0)
				parser_s.line_type = DIRECTIVE;
			return;
		}
	}
}

int isLabel(symbol_table_t *sym_tbl, char *buffer, int *pos) {
	symbol_t *head = sym_tbl->symbol_List;
	char cmd[MAX_SYMBOL_NAME];/*support macro name with colon*/
	char cmd_no_colon[MAX_SYMBOL_NAME];
	*pos = 0;
	int len = 0;

	if (sscanf(buffer, "%s%n:", cmd, pos) == 1) {
		len = strlen(cmd);
		if(cmd[len-1] !=':')
			return 0;
		else {
			removeColon(cmd,cmd_no_colon);
		}
			while (head != NULL) {
				if (strcmp(cmd_no_colon, head->symbol_name) == 0) {
					strcpy(parser_s.symbol_name, cmd);
					if (loadSymbolTable(sym_tbl, cmd, 0) == 1) {
						printf("Symbol Added\n");
					}
					break;
				}
				head = head->next_sym;
			}
			/*todo possible: check if symbol exists - or defer later to entire analsys*/

			/*case  label was found head is not null continue check but advance position*/
			if (head != NULL) {
				memset(cmd, '\0', sizeof(cmd));
				buffer += *pos;
				return 1;
			}
		}
		parser_s.line_type = ERR;
	}


	/*label was found continue scan*/
	/*if (sscanf(buffer, "%s%n\n", cmd, pos) == 1) {
		for (j = 0; j < 16; ++j) {
			if (strcmp(cmd, opcode_specs[j][0][0]) == 0) {
				parser_s.line_type = OP_CODE;
				buffer+=*pos;
				return;
			}
		}
		for (j = 0; j < 4; ++j) {
			if (strcmp(cmd, directives[j]) == 0)
				parser_s.line_type = DIRECTIVE;
				buffer+=*pos;
			    return;
		}

	}
	parser_s.line_type = ERR;
}*/

/*in case a colon appears in macro_name*/




