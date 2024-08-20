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




int line_count;

void read_preprocessor(macro_table_t *tbl, symbol_table_t *sym_tbl) {
	char *buffer = malloc(sizeof(char) * LINE_LENGTH);
	char *macro_name = (char *) calloc(10, sizeof(char));
	int idx = 0;
	int *pos = calloc(1, sizeof(int));
	memset(buffer, '\0', sizeof(buffer));

	line_count = 0;

	if (tbl == NULL || sym_tbl == NULL) {
		report_error(ERR_MACRO_TABLE_GENERAL_ERROR, line_count ,FIRST , CRIT ,0);
		return;
	}


	while (fgets(buffer, LINE_LENGTH, fptr_before) != NULL) {
		line_count++;
		idx = 0;
		if (isError)
			return;

		/*check for empty line*/
		if (isRestOfLineEmpty(buffer)) /*checks case of empty line*/
			continue;

		//removes whitespace
		removeFrontalWhitespace(buffer, pos);
		buffer += *pos; /*advances the pointer to end of whitespace*/
		if (*buffer == ';' || *buffer == '\0') { /*check for comment ;*/
			continue;
		}
		/*check if sentence is too long */
		/*find \n in mid sentence means lenght illegal*/
		if (findSeperator(buffer, "\n", 1) == 1) {
			report_error(ERR_LINE_LENGTH, line_count,MAC, CRIT ,0);
			continue;
		}

		switch (typeofline(tbl, buffer, macro_name, sym_tbl)) {
			case MACRO_START:
				printf("REPORT: macro_start  at line %d\n", line_count);
				if (dupNameExistsInTable(tbl, macro_name,1) == 1)
					report_error(ERR_MACRO_NAME_DUP, line_count ,MAC , CRIT ,0);    /*critical error*/
				if (tbl->isMacroOpen == 0 && tbl->amount < tbl->size)
					tbl->isMacroOpen = 1;
				else
					report_error(ERR_MACRO_PERMISSION, line_count,MAC ,CRIT ,0);
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

/*line- fgets scan , macro_name-in charge of tranfering macro name, sym_tbl - symbols are scanned for */
int typeofline(macro_table_t *tbl, char *line, char *macro_name, symbol_table_t *sym_tbl) {
	char *buffer = calloc(LINE_LENGTH, sizeof(char));
	char *start = calloc(MACRO_END, sizeof(char));
	int pos = 0;


	if (*line == '\0') return EMPTY_LINE;
	strcpy(buffer, line);
	/*removes white space from the front*/

	if (sscanf(buffer, "%s%n", start, &pos) == 1) {
		if (!checkLegalName(start, ALPHANUM_COMBINED)) {
			report_error(ERR_MACRO_NAME_WRONG, line_count ,MAC , CRIT ,0);
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


int checkMacroStart(char *buffer, char *start, char *macro_name, int pos, symbol_table_t *sym_tbl) {

	char *str = buffer;
	char macro_n[MAX_MACRO_NAME];
	memset(macro_n, '\0', sizeof(macro_n));


	if (strncmp(MACRO_START_WORD, start, MACRO_START_LEN) == 0) {
		str = str + pos + 1;

		pos = 0;
		if (sscanf(str, "%s%n", macro_n, &pos) == 1) {/*check for actual macro name*/
			/*change the position*/
			str = str + pos + 1;

			if (strlen(macro_n) >= MAX_MACRO_NAME) {
				report_error(ERR_MACRO_NAME_LONG, line_count ,MAC , CRIT ,0);/*critical error*/
				return 0;
			}
			/*macro named identified check if contiuation of buffer is empty*/
			if (!(isRestOfLineEmpty(str))) {
				report_error(ERR_START_MACRO_DEF, line_count ,MAC , CRIT ,0);  /* critical wait for macro check*/
				return 0;
			}
			/*final filtering for isMacroStart, test for duplicates*/
			strcpy(macro_name, macro_n);/*only place in use by typeofline*/
			if (macro_name_duplicate(macro_name, sym_tbl) == 1) {
				report_error(ERR_MACRO_NAME_OP_DIRECT_SYMBOL, line_count ,MAC , CRIT ,0);
				return 0;
			}
			return 1;/*line with name is correct*/
		}
	}
	return 0;
}

int checkMacroEnd(char *buffer, char *start, int pos) {
	char *str = buffer;
	if (strncmp(MACRO_END_WORD, start, MACRO_END_LEN) == 0) {
		str = str + pos;

		if (!(isRestOfLineEmpty(str))) {
			report_error(ERR_MACRO_END, line_count ,MAC , CRIT ,0);
			return 0;
		} else {
			return 1;
		}
	}
	/*didn't identify macro_end_word, but we still check for EOF*/
	/*hence we should close the macro*/
	return checkEOFInBuffer(buffer);
}


int checkMacroExpand(macro_table_t *tbl, char *buffer, char *start, char *macro_name, int pos) {

	if (tbl->amount > 0 && (dupNameExistsInTable(tbl, start, 0) == 1)) {
		strcpy(macro_name, start);
		buffer += pos;
		if (isRestOfLineEmpty(buffer))
			return 1;

		else {
			/*macro name is the single word allowed on macro expand*/
			report_error(ERR_MACRO_EXPAND, line_count ,MAC , CRIT ,0);
			return 0;
		}

	}
	return 0;
}


/*todo add more error checking use ifeof*/
int checkEOFInBuffer(char *buffer) {
	char *localPtr = buffer;

	while (*localPtr != '\0') {
		if (*localPtr == EOF)
			return 1; /* EOF marker found*/
		localPtr++; /*Move to the next character*/
	}
	return 0; /*Eof Not Found*/
}

/*checks only opcode and directive*/
/*symbols will be checked in 2nd pass*/
int macro_name_duplicate(char *macro_name, symbol_table_t *sym_tbl) {
	int j = 0;



	for (j = 0; j < 16; ++j) {
		if (strcmp(macro_name, opcode_names[j]) == 0)
			return 1;
	}
	for (j = 0; j < 4; ++j) {
		if (strcmp(macro_name, directives[j]) == 0)
			return 1;
	}



	return 0;
}


