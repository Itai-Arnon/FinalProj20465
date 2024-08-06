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

static char *directives[4] = {".data", ".string", ".extern", ".entry"};
static directive_enums direct_enums[4] = {DATA, STRING, ENTRY, EXTERN};
static direct_arr_t directArray[4];


static op_code op_code_enums[16] = {mov, cmp, add, sub, lea, clr, not, inc, dec, jmp,
                                    bne, red, prn, jsr, rts, stop};
static opcode_arr_t opcodeArray[16];

static char *opcode_specs[16][3][1] = {
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
		{{"stop"}, {"----"}, {"----"}}};
int *IC;
int *DC;
parser_t parser_s;
static sep_commas_t seperator_c;
static sep_whitespace_t seperator_w;

void parse(symbol_table_t *);

void initParser_T();

/* fills parser and checks  if concurs with op_code (1) or directive(2) (0) failuter*/
int classify_line(char *cmd );

/*fills parser and checks if symbol- then returns 1  non duplicate , 2 for duplicate
 *  0 for fail */
int if_Symbol_if_Duplicate(symbol_table_t *sym_tbl ,char *cmd);

/* creates an array of separated strings using strpbrk*/
sep_whitespace_t string_sep_white_space(char *);

void classify_opcode(symbol_table_t *sym_tbl, char *buffer, int *pos);

sep_commas_t string_comma_seps(char *str);

void initEnumArr();

void initDirectiveArray();


void parse(symbol_table_t *sym_tbl) {
	char *buffer = calloc(LINE_LENGTH, sizeof(char));
	char *_line = calloc(LINE_LENGTH, sizeof(char));
	char *cmd = calloc(MAX_SYMBOL_NAME, sizeof(char));
	int *pos = calloc(1, sizeof(int));
	int isSymbol = 0;
	line_count = 0;
	*IC = 100;
	initParser_T();
	initEnumArr();
	initDirectiveArray();/*todo check if needs more*/


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
		if (sscanf(buffer, "%s%n:", cmd, pos) == 1)

			if ((isSymbol = if_Symbol_if_Duplicate(sym_tbl,  cmd)) > 0) {
				buffer += *pos;
				if (isSymbol == 1) loadSymbolTable(sym_tbl,cmd , *IC);
			}
		if (classify_line(cmd) > 0)
			buffer += *pos;

		printf("%s\n", buffer);

		switch (parser_s.line_type) {
			case OP_CODE:
				classify_opcode(sym_tbl, buffer, pos);
				break;
			case DIRECTIVE:
				break;

			case ERR:
				report_error(ERR_LINE_UNRECOGNIZED, line_count, CRIT);
				break;
			case BLANK:
				break;
			default:
				break;
		}


	}

}


int classify_line( char *cmd) {

	int j = 0;
	int len = 0;

/*todo check premise of having all/some labels in label LL*/

	for (j = 0; j < 16; ++j) {
		if (strcmp(cmd, opcodeArray[j].opcode_name) == 0) {
			parser_s.line_type = OP_CODE;
			parser_s.operands[0].op = opcodeArray[j].op_num;
			return 1;
		}
	}
	for (j = 0; j < 4; ++j) {
		if (strcmp(cmd, directArray[j].direct_name) == 0)
			parser_s.line_type = DIRECTIVE;
		parser_s.directive.d_enum = directArray[j].dir_num;
		return 1;
	}

	parser_s.line_type = ERR;
	return 0;
}

int if_Symbol_if_Duplicate(symbol_table_t *sym_tbl, char *cmd) {
	int len = 0;
	len = strlen(cmd);

	if (cmd[len - 1] != ':') {
		parser_s.line_type = ERR;
		return 0;
	}

	if (isDuplicateSymbol(sym_tbl, cmd) == 0) {
		strncpy(parser_s.symbol_name, cmd, len);
		return 1; /*not dupilcate return 1 */
	}else{
		strncpy(parser_s.symbol_name, cmd, len);
		return 2;/* dupilcate return 2*/
	}


}


void classify_opcode(symbol_table_t *sym_tbl, char *buffer, int *pos) {
	seperator_c = string_comma_seps(buffer);
	if (seperator_c.isError == 1)
		report_error(ERR_OP_CODE_RECOGNITION, line_count, CRIT);

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

/* Function to initialize and populate an array of enum_arr*/
void initEnumArr() {
	/*Declare an array of enum_arr with a length of 16*/

	/*Populate the array*/
	for (int i = 0; i < 16; ++i) {
		opcodeArray[i].opcode_name = opcode_specs[i][0][0];
		opcodeArray[i].op_num = op_code_enums[i];
	}
}

/*Function to initialize and populate the directArray*/
void initDirectiveArray() {
	for (int i = 0; i < 4; ++i) {
		directArray[i].direct_name = directives[i];
		directArray[i].dir_num = direct_enums[i];
	}
}


void printParserContent() {
	/*Print line type*/
	printf("Line Type: ");
	switch (parser_s.line_type) {
		case OP_CODE:
			printf("OP_CODE\n");
			break;
		case DIRECTIVE:
			printf("DIRECTIVE\n");
			break;
		case ERR:
			printf("ERR\n");
			break;
		case BLANK:
			printf("BLANK\n");
			break;
		default:
			printf("UNKNOWN\n");
			break;
	}

	/*Print symbol name*/
	printf("Symbol Name: %s\n", parser_s.symbol_name);

	// Print directive details if line type is DIRECTIVE
	if (parser_s.line_type == DIRECTIVE) {
		printf("Directive Enum: %d\n", parser_s.directive.d_enum);
		printf("Directive Operand Symbol: %s\n", parser_s.directive.operand.symbol);
		printf("Directive Operand Data Length: %d\n", parser_s.directive.operand.data_len);
		printf("Directive Operand String: %s\n", parser_s.directive.operand.str);
	}

	/*Print operands details if line type is OP_CODE*/
	if (parser_s.line_type == OP_CODE) {
		for (int i = 0; i < 2; ++i) {
			printf("Operand %d Type: ", i);
			switch (parser_s.operands[i].type) {
				case NO_OPER:
					printf("NO_OPER\n");
					break;
				case NUMBER:
					printf("NUMBER\n");
					break;
				case LABEL:
					printf("LABEL\n");
					break;
				case REGISTR:
					printf("REGISTR\n");
					break;
				default:
					printf("UNKNOWN\n");
					break;
			}
			printf("Operand %d Value: ", i);
			switch (parser_s.operands[i].type) {
				case NO_OPER:
					printf("N/A\n");
					break;
				case NUMBER:
					printf("%d\n", parser_s.operands[i].operand.num);
					break;
				case LABEL:
					printf("%s\n", parser_s.operands[i].operand.symbol);
					break;
				case REGISTR:
					printf("%d\n", parser_s.operands[i].operand.registry);
					break;
				default:
					printf("UNKNOWN\n");
					break;
			}
		}
	}
}

void initParser_T() {
	parser_s.line_type = ERR;
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
