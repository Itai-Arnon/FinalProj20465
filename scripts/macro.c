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

#define   FILE_PATH()   printf(__FILE__ );


int line_count;
static const char *opcode_names[16] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp",
                                       "bne", "red", "prn", "jsr", "rts", "stop"};
static const char *directives[4] = {".data", ".string", ".extern", ".entry"};

void read_preprocessor(macro_table_t *tbl, symbol_table_t *sym_tbl) {
	char *buffer = malloc(sizeof(char) * LINE_LENGTH);
	char *macro_name = (char *) calloc(10, sizeof(char));
	FILE *fptr_3 = fopen("c:\\opu\\20465\\m\\m14\\a.txt", "r");
	memset(buffer, '\0', sizeof(buffer));

	int idx = 0;
	line_count = 0;

	if (tbl == NULL || sym_tbl == NULL) {
		report_error(ERR_MACRO_TABLE_GENERAL_ERROR, line_count);/*TODO: critical*/
		return;
	}
/*
	fscanf(fptr_before,"%s\n", buffer);
	printf("%s\n",buffer);
	fprintf(fptr_after,"%s\n","choompee");
	printf("%s\n","choompee");*/

	if (fgets(buffer, LINE_LENGTH, fptr_before) == NULL)
		if (ferror(fptr_before)) {
			printf("Error fgets");
		}
	fscanf(fptr_3, "%s\n", buffer);
	printf("%s\n", buffer);


	printf("value of fgets %d\n", fgets(buffer, LINE_LENGTH, fptr_before));
	while (fgets(buffer, LINE_LENGTH, fptr_before) != NULL) {
		line_count++;
		idx = 0;

		if (isRestOfLineEmpty(buffer)) /*checks case of empty line*/
			continue;

		//removes whitespace
		while (isspace(buffer[idx]) && idx < LINE_LENGTH) ++idx;
		if (buffer[idx] == ';')
			continue;
		//check if \n in a middle of a sentence
		if (findSeperator(buffer, "\n", 1) == 1) {
			report_error(ERR_LINE_LENGTH, line_count);
			continue;
		}

		switch (typeofline(tbl, buffer, macro_name, sym_tbl)) {
			case MACRO_START:
				printf("REPORT: macro_start %d", line_count);

				if (tbl->isMacroOpen == 0 && tbl->amount < tbl->size)
					tbl->isMacroOpen = 1;
				else
					report_error(ERR_MACRO_PERMISSION, line_count);
				break;
			case MACRO_END:
				printf("REPORT: macro_end %d\n", line_count);
				/*closes macro writing*/
				tbl->isMacroOpen = 0;
				/*macro is locked from rewriting forever*/
				macro_lock(tbl, macro_name);
				/*memset(macro_name,'\0', sizeof(macro_name));*/

				break;
			case MACRO_EXPAND:
				printf("REPORT: Macro Expand %d\n", line_count);
				expandMacro(tbl, macro_name);
				break;
			case LINE_INSIDE:
				printf("REPORT:line inside %d\n", line_count);
				loadMacroTable(tbl, macro_name, buffer);
				break;
			case LINE_OUTSIDE:
				printf("REPORT:line outside %d\n", line_count);
				fprintf(fptr_after, "%s", buffer);
			default:
				break;
		}

	}
}


int typeofline(macro_table_t *tbl, char *line, char *macro_name, symbol_table_t *sym_tbl) {
	char *buffer = calloc(LINE_LENGTH, sizeof(char));
	char *start = calloc(MACRO_END, sizeof(char));
	int pos = 0;
	int i = 0;

	if (*line == '\0') return EMPTY_LINE;
	strcpy(buffer, line);
	/*removes white space from the front*/

	if (sscanf(buffer, "%s%n", start, &pos) == 1) {

		if (!checkLegalName(start, ALPHANUM_COMBINED)) {
			report_error(ERR_MACRO_DEFINE, line_count);
			return (MACRO_ERROR);
		} else if (checkMacroStart(buffer, start, macro_name, pos, sym_tbl))
			return MACRO_START;
		else if (checkMacroEnd(buffer, start, pos))
			return MACRO_END;
		else if (checkMacroExpand(tbl, line, start, macro_name, pos))
			return MACRO_EXPAND;
		else if (tbl->isMacroOpen == 1)
			return LINE_INSIDE;
		else
			return LINE_OUTSIDE;
	}
}

/*else if (tbl->isMacroOpen == 0)
	return LINE_OUTSIDE;
else {
	report_error(ERR_MACRO_DEFINE, line_count);
	return MACRO_ERROR;
}		return MACRO_EXPAND;
}
return MACRO_ERROR;
}
}*/


int checkMacroStart(char *line, char *start, char *macro_name, int pos, symbol_table_t *sym_tbl) {

	char *str = line;
	char macro_n[MAX_MACRO_NAME];
	memset(macro_n, '\0', sizeof(macro_n));

	if (strncmp(MACRO_START_WORD, start, MACRO_START_LEN) == 0) {
		str = str + pos + 1;
		pos = 0;
		if (sscanf(str, "%s%n", macro_n, &pos) == 1) {/*check for actual macro name*/
			/*change the position*/
			str = str + pos + 1;

			if (strlen(macro_n) >= MAX_MACRO_NAME) {
				report_error(ERR_MACRO_NAME_LONG, line_count);/*critical error*/
				return 0;
			}
			/*macro named identified check if contiuation of line is empty*/
			if (!(isRestOfLineEmpty(str))) {
				report_error(ERR_MACRO_DEFINE, line_count);  /* critical wait for macro check*/
				return 0;
			}
			printf("REPORT: macro start %d\n", line_count);
			/*final filtering for isMacroStart*/
			strcpy(macro_name, macro_n);/*only place in use by typeofline*/
			if (macro_name_duplicate(macro_name, sym_tbl) == 1) {
				report_error(ERR_MACRO_NAME_OP_DIRECT_SYMBOL, line_count);
				return 0;
			}
			return 1;/*line with name is correct*/
		}
	}
	return 0;
}

int checkMacroEnd(char *line, char *start, int pos) {
	char *str = line;
	if (strncmp(MACRO_END_WORD, start, MACRO_END_LEN) == 0) {
		str = str + pos;

		if (!(isRestOfLineEmpty(str))) {
			report_error(ERR_MACRO_END, line_count);/*critical*/
			return 0;
		} else {
			return 1;
		}
	}
	/*didn't identify macro_end_word, but we still check for EOF*/
	/*hence we should close the macro*/
	return checkEOFInBuffer(line);
}


int checkMacroExpand(macro_table_t *tbl, char *line, char *start, char *macro_name, int pos) {
	char *str = line;

	if (tbl->amount > 0 && (checkNameExistsInTable(tbl, start) == 1)) {
		strcpy(macro_name, start);
		str += pos;
		if (isRestOfLineEmpty(str))
			return 1;
		else {
			/*macro name is the single word allowed on macro expand*/
			report_error(ERR_MACRO_EXPAND, line_count);
			return 0;
		}
	}
	return 0;
}


void report_error(char *err, int line_count) {

	printf("%s at line %lu\n", err, line_count);

}
/*this function check if the non alpha numeric chars in a given string*/
/*ALPHA - only alphabets allowed  ALPHANUM alphabets and a number at the end*/



int checkEOFInBuffer(char *buffer) {
	char *localPtr = buffer;

	while (*localPtr != '\0') {
		if (*localPtr == EOF)
			return 1; /* EOF marker found*/
		localPtr++; /*Move to the next character*/
	}
	return 0; /*Eof Not Found*/
}


int macro_name_duplicate(char *macro_name, symbol_table_t *sym_tbl) {
	symbol_t *head = sym_tbl->symbol_List;
	char macro_name_c[MAX_SYMBOL_NAME];/*support macro name with colon*/
	int j = 0;
	int len = 0;

	for (j = 0; j < 16; ++j) {
		if (strcmp(macro_name, opcode_names[j]) == 0)
			return 1;
	}
	for (j = 0; j < 4; ++j) {
		if (strcmp(macro_name, directives[j]) == 0)
			return 1;
	}

	/*in case a colon appears in macro_name*/

	while (head != NULL) {
		len = strlen(macro_name);
		strncpy(macro_name_c, macro_name, len - 1);
		if (strcmp(macro_name, head->symbol_name) == 0 || strcmp(macro_name_c, head->symbol_name) == 0) {
			return 1;
		}
		head = head->next_sym;
		memset(macro_name_c, '\0', sizeof(macro_name_c));
	}
	return 0;
}


