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


static directive_cmd_t direct_enums[4] = {DATA, STRING, ENTRY, EXTERN};
static direct_arr_t directArray[4];



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


parser_t parser;
static sep_commas_t seperator_c;
static sep_whitespace_t seperator_w;

void parse(symbol_table_t *sym_tbl) {
	char *buffer = calloc(LINE_LENGTH, sizeof(char));/*sentence analyzed*/
	char *cmd = calloc(MAX_SYMBOL_NAME, sizeof(char));/*cmd parse*/
	char *cmd_extra = calloc(MAX_SYMBOL_NAME, sizeof(char)); /* 2nd cmd parse*/
	char *directive_str = NULL;/*auxialry var*/
	char *sArr = calloc(MAX_SYMBOL_NAME, sizeof(char));/*.string array*/
	int *pos = calloc(1, sizeof(int));/*promotes the buffer*/
	int idx, numCount, buff_len, scanned, result, isExtern , isSymbol; /*auxiliary vars*/
	idx = numCount = buff_len, scanned, result, isExtern, isSymbol = 0;
	line_count = 0;


	initEnumArr();
	initDirectiveArray();/*todo check if needs more*/
/*number of sscanf arguments*/

	if (ferror(fptr_after)) {
		printf("ERROR");
	}


	if (sym_tbl == NULL) {
		report_error(ERR_FAIL_CREATE_SYMBOL, line_count, CRIT);
		return;
	}
	/*fgets(buffer, LINE_LENGTH, fptr_after) != NULL*/
	while (fgets(buffer, LINE_LENGTH, fptr_after) != NULL) {
		initParser();
		line_count++;
		buffer = strstrip(buffer);

		/*option label is at start - will identify both label and opcode*/
		/*scanned how many succesfuly scanned*/
		if (sscanf(buffer, "%s", cmd) == 1) {
			if (if_Symbol_if_Duplicate(sym_tbl, cmd, HEAD) == 1) {
				/*returns 1 if symbol non existent*/
				loadSymbolTable(sym_tbl, cmd, 0, _INSTRUCTION);
				buffer = advance_buffer_if_possible(buffer, cmd);
				buffer = strstrip(buffer);/*remove excess white*/
				sscanf(buffer, "%s", cmd_extra);
				(scanned = classify_line(cmd_extra));
				if (scanned == -1)
					report_error(ERR_EXTERN_ENTRY_ILLEGAL, line_count, CRIT);
				if (scanned > 0)
					/*advanced the buffer only if doens't emtpy it*/
					buffer = advance_buffer_if_possible(buffer, cmd_extra);
				else
					report_error(ERR_DIRECTIVE_RECOGNITION, line_count, CRIT);
			} else if (classify_line(cmd) > 0)
				buffer = advance_buffer_if_possible(buffer, cmd);
		} else report_error(ERR_LINE_UNRECOGNIZED, line_count, CRIT);


		result = scanned = buff_len = 0;
		buffer = strstrip(buffer);
		memset(cmd, '\0', sizeof(cmd));
		memset(cmd, '\0', sizeof(cmd_extra));
		switch (parser.line_type) {
			case OP_CODE:
				/*seperates the registers across an array of strings*/
				seperator_c = string_comma_seps(buffer);

				/*isError - too many comma | register_count_by_op - no of registers by opcode definition  */
				if (seperator_c.isError == 1 || seperator_c.counter != register_count_by_op(parser.op))
					report_error(ERR_OP_CODE_RECOGNITION, line_count, CRIT);
				/*idx  1 if only destination reg exits , 0 if two regs exist*/
				idx = (seperator_c.counter == 2) ? 0 : 1;
				for (; idx < seperator_c.counter; idx++) {/*classifies registers*/
					seperator_c.cString[idx] = strstrip(seperator_c.cString[idx]);
					parser.operands[idx].type_of_register = classifyRegisters(seperator_c.cString[idx], idx);
				}
				if (checkRegisterCount(parser.op) == -1 || checkRegisterCompliance() == 0)
					report_error(ERR_OP_CODE_REGISTRY_ILLEGAL, line_count, CRIT);

				/*Creat symbol a register with direct : labels*/
				for (idx = 0; idx < seperator_c.counter; idx++) {
					if (parser.operands[idx].type_of_register == _DIRECT) {    /*_Direct means label*/
						/* if_Symbol_if_Duplicant:chks if symbol| 1 is new symbol|2 is duplicant|last arg decides if symbol is start/middle */
						if (if_Symbol_if_Duplicate(sym_tbl, parser.operands[idx].operand.symbol, MIDDLE) == 1) {
							loadSymbolTable(sym_tbl, parser.operands[idx].operand.symbol, 0, _INSTRUCTION);
						}
					}
				}
				break;
			case DIRECTIVE:
				if ((result = parser.directive.cmd) == DATA) {
					/*separates the numbers across an array of strings*/
					numCount = countNumbersInString(buffer);
					seperator_c = string_comma_seps(buffer);
					/*check if buffer number count concurs with separator struct|isErrpr has
					 * error ocuured*/
					if (seperator_c.isError || seperator_c.counter != numCount)
						report_error(ERR_DATA_DIRECTIVE_NUMBER, line_count, CRIT);

					parser.directive.operand.data_len = numCount;
					parser.directive.operand.data = (int *) calloc(seperator_c.counter, sizeof(int));

					for (idx = 0; idx < seperator_c.counter; idx++) {
						directive_str = strstrip(seperator_c.cString[idx]);
						*pos = convertOrCheckStringToNum(directive_str, 0);
						if (*pos < MIN_15Bit_NUM || *pos > MAX_15Bit_NUM)
							report_error(ERR_DATA_DIRECTIVE_NUMBER, line_count, CRIT);
						/*assigning  the number to the parser*/
						parser.directive.operand.data[idx] = *pos;
					}
					break;
				} else if (result == STRING) {
					buff_len = strlen(buffer) - 2;
					if (processString(buffer, sArr) == 0)
						report_error(ERR_STRING_QUOTATION_MISSING, line_count, CRIT);
					else {
						parser.directive.operand.str = (char *) calloc(buff_len, sizeof(char));
						parser.directive.operand.data_len = buff_len;
					}
					for (idx = 0; idx < buff_len; idx++) {
						parser.directive.operand.str[idx] = sArr[idx];
						printf("%c\n", parser.directive.operand.str[idx]);
					}
					break;
				} else if (result == ENTRY || result == EXTERN) {
					if (sscanf(buffer, "%s%n", cmd , pos) == 1) {
						buffer +=* pos;
						if(isRestOfLineEmpty(buffer) == 0)
							report_error(ERR_OP_CODE_FAILED_STRUCTURE,line_count,CRIT);
						if ((isSymbol = if_Symbol_if_Duplicate(sym_tbl, cmd, MIDDLE)) == 1) {
							loadSymbolTable(sym_tbl, buffer, 0, _INSTRUCTION);
							parser.directive.operand.symbol = calloc(MAX_SYMBOL_NAME, sizeof(char));
							strcpy(parser.directive.operand.symbol, cmd);
							parser.directive.operand.data_len = 1;
							printf("blah");
						}
					} else
						report_error(ERR_DIRECTIVE_RECOGNITION, line_count, CRIT);
				}
				break;
			case ERR:
				report_error(ERR_LINE_UNRECOGNIZED, line_count, CRIT);
				break;
			case TBD:
				report_error(ERR_DIRECTIVE_RECOGNITION, line_count, CRIT);
				break;
			default:
				break;
		}
	}
} /*END OF PARSE*/



int classify_line(char *cmd) {
	int j = 0;
	int len = 0;

	if (*cmd == '\0') return -1;


	for (j = 0; j < 16; ++j) {
		if (strcmp(cmd, opcodeArray[j].opcode_name) == 0) {
			parser.line_type = OP_CODE;
			/*although only one opcode defines for two operands due to struct structure*/
			parser.op = opcodeArray[j].op_enum;
			return 1;
		}
	}
	for (j = 0; j < 4; ++j) {
		if (strcmp(cmd, directArray[j].direct_name) == 0) {
			parser.line_type = DIRECTIVE;
			parser.directive.cmd = directArray[j].cmd;
			/*extern and entry can't have a label before them*/
			if (parser.directive.cmd == EXTERN || parser.directive.cmd == ENTRY) {
				if (strcmp(parser.symbol_name, "TBD") != 0)
					return -1;
			}
			return 1;
		}
	}
	parser.line_type = ERR;
	return 0;
}


/*checks type of registers and correctnees - sets then in parser*/
/*first or second operands relates to the  parser*/
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
			number = convertOrCheckStringToNum(s_ptr, 0);
			printf("%d\n", number);
			parser.operands[first_or_second_operand].operand.num = number;
			return _IMMEDIATE;
		}
	}
		/* Check if '*'hence indirect */
		/* in the reg there's an adddres of the real value */
	else if (strncmp(str, "*", 1) == 0) {
		if (str[1] == 'r' && isdigit(str[2]) && str[2] >= '0' && str[2] <= '7') {
			parser.operands[first_or_second_operand].operand.registry = str[2];
			return _INDIRECT; /*number 3 pointer type*/
		}
	}
		/* Check if the string starts with 'r' */
	else if (strncmp(str, "r", 1) == 0) {
		if (str[0] == 'r' && isdigit(str[1]) && str[1] >= '0' && str[1] <= '7')
			parser.operands[first_or_second_operand].operand.registry = str[1];
		return _REGULAR; /*Number 4*/
	}
		/* Default case for other characters */

	else if (checkLegalName(str, ALPHANUM_COMBINED)) {
		str = removeColon(str);
		strcpy(parser.operands[first_or_second_operand].operand.symbol, str);
		return _DIRECT; /*label*/   /*todo chk if label duplicate and add accordingly*/
	}

	return _ERROR;
}

/*return number of registers allowed, -1 means op_code number of registers is wrong*/
int register_count_by_op(op_code_t op) {
	if (op >= mov && op <= lea)
		return 2;
	else if (op >= clr && op <= stop)
		return 1;
	else
		return 0;
}

int checkRegisterCount(op_code_t op) {

	if (op >= mov && op <= lea) {
		if ((parser.operands[1].type_of_register == _ERROR ||
		     parser.operands[1].type_of_register == _TBD) ||
		    (parser.operands[2].type_of_register == _ERROR ||
		     parser.operands[2].type_of_register == _TBD)) {
			report_error(ERR_OP_CODE_REGISTRY_ILLEGAL, line_count, CRIT);
			return -1;
		} else return 2;
	} else if (op >= clr && op <= prn) {
		if ((parser.operands[1].type_of_register != _ERROR &&
		     parser.operands[1].type_of_register != _TBD) &&
		    parser.operands[2].type_of_register == _ERROR ||
		    parser.operands[2].type_of_register == _TBD) {
			report_error(ERR_OP_CODE_REGISTRY_ILLEGAL, line_count, CRIT);
			return -1;
		} else return 1;

	} else {
		if (parser.operands[1].type_of_register != _ERROR &&
		    parser.operands[1].type_of_register != _TBD &&
		    parser.operands[2].type_of_register != _ERROR &&
		    parser.operands[2].type_of_register != _TBD) {
			report_error(ERR_OP_CODE_REGISTRY_ILLEGAL, line_count, CRIT);
			return -1;
		} else return 0;
	}
}

/*checks type of register assigned to register is correct */
int checkRegisterCompliance() {
	type_of_register_t type1, type2;
	op_code_t opCode = parser.op;
	type1 = type2 = 0;

	switch (checkRegisterCount(opCode)) {
		case -1:/*if error in checkRegisterCount */
			return -1;
		case 0:/*case of no operands always correct*/
			return 1;
		case 1:
			/*check destination operands only */
			type2 = parser.operands[0].type_of_register;
			if (type2 == opcode_specs[opCode][2][0][type1] - '0')
				return 1;
			return 0;
			break;
		case 2:
			/*check 2 operands*/
			type1 = parser.operands[0].type_of_register;
			type2 = parser.operands[1].type_of_register;
			/*comparison based opcpde_specs which defines all possible modes for each reg*/
			if ((type1 == opcode_specs[opCode][1][0][type1] - '0') &&
			    (type2 == opcode_specs[opCode][2][0][type2] - '0'))
				return 1;
			return 0;
			break;
		default:
			break;
	}
}


char *advance_buffer_if_possible(char *buffer, char *cmd) {
	int cmd_len = strlen(cmd);
	if (strlen(buffer) > cmd_len) {
		buffer += cmd_len;
		return buffer;
	}
	return buffer;
}


/* process .string  string  0 : non printable asciis  1: success   */
int processString(char *str, char *sArr) {
	int len = strlen(str);
	int idx, i;
	i = idx = 0;

	if (len < 2) return 0;

	if (str[0] == '\"' && str[len - 1] == '\"') {
		str++;
		len -= 2;
	} else return 0;

	/* check each character using isprint that check for printable chars */
	for (i = 0; i < len; i++) {
		 if(isprint(str[i])){
		          sArr[idx++] = str[i];
			} else
			return 0;
	}
	sArr[idx] = '\0';
	return 1;
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
	op_code_t arr[16]= 	{mov, cmp, add, sub, lea, clr, not, inc, dec, jmp,
			bne, red, prn, jsr, rts, stop};
	/*Populate the array*/
	for (i = 0; i < 16; ++i) {
		opcodeArray[i].opcode_name = opcode_names[i];
		opcodeArray[i].op_enum =arr[i];
	}
}

/*Function to initialize and populate the directArray*/
void initDirectiveArray() {
	int i;
	for (i = 0; i < 4; ++i) {
		directArray[i].direct_name = directives[i];
		directArray[i].cmd = direct_enums[i];
	}
}




/** Function to print the values of parser variables */
/** meant for development only */
void printParser() {
	/** Declare necessary variables */
	int i;

	/** Print line type */
	printf("Line Type: %d\n", parser.line_type);

	/** Print symbol name */
	printf("Symbol Name: %s\n", parser.symbol_name);

	/** Print directive fields */
	printf("Directive Enum: %d\n", parser.directive.cmd);
	printf("Directive Symbol: %s\n", parser.directive.operand.symbol);
	printf("Directive Data Length: %d\n", parser.directive.operand.data_len);
	if (parser.directive.operand.data != NULL) {
		printf("Directive Data: ");
		for (i = 0; i < parser.directive.operand.data_len; i++) {
			printf("%d ", parser.directive.operand.data[i]);
		}
		printf("\n");
	}
	printf("Directive String: %s\n", parser.directive.operand.str);

	printf("Operand %d Op Code: %d\n", i, parser.op);
	/** Print operands fields */
	for (i = 0; i < 2; ++i) {
		printf("Operand %d Type of Register: %d\n", i, parser.operands[i].type_of_register);
		printf("Operand %d Number: %d\n", i, parser.operands[i].operand.num);
		printf("Operand %d Symbol: %s\n", i, parser.operands[i].operand.symbol);
		printf("Operand %d Registry: %d\n", i, parser.operands[i].operand.registry);
	}
}

void initParser() {
	int i;

	/* Initialize line type */
	parser.line_type = ERR;
	/* Initialize symbol name */
	/*memset(parser.symbol_name, 0, sizeof(parser.symbol_name));*/
	strcpy(parser.symbol_name, "TBD");
	/* Initialize directive fields */
	parser.directive.cmd = DATA; /* Assuming DATA is a default value */
	parser.directive.operand.symbol = NULL; /*array dynamic alloc*/
	parser.directive.operand.data = NULL;
	parser.directive.operand.str = NULL;
	parser.directive.operand.data_len = 0;

	/* Initialize operands fields */
	parser.op = mov; /* Assuming mov is a default value */
	for (i = 0; i < 2; ++i) {

		parser.operands[i].type_of_register = _TBD; /*type of register*/
		parser.operands[i].operand.symbol;
		parser.operands[i].operand.num = 0;
		parser.operands[i].operand.registry = 0;
	}
}

