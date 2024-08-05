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
static sep_commas_t sepcomma_s;
static sep_whitespace_t seperator_s;

void initParser_T() {
	parser_s.line_type = ERR;
}

void parse(symbol_table_t *sym_tbl) {
	char buffer[LINE_LENGTH];
	char *_line = calloc(LINE_LENGTH, sizeof(char));
	int *pos = calloc(1, sizeof(int));
	line_count = 0;
	initParser_T();

	if (ferror(fptr_after)) {
		printf("ERROR");
	}

	/*fprintf(fptr_after,"test this\n");*/
	if (sym_tbl == NULL) {
		report_error(ERR_FAIL_CREATE_SYMBOL, line_count, CRIT);
		return;
	}
	while (fgets(buffer, LINE_LENGTH, fptr_after) != NULL) {
		line_count++;
		strcpy(_line, buffer);
		ifSymbol_n_Write(sym_tbl, _line, pos);
		classify_line(_line, pos);
		fprintf(fptr_after, "test this\n");

		if (ferror(fptr_after)) {
			printf("ERROR");
		}


		/*function dividing into sentences*/

		/*function that classify which operation is involved*/
	}
}

int classify_line(char *buffer, int *pos) {
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
				return 1;
			}
		}
		for (j = 0; j < 4; ++j) {
			if (strcmp(cmd, directives[j]) == 0)
				parser_s.line_type = DIRECTIVE;
			return 2;
		}
	}
	return 0;
}

int ifSymbol_n_Write(symbol_table_t *sym_tbl, char *buffer, int *pos) {
	char cmd[MAX_SYMBOL_NAME];/*support macro name with colon*/
	int len = 0;
	*pos = 0;

	if (sscanf(buffer, "%s%n:", cmd, pos) == 1) {
		len = strlen(cmd);
		if (cmd[len - 1] != ':')
			return 0;
		else {
			if (isDuplicateSymbol(sym_tbl, cmd) == 1) {
				strncpy(parser_s.symbol_name, cmd, len);
				buffer += *pos;
				return 1;
			} else {
				if (loadSymbolTable(sym_tbl, cmd, 0) == 1)
					printf("Symbol Added\n");
				buffer += *pos;
				return 1;
			}
		}
	}
	parser_s.line_type = ERR;
	return 0;
}


sep_whitespace_t string_sep(char *line) {
	int strings_count = 0;
	char *s;
	sep_whitespace_t seperator = {0};
	while (isspace(*line)) line++;
	if (*line == '\0') {
		return seperator;
	}

	do {
		seperator.wString[strings_count++] = line;
		/*strpbrk refer to the constant SPACES*/
		s = strpbrk(line, SPACES);
		if (s) {
			*s = '\0';
			s++;
			while (isspace(*s))s++;
			if (*s == '\0')
				break;
			line = s;
		} else {
			break;
		}
	} while (1);
	seperator.counter = strings_count;
	return seperator;
}

sep_commas_t get_comma_seps(char *str) {
	sep_commas_t seperator = {0};
	char *s;
	int seps_count = 0;
	if (*str == '\0') {
		return seperator;
	}
	do {
		seperator.sString[seps_count] = str;
		seps_count++;
		s = strchr(str, ',');
		if (s) {
			*s = '\0';
			s++;
			if (*s == ',') {
				seperator.isError = 1;
				return seperator;
			}
		}
		str = s;
	} while (s);
	seperator.counter = seps_count;
	return seperator;
}



