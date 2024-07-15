#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>
#include "macro.h"

/*int getErrorLoci(FILE* file){}*/

FILE *fptr_before = NULL;
FILE *fptr_after = NULL;
macro_t *macptr = NULL;
int currentLine = 0;

int main(int argc, char *argv[]) {

	char *buffer = calloc(LINE_LENGTH, sizeof (char)) ;
	char start[MACRO_END_LEN];
	int pos = 0;
	char *temp = &buffer[0];
	int i = 0;

	strcpy(buffer,  "endmacr    \0");

	while (isspace(buffer[i] != '\0' && buffer[i++]));
	if (*buffer == '\0' || isdigit(*buffer))
		return 0;

	i = 0;
	if (sscanf(buffer, "%s%n", start, &pos) == 1) {
		if (!checkLegalName(start)) {
			macro_error(ERR_MACRO_DEFINE);
			return (MACRO_ERROR);
		} else
			printf("%d\n", checkMacroEnd(buffer, start, pos));
	}
	/*	printf("%d\n", checkMacroEnd(buffer, start, pos));*/

	return 0;
}

int dummy() {
	return 1;
}

int typeofline(char *line) {
	char start[7];
	char macro_name[MAX_MACRO_NAME];
	char *str = line;
	int pos = 0;
	if (sscanf(str, "%s%n", start, &pos) == 1) {

		if (checkMacroStart(str, start, pos))
			return MACRO_START;
		else if (checkMacroEnd(str, start, pos))
			return MACRO_END;
		else if (macptr != NULL)
			return LINE_INSIDE;
		else if (macptr == NULL)
			return LINE_OUTSIDE;


	} else {
		macro_error(ERR_MACRO_DEFINE);
		return MACRO_ERROR;/*TODO: placeholder for is macro expandelse if (dummy() == 1)*//*TODO:  use check name
		return MACRO_EXPAND;*/
	}
}

void readline(int _argc, char **_argv) {
	char buffer[LINE_LENGTH];
	char *macro_buffer;
/*TODO add a way to save line numbers*/
	int count = 0, end = 0;
	fptr_before = initPointerFile(_argc, _argv, fptr_before, 0);
	fptr_after = initPointerFile(_argc, _argv, fptr_after, 1);

	while (fgets(buffer, 10, fptr_before) != NULL);

	printf("%s\n", buffer);
	/*later will head the switch*/
	/*typeofline(char* line)*/
	fclose(fptr_before);
	fclose(fptr_after);
}

int checkMacroStart(char *line, char *start, int pos) {

	char *str = line;
	char macro_name[MAX_MACRO_NAME];

	if (strncmp(MACRO_START_WORD, start, MACRO_START_LEN) == 0) {
		printf("%s\n", str);
		str = str + pos + 1;
		pos = 0;
		printf("%s\n", str);
		if (sscanf(str, "%s%n", macro_name, &pos) == 1) {/*check for actual macro name*/
			printf("%s\n", macro_name);
			str = str + pos + 1;
			while (isspace(*str)) {
				str++;
			}
			if (*str != '\0' && *str != '\n') {
				macro_error(ERR_MACRO_DEFINE);
				return 0;
			} else
				return 1;/*==0*/
		}

	}
	macro_error(ERR_MACRO_DEFINE);
	return 0;
}

int checkMacroEnd(char *line, char *start, int pos) {
	char *str = line;

	printf("beginning line:%s start:%s\n", line, start);
	printf("macro_end_word len %lu || start len %lu ||", strlen(MACRO_END_WORD), strlen(start));
	if (strncmp(MACRO_END_WORD, start, MACRO_END_LEN+1) == 0) {
		str = str + pos;
		printf("inside strncmp line:%s    start:%s\n", str, start);
		while (isspace(*str)) {
			str++;
		}
		if (*str != '\0' && *str != '\n') {

			macro_error(ERR_MACRO_END);
			return 0;
		} else
			return 1;
	} else {/* in case it didn't recognize endmacr*/
		printf("%s\n", str);
		return 0;
	}

}


int nonNullTerminatedLength(char *arr) {
	int count = 0;
	while (arr[++count] != 0);
	return count;
}

/*type=0 for input file type =1 for output*/
FILE *initPointerFile(int _argc, char **_argv, FILE *fptr, int type) {
	char line[LINE_LENGTH];
	char filename_before[64];
	char filename_after[64];
	int argv_len = 0;
	if (_argc > 1) {/*argc  must always be subtracted by one*/
		argv_len = nonNullTerminatedLength(_argv[_argc - 1]);
		switch (type) {
			case 0:
				strcpy(filename_before, PATH_BASE);/*TODO no need in linux*/
				strncat(filename_before, _argv[_argc - 1], argv_len); /*TODO arg_c iteration tbd*/
				printf("%s\n", filename_before);
				if (!(fptr = fopen(filename_before, "r"))) {
					printf("%s\n", ERR_FILE_BEFORE);
					exit(0);
				}
				return fptr;
			case 1:
				strcpy(filename_after, PATH_BASE);/*TODO no need in linux*/
				strcat(filename_after, "out");
				strncat(filename_after, _argv[_argc - 1], argv_len);
				/*TODO add extension in linux*/
				printf("%s\n", filename_after);
				if (!(fptr = fopen(filename_after, "a"))) {
					printf("%s\n", ERR_FILE_AFTER);
					exit(0);
				}
				printf("success\n");
				return fptr;

			default:
				printf(" Error: You entered A wrong type value");
				break;
		}

	} else printf("%s\n", ERR_NO_MORE_FILES);
}

void macro_error(char *err) {

	printf("%s at line %lu\n", err, ftell(fptr_before) / 80);

}

int checkLegalName(char *str) {/*this function check if the non alpha numeric chars in a given string*/
	int i = 0;
	int len = nonNullTerminatedLength(str);
	while (i < len && isalpha(str[i])) {
		i++;
	}
	/*check condition where after the alphabet there are digits which is acceptable*/
	while (i < len && isdigit(str[i])) {
		i++;
	}
	return i == len ? 1 : 0;
}

string_seperator_t string_sep(char *line) {
	int strings_count = 0;
	char *s;
	string_seperator_t ssr_t = {0};
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

/*macro_table_t* loadFile(table* tbl, int* hash_arr, char** _argv)
{
	node* temp =NULL;
	node* last = NULL;
	char* f_name = _argv[idx];
	int i = 0;
	for (i = 0; i < ARR_SIZE; i++)
	{
		if (hash_arr[i] != 0)
		{
			temp = constructNode(i, hash_arr[i], f_name);
			if (tbl->slot[i] != NULL)
			{
				last = retEndList(tbl->slot[i]);
				last->next = temp;
			}
			else
				tbl->slot[i] = temp;

			temp->next = NULL;
		}
	}
	return tbl;
}*/
