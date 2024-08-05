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
		{{"mov"},  {"-123"}, {"0123"}},
		{{"cmp"},  {"0123"}, {"0123"}},
		{{"add"},  {"-123"}, {"0123"}},
		{{"sub"},  {"-123"}, {"0123"}},
		{{"lea"},  {"-123"}, {"-1--"}},
		{{"clr"},  {"-123"}, {"----"}},
		{{"not"},  {"-123"}, {"----"}},
		{{"inc"},  {"-123"}, {"----"}},
		{{"dec"},  {"-123"}, {"----"}},
		{{"jmp"},  {"-12-"}, {"----"}},
		{{"bne"},  {"-12-"}, {"----"}},
		{{"red"},  {"-123"}, {"----"}},
		{{"prn"},  {"0123"}, {"----"}},
		{{"jsr"},  {"-12-"}, {"----"}},
		{{"rts"},  {"----"}, {"----"}},
		{{"stop"}, {"----"}, {"----"}}
};
int *IC;
int *DC;
parser_t parser_s;
static sep_commas_t seperator_c;
static sep_whitespace_t seperator_w;

void initParser_T() {
	parser_s.line_type = ERR;
}

void parse(symbol_table_t *sym_tbl) {
	char *buffer = calloc(LINE_LENGTH , sizeof(char));
	char *_line = calloc(LINE_LENGTH, sizeof(char));
	int *pos = calloc(1, sizeof(int));
	int isSymbol  = 0;
	line_count = 0;
	*IC = 100;
	initParser_T();


	if (ferror(fptr_after)) {
		printf("ERROR");
	}

	/*fprintf(fptr_after,"test this\n");*/
	if (sym_tbl == NULL) {
		report_error(ERR_FAIL_CREATE_SYMBOL, line_count, CRIT);
		return;
	}
	/*fgets(buffer, LINE_LENGTH, fptr_after) != NULL*/
	while (fgets(buffer, LINE_LENGTH, fptr_after) != NULL) {
		line_count++;
		*IC++;
		strcpy(_line, buffer);
		if((isSymbol = ifSymbol_n_Write(sym_tbl, buffer, pos))> 0)
			buffer += *pos;
		if(classify_line(buffer, pos)>0)
			buffer += *pos;




		printf("%s\n", buffer);

		switch(parser_s.line_type) {
			case OP_CODE:
				classify_opcode(sym_tbl , buffer ,pos);
				break;
			case DIRECTIVE:
				break;

			case ERR:
				report_error(ERR_LINE_UNRECOGNIZED,line_count,CRIT);
				break;
			case BLANK:
				break;
			default:
				break;
		}



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
	parser_s.line_type = ERR;
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

sep_commas_t string_comma_seps(char *str) {
	sep_commas_t seperator = {0};
	char *s;
	int seps_count = 0;
	if (*str == '\0') {
		return seperator;
	}
	do {
		seperator.cString[seps_count] = str;
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


void classify_opcode(symbol_table_t *sym_tbl , char *buffer ,int  *pos){
	seperator_c = string_comma_seps(buffer);
	if(seperator_c.isError == 1)
		report_error(ERR_OP_CODE_RECOGNITION, line_count,CRIT);








}

/*
void compare_commas(int type, int sep_count, int num_in_string)
{
	*/
/*usess ENUM "STOP_C, PRINT_C, SET_C, READ_C*//*

	switch (type)
	{
		case STOP_C:
			if (sep_count != 0)
			{
				strcpy(parser.err_buff, ERR_COMMA);
				printf("%s\n", parser.err_buff);
				parser.oper = ERROR_OPER;
			}
			break;
		case PRINT_C:
			if (sep_count != 1)
			{
				strcpy(parser.err_buff, ERR_COMMA);
				printf("%s\n", parser.err_buff);
				parser.oper = ERROR_OPER;
			}
			break;
		case SET_C:
			if (sep_count != 3 || comma_count != 2)
			{
				strcpy(parser.err_buff, ERR_COMMA);
				printf("%s\n", parser.err_buff);
				parser.oper = ERROR_OPER;
			}
			break;
		case READ_C:
			if (num_in_string == sep_count)
			{
				strcpy(parser.err_buff, ERR_COMMA);
				printf("%s\n", parser.err_buff);
				parser.oper = ERROR_OPER;
			}
			break;
		default:
			break;
	}
}
*/
