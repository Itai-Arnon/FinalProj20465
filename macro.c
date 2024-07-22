#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>
#include "macro.h"
#include "linkedlist.h"
#include "shared.h"
#include "global_vars.h"


FILE *fptr_before;
FILE *fptr_after;
int line_count;

int main(int argc, char *argv[]) {


	readline(argc, argv);



//	loadTable(tbl, "mimi", "mcr\0");
//	loadTable(tbl, "mimi", "mona\0");
//	loadTable(tbl, "mimi", "miko\0");
//

	/*printf("tbl slot 2 name %s \n", tbl->slot[0]->macro_next->macro_name);
	printf("tbl slot 2 name %s \n", tbl->slot[0]->macro_next->macro_next->macro_name);*/


	return 0;
}


void readline(int _argc, char **_argv) {
	char buffer[LINE_LENGTH];
	char *macro_name = (char *) calloc(10, sizeof(char));
	macro_table_t *tbl = NULL;
	int line_length = 0;
	int idx = 0;
	int current_slot = -1;

	tbl = initTable(tbl);
	fptr_before = initSourceFiles(_argc, _argv, fptr_before, 1);
	fptr_after = initDestinationPointer(fptr_after, "out.txt");

	while (fgets(buffer, LINE_LENGTH, fptr_before) != NULL) {
		line_count++;
		line_length = 0;
		idx = 0;

		while (isspace(buffer[idx]) && line_length++ && buffer[idx++] != '\0');
		if (buffer[idx] == '\n' && line_length > LINE_LENGTH) {
			report_error(ERR_LINE_LENGTH, line_count);
			continue;
		}
		if (buffer[idx] == ';')
			continue;


		/*printf("typeofline: %d\n", (typeofline(tbl, buffer, macro_name)));*/
		switch (typeofline(tbl, buffer, macro_name)) {
			case MACRO_START:
				if (tbl->isMacroOpen == 0 && tbl->amount < MAX_MACROS)
					tbl->isMacroOpen = 1;
				else
					report_error(ERR_MACRO_PERMISSION, line_count);

				if ((current_slot = retSlot(tbl, macro_name) != -1))
					report_error(EER_MACRO_TABLE_RETREIVE, line_count);/*critical*/
					exit(0);

				break;
			case MACRO_END:
				if ( current_slot!=-1) {
					tbl->isMacroOpen = 0;
					tbl->slot[current_slot]->macro_lock = 1;
					printf("REPORT: macro locked");
				}
				break;
			case MACRO_EXPAND:
			case LINE_INSIDE:
				printf("line inside\n");
				loadTable(tbl, macro_name, buffer);
				/*printf("Table is Full  %d | isMacroOpen %d|tbl amount %d \n", tbl->isFull, tbl->isMacroOpen, tbl->amount);*/
				printMacroName(tbl->slot[current_slot]->macro_name);
				printMacroName(tbl->slot[current_slot]->macro_name);
				expandMacro(tbl, macro_name);


			default:
				break;
		}

	}

	fclose(fptr_before);
	fclose(fptr_after);
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

		if (!checkLegalName(start, ALPHANUM)) {
			report_error(ERR_MACRO_DEFINE, line_count);
			return (MACRO_ERROR);
		} else if (checkMacroStart(buffer, start, macro_name, pos))
			return MACRO_START;
		else if (checkMacroEnd(buffer, start, pos))
			return MACRO_END;
		else
			return LINE_INSIDE;
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
			if (!(isLineEmpty(str))) {
				report_error(ERR_MACRO_DEFINE, line_count);  /*wait for macro check*/
				return 0;
			}
			printf("REPORT: macro start \n");
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

		if (!(isLineEmpty(str))) {
			report_error(ERR_MACRO_END, line_count);/*critical*/
			return 0;
		} else {
			printf("report: macro end");
			return 1;
		}
	}

	return 0;
}


int checkLineInside(char *line, char *start, int pos) {

	return 1;


	/*empty line case*/
	/*if (isLineEmpty(line) && *start == '\0')
		return 2;*//*doesn't belong to the macro*//*

	if (macptr != NULL) {
		*//*start new node and copy content of line*//*
		*//*invokes node creation and load to hash table*//*
		*//*expects success*//*
		*//*error ERR_WRITING_MACRO*//*
		printf("LineInside = placedholder\n");
		return 1;
	}
	if (macptr == NULL) {
		*//*checks if macptr==NULL
		 * and if so writes the line straight to the file
		 * it returns 2 to show that it's outside
		 * will be taken into account the switch/case filter i*//*
		printf("LineInside = placedholder\n");
		return 2;*//*doesn't belong to the macro*//*
	}*/
}

/*

int checkMacroExapnd(char *line, char *start, int pos)
{
	char* str = line;

if ((strncmp(MACRO_END_WORD, start, MACRO_END_LEN) != 0) && (strncmp(MACRO_END_WORD, start, MACRO_END_LEN)!=0)){
	str = str + pos;
}
*/
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

	} else report_error(ERR_NO_MORE_FILES, line_count);
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
	switch (type) {
		case ALPHA:
			while (i < len && isalpha(str[i])) i++;

			return i == len ? 1 : 0;
		case ALPHANUM:
			while (i < len && isalpha(str[i])) i++;
			/*check condition where after the alphabet there are digits which is acceptable*/
			while (i < len && isdigit(str[i])) i++;

			return i == len ? 1 : 0;

		default:
			break;
	}
}

string_separator_t string_sep(char *line) {
	int strings_count = 0;
	char *s;
	string_separator_t ssr_t = {0};
	while (isspace(*line)) line++;
	if (*line == '\0') {
		return ssr_t;
	}

	do {
		ssr_t.strings[strings_count++] = line;
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
	ssr_t.strings_count = strings_count;
	return ssr_t;
}


int isLineEmpty(char *line) {
	while (isspace(*line)) line++;
	if (*line != '\0' && *line != '\n')
		return 0;
	return 1;
}

/*
 * line inside is not check for syntax errors it's will
 * be check at the later parsing stage
 * */

/*not necessary*/
/*if (!(isLineEmpty(line))) {
	report_error(ERR_MACRO_END, line_count);
	return 0;
}*/

int dummy() {
	return 1;
}
/*int checkLineOutside(char*,char*,int);*/




