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


static op_code_t op_code_enums[16] = {mov, cmp, add, sub, lea, clr, not, inc, dec, jmp,
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

void initParser();

/* fills parser and checks  if concurs with op_code_t (1) or directive(2) (0) failuter*/
int classify_line(char *cmd);

void classify_opcode(symbol_table_t *sym_tbl, char *buffer);

/*fills parser and checks if symbol- then returns 1  non duplicate , 2 for duplicate
 *  0 for fail */
int if_Symbol_if_Duplicate(symbol_table_t *sym_tbl, char *cmd);


/* creates an array of separated strings using strpbrk*/
sep_whitespace_t string_sep_white_space(char *);

void classify_opcode(symbol_table_t *sym_tbl, char *buffer);
void checkRegisterCompliance();
int checkRegisterCount(op_code_t op);

		sep_commas_t string_comma_seps(char *str);

type_of_register_t classifyRegisters(char *str, int first_or_second_operand);

int convertStringToNum(char *str);

void initEnumArr();

void initDirectiveArray();

void printParser();
/*check complaince of line to the oper code*/


/***********************************************************************************************/
void parse(symbol_table_t *sym_tbl) {
	char *buffer = calloc(LINE_LENGTH, sizeof(char));
	char *_line = calloc(LINE_LENGTH, sizeof(char));
	char *cmd = calloc(MAX_SYMBOL_NAME, sizeof(char));
	char *cmd_extra = calloc(MAX_SYMBOL_NAME, sizeof(char));
	int *pos = calloc(1, sizeof(int));
	int isSymbol = 0; /*notes if there is a symbols and he's new or already existed*/
	line_count = 0;
	int initVal = 100;/*initial value of IC*/
	int idx = 0;
	int scanned;
	IC = &initVal;
	initParser();
	initEnumArr();
	initDirectiveArray();/*todo check if needs more*/


	if (ferror(fptr_after)) {
		printf("ERROR");
	}


	if (sym_tbl == NULL) {
		report_error(ERR_FAIL_CREATE_SYMBOL, line_count, CRIT);
		return;
	}
	/*fgets(buffer, LINE_LENGTH, fptr_after) != NULL*/
	while (fgets(buffer, LINE_LENGTH, fptr_after) != NULL) {
		line_count++;
		*IC += 10;

		buffer = strstrip(buffer);
		if ((scanned = sscanf(buffer, "%s%s:", cmd, cmd_extra)) == 2) {
			if ((isSymbol = if_Symbol_if_Duplicate(sym_tbl, cmd)) > 0) {
				*pos = strlen(cmd);/*jumps over symbol */
				buffer += *pos;
				buffer = strstrip(buffer);/*remove excess white*/
				if (isSymbol == 1)
					loadSymbolTable(sym_tbl, cmd, *IC);
				else
					checkOrUpdateSymbolAddress(sym_tbl, cmd, *IC, YES);
				if (classify_line(cmd_extra) > 0) {
					*pos = strlen(cmd_extra);
					buffer += *pos;
				} else
					report_error(ERR_LINE_UNRECOGNIZED, line_count, CRIT);
			} else if (scanned > 0) {
				if (classify_line(cmd) > 0) {
					*pos = strlen(cmd);
					buffer += *pos;

				}
			} else
				report_error(ERR_LINE_UNRECOGNIZED, line_count, CRIT);
		}

		buffer = strstrip(buffer);
		switch (parser_s.line_type) {

			case OP_CODE:
				seperator_c = string_comma_seps(buffer);
				if (seperator_c.isError == 1 /*|| seperator_c.counter > 2*/)
					report_error(ERR_OP_CODE_RECOGNITION, line_count, CRIT);

				for (idx = 0; idx < seperator_c.counter; idx++) {
					seperator_c.cString[idx] = strstrip(seperator_c.cString[idx]);
					parser_s.operands[idx].type_of_register = classifyRegisters(seperator_c.cString[idx], idx);
					if (parser_s.operands[idx].type_of_register == _DIRECT)    /*_Direct means label*/
						/*1 is new symbol 2 is duplicant onlt addresss updated*/
						if ((isSymbol == isDuplicateSymbol(sym_tbl, parser_s.operands[idx].operand.symbol)) > 0) {
							if (isSymbol == 1)
								loadSymbolTable(sym_tbl, cmd, *IC);
							else
								checkOrUpdateSymbolAddress(sym_tbl, cmd, *IC, YES);
						}
				}


				/*check complaince of line to the oper code*/
				/*	checkRegisterCompliance();*/


				break;
			case DIRECTIVE:
				break;
			case ERR:
				report_error(ERR_LINE_UNRECOGNIZED, line_count, CRIT);
				break;
			case TBD:
				break;
			default:
				break;
		}
	}
}

int classify_line(char *cmd) {

	int j = 0;
	int len = 0;

/*todo check premise of having all/some labels in label LL*/

	for (j = 0; j < 16; ++j) {
		if (strcmp(cmd, opcodeArray[j].opcode_name) == 0) {
			parser_s.line_type = OP_CODE;
			parser_s.operands[0].op = opcodeArray[j].op_num;
			parser_s.operands[1].op = opcodeArray[j].op_num;
			return 1;
		}
	}
	for (j = 0; j < 4; ++j) {
		if (strcmp(cmd, directArray[j].direct_name) == 0) {
			parser_s.line_type = DIRECTIVE;
			parser_s.directive.d_enum = directArray[j].dir_num;
			return 1;
		}
	}
	parser_s.line_type = ERR;
	return 0;
}


void classify_opcode(symbol_table_t *sym_tbl, char *buffer) {
	int i;



	/*todo here will come a function checking validity of each register */
	/*todo here will come a function classifying */

}

/*checks type of registers and correctnees - sets then in parser_s*/
/*first or second operands relates to the  parser_s*/
type_of_register_t classifyRegisters(char *str, int first_or_second_operand) {
	int i;
	char *s_ptr;
	int len = strlen(str);
	int number = 0;
	/* Check if  '#' hence immediate */
	if (strncmp(str, "#", 1) == 0) {
		s_ptr = str + 1;
		if (*s_ptr == '-')/*minus is allowed only in the beginning */
			s_ptr++;
		while (isdigit(*(s_ptr) && s_ptr++));
		if (*s_ptr)
			report_error(ERR_OP_CODE_REGISTRY_ILLEGAL, line_count, CRIT);
		else {
			number = convertStringToNum(s_ptr);
			printf("%d\n", number);
			parser_s.operands[first_or_second_operand].operand.num = number;
			return _IMMEDIATE;
		}
	}
		/* Check if '*'hence indirect */
		/* in the reg there's an adddres of the real value */
	else if (strncmp(str, "*", 1) == 0) {
		if (str[1] == 'r' && isdigit(str[2]) && str[2] >= '0' && str[2] <= '7') {
			parser_s.operands[first_or_second_operand].operand.registry = str[2];
			return _INDIRECT; /*number 3 pointer type*/
		}
	}
		/* Check if the string starts with 'r' */
	else if (strncmp(str, "r", 1) == 0) {
		if (str[0] == 'r' && isdigit(str[1]) && str[1] >= '0' && str[1] <= '7')
			parser_s.operands[first_or_second_operand].operand.registry = str[1];
		return _REGULAR; /*Number 4*/
	}
		/* Default case for other characters */

	else if (str[len - 1] == ':') {
		strncpy(parser_s.operands[first_or_second_operand].operand.symbol, str, len - 1);
		return _DIRECT; /*label*/   /*todo chk if label duplicate and add accordingly*/
	}

	return _ERROR;
}

/*return number of registers allowed*/
int checkRegisterCount(op_code_t op) {

	switch (op) {/*check if the struct differs b/w oper[1] and oper[2] opcode*/
		case mov:/*op codes that have two operands*/
		case cmp:
		case add:
		case sub:
		case lea:
			if (parser_s.operands[1].type_of_register == _ERROR ||
			    parser_s.operands[1].type_of_register == _TBD &&
			    parser_s.operands[2].type_of_register == _ERROR ||
			    parser_s.operands[2].type_of_register == _TBD) {
				report_error(ERR_OP_CODE_REGISTRY_ILLEGAL, line_count, CRIT);
				return -1;
			} else return 2;

			break;

		case clr: /*op codes that only have a destination operand */
		case not:
		case inc:
		case dec:
		case jmp:
		case bne:
		case red:
		case prn:
			if (parser_s.operands[1].type_of_register != _ERROR ||
			    parser_s.operands[1].type_of_register != _TBD &&
			    parser_s.operands[2].type_of_register == _ERROR ||
			    parser_s.operands[2].type_of_register == _TBD) {
				report_error(ERR_OP_CODE_REGISTRY_ILLEGAL, line_count, CRIT);
				return -1;
			} else return 1; 
			break;
		case jsr:/*no operands*/
		case stop:
			if (parser_s.operands[1].type_of_register != _ERROR ||
			    parser_s.operands[1].type_of_register != _TBD &&
			    parser_s.operands[2].type_of_register != _ERROR ||
			    parser_s.operands[2].type_of_register != _TBD) {
				report_error(ERR_OP_CODE_REGISTRY_ILLEGAL, line_count, CRIT);
				return -1;
			} else return 0;
			break;
		default:
			break;
	}
}

void checkRegisterCompliance() {
	char *str1, *str2;
	int idx = 0;
	op_code_t opCode = parser_s.operands[0].op;

	switch(checkRegisterCount(opCode){}
	
	
	type_of_register_t type1 = parser_s.operands[0].type_of_register;
	type_of_register_t type2 = parser_s.operands[1].type_of_register;
	

	if (type1 == opcode_specs[opCode][1][0][type1] &&
	    type2 == opcode_specs[opCode][1][0][type2])


		case mov:

		case cmp:
		case add:
		case sub:
		case lea:


		case clr: /*op codes that only have a destination operand */
		case not:
		case inc:
		case dec:
		case jmp:
		case bne:
		case red:
		case prn:
		case jsr:/*no operands*/
		case stop:

}

}


int convertStringToNum(char *str) {
	char *endtoken;
	long num = strtol(str, &endtoken, 10);
	if (*endtoken != '\0') {
		printf("invalid number '%s' (syntax error)\n", str);
		report_error(FAILED_TO_CONVERT_NUMBER, line_count, CRIT);
	}
	return (int) num;
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
	int i = 0;

	/*Populate the array*/
	for (i = 0; i < 16; ++i) {
		opcodeArray[i].opcode_name = opcode_specs[i][0][0];
		opcodeArray[i].op_num = op_code_enums[i];
	}
}

/*Function to initialize and populate the directArray*/
void initDirectiveArray() {
	int i;
	for (i = 0; i < 4; ++i) {
		directArray[i].direct_name = directives[i];
		directArray[i].dir_num = direct_enums[i];
	}
}




/** Function to print the values of parser_s variables */
/** meant for development only */
void printParser() {
	/** Declare necessary variables */
	int i;

	/** Print line type */
	printf("Line Type: %d\n", parser_s.line_type);

	/** Print symbol name */
	printf("Symbol Name: %s\n", parser_s.symbol_name);

	/** Print directive fields */
	printf("Directive Enum: %d\n", parser_s.directive.d_enum);
	printf("Directive Symbol: %s\n", parser_s.directive.operand.symbol);
	printf("Directive Data Length: %d\n", parser_s.directive.operand.data_len);
	if (parser_s.directive.operand.data != NULL) {
		printf("Directive Data: ");
		for (i = 0; i < parser_s.directive.operand.data_len; i++) {
			printf("%d ", parser_s.directive.operand.data[i]);
		}
		printf("\n");
	}
	printf("Directive String: %s\n", parser_s.directive.operand.str);

	/** Print operands fields */
	for (i = 0; i < 2; ++i) {
		printf("Operand %d Op Code: %d\n", i, parser_s.operands[i].op);
		printf("Operand %d Type of Register: %d\n", i, parser_s.operands[i].type_of_register);
		printf("Operand %d Number: %d\n", i, parser_s.operands[i].operand.num);
		printf("Operand %d Symbol: %s\n", i, parser_s.operands[i].operand.symbol);
		printf("Operand %d Registry: %d\n", i, parser_s.operands[i].operand.registry);
	}
}

void initParser() {
	int i;

	/* Initialize line type */
	parser_s.line_type = ERR;
	/* Initialize symbol name */
	/*memset(parser_s.symbol_name, 0, sizeof(parser_s.symbol_name));*/
	strcpy(parser_s.symbol_name, "*");
	/* Initialize directive fields */
	parser_s.directive.d_enum = DATA; /* Assuming DATA is a default value */
	parser_s.directive.operand.symbol = NULL;
	parser_s.directive.operand.data = NULL;
	parser_s.directive.operand.str = NULL;
	parser_s.directive.operand.data_len = 0;

	/* Initialize operands fields */
	for (i = 0; i < 2; ++i) {
		parser_s.operands[i].op = mov; /* Assuming mov is a default value */
		parser_s.operands[i].type_of_register = _TBD; /*type of register*/
		parser_s.operands[i].operand.symbol = NULL;
		parser_s.operands[i].operand.num = 0;
		parser_s.operands[i].operand.registry = 0;
	}
}

