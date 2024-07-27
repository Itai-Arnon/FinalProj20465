#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>
#include "macro.h"
#include "linkedlist.h"
#include "shared.h"
#include "global_vars.h"
#include "utils.h"



int line_count;
static const char *opcode_names[16] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp",
                                "bne", "red", "prn", "jsr", "rts", "stop"};
static const char *directives[4] ={".data",".string",".extern",".entry"};

void read_preprocessor( macro_table_t *tbl) {
	char buffer[LINE_LENGTH];
	char *macro_name = (char *) calloc(10, sizeof(char));
	int idx = 0;

	if (tbl == NULL) {
		report_error(ERR_MACRO_TABLE_GENERAL_ERROR, line_count);
		return;
	}

	while (fgets(buffer, LINE_LENGTH, fptr_before) != NULL) {
		line_count++;
		idx = 0;

		if (isRestOfLineEmpty(buffer)) /*checks case of empty line*/
			continue;

			//removes whitespace
			while (isspace(buffer[idx]) && idx< LINE_LENGTH)  ++idx;
			if (buffer[idx] == ';')
				 continue;
			//check if \n in a middle of a sentence
			if(findSeperator(buffer, "\n", 1)  ==  1){
				report_error(ERR_LINE_LENGTH,line_count);
				continue;
			}



		switch (typeofline(tbl, buffer, macro_name)) {
			case MACRO_START:
				printf( "REPORT: macro_start %d", line_count);

				if (tbl->isMacroOpen == 0 && tbl->amount < tbl->size)
					tbl->isMacroOpen = 1;
				else
					report_error(ERR_MACRO_PERMISSION, line_count);
				break;
			case MACRO_END:
				printf( "REPORT: macro_end %d\n",line_count);
					/*closes macro writing*/
					tbl->isMacroOpen = 0;
					/*macro is locked from rewriting forever*/
					macro_lock(tbl,macro_name);
					/*memset(macro_name,'\0', sizeof(macro_name));*/

				break;
			case MACRO_EXPAND:
				printf("REPORT: Macro Expand %d\n", line_count);
				expandMacro(tbl, macro_name);
				break;
			case LINE_INSIDE:
				printf("REPORT:line inside %d\n", line_count);
				loadTable(tbl, macro_name, buffer);
				break;
			case LINE_OUTSIDE:
				printf("REPORT:line outside %d\n", line_count);
				fprintf(fptr_after, "%s", buffer);
			default:
				break;
		}

	}

}


int typeofline(macro_table_t *tbl, char *line, char *macro_name) {
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
		} else if (checkMacroStart(buffer, start, macro_name, pos))
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


int checkMacroStart(char *line, char *start, char *macro_name, int pos) {

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


int nonNullTerminatedLength(char *arr) {
	int count = 0;
	while (arr[++count] != 0);
	return count;
}

/*type=0 for input file type =1 for output*/
FILE *initSourceFiles(int _argc, char **_argv, FILE *fptr, int index) {
	/*TODO: add option to goto next file once a file fails to open*/
	char line[LINE_LENGTH];
	char filename_before[64];
	int argv_len = 0;
	if (_argc > 1) {/*argc  must always be subtracted by one*/
		argv_len = nonNullTerminatedLength(_argv[index]);

		strcpy(filename_before, PATH_BASE);/*TODO no need in linux*/
		strncat(filename_before, _argv[index], argv_len); /*TODO arg_c iteration tbd*/
		printf("%s\n", filename_before);
		if (!(fptr = fopen(filename_before, "r"))) {
			report_error(ERR_FILE_BEFORE, line_count);
			exit(0);
		}

		return fptr;

	} else report_error(ERR_NO_FILES, line_count);
}

FILE *initDestinationPointer(FILE *fptr, char *filename) {
	// Construct the file path by prepending the parent directory
	char filename_path[64];
	strcpy(filename_path, PATH_BASE);/*TODO no need in linux*/
	strcat(filename_path, filename);

	/* Attempt to open the file for writing*/
	if (!(fptr = fopen(filename_path, "a"))) {
		report_error(ERR_FILE_AFTER, line_count);
		exit(0);
	}
	/* Return the file pointer if the file was successfully created*/
	return fptr;
}


void report_error(char *err, int line_count) {

	printf("%s at line %lu\n", err, line_count);

}
/*this function check if the non alpha numeric chars in a given string*/
/*ALPHA - only alphabets allowed  ALPHANUM alphabets and a number at the end*/
int checkLegalName(char *str, int type) {
	int i = 0;
	int len = nonNullTerminatedLength(str);
	if(!isalpha(str[0]) ) return 0;
	str++;
	switch (type) {
		case ALPHA:
			while (i < len && isalpha(str[i])) i++;

			return i == (len - 1) ? 1 : 0;
		case ALPHANUM:

			while (i < len && isalpha(str[i])) i++;
			/*check condition where after the alphabet there are digits which is acceptable*/
			while (i < len && isdigit(str[i])) i++;

			return i ==  (len - 1) ? 1 : 0;

			case ALPHANUM_COMBINED:
				/*check condition where after the alphabet there are digits which is acceptable*/
				while (i < len && isalnum(str[i])) i++;


			return i ==  (len - 1) ? 1 : 0;

		default:
			break;
	}
}/*TODO:go to utils*/



int checkEOFInBuffer(char *buffer) {
	char *localPtr = buffer;

	while (*localPtr != '\0') {
		if (*localPtr == EOF)
			return 1; /* EOF marker found*/
		localPtr++; /*Move to the next character*/
	}
	return 0; /*Eof Not Found*/
}


int macro_name_has_opcode_direct_symbol(char* macro_name){
	int j = 0 ;
	for ( j = 0; j < 16; ++j) {
		if(strcmp(macro_name,opcode_names[j])!=0)
			return 1;
	}
	for ( j = 0; j < 4; ++j) {
		if(strcmp(macro_name,directives[j])!=0)
			return 1;
	}
	return 0;
	/* TODO; later on symbols will come here*/

}


