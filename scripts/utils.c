#include <ctype.h>
#include <string.h>
#include "headers/utils.h"
#include "headers/global_vars.h"
#include "headers/assembler.h"

int isRestOfLineEmpty(char *line) {
	int idx = 0;
	int LEN = strlen(line);
	if (*line == '\0' || LEN == 0)
		return 1;

	for (idx = 0; idx < LEN; idx++) {
		if (*line && !isspace(line[idx]))
			return 0;
	}
	return 1;
}


int findSeperator(char *str, char sep ) {
	int LEN = strlen(str);
	int i, j;
	i = j = 0;
	for (i = 0; str[i] < LEN; i++) {
		if (str[i] == '\n' && i < (LEN - 1)) {
			while (isspace(*str++));
			if (!*str)
				return 1;
		}
	}
return 0;
}

int nonNullTerminatedLength(char *arr) {
	int count = 0;
	while (arr[++count] != 0);
	return count;
}


int is_char_separator(char c, char separators[], int separators_amount) {
	int i;
	for (i = 0; i < separators_amount; i++) {
		if (c == separators[i])
			return 1;
	}

	return 0;
}

int extra_char_at_end(char line[], int loc) {
	while (line[loc] == ' ' || line[loc] == '\t')
		loc++;

	if (line[loc] == '\0')
		return 0;

	return 1;
}


char *removeColon(char *symbol_name) {
	int length = strlen(symbol_name);
	char *symbol_name_c = malloc(sizeof(char));
	if (symbol_name[length] == ':') {
		strncat(symbol_name_c, symbol_name, length - 1);
		return symbol_name_c;
	}
	return symbol_name;
}


/*removes white spaces from noth sides*/




int checkLegalName(char *str, check_legal_name type) {
	int i = 0;
	int len = nonNullTerminatedLength(str);
	if (!isalpha(str[0])) return 0;
	switch (type) {
		case ALPHA:
			while (i < (len - 1) && isalpha(str[i])) ++i;
			break;
		case ALPHANUM:
			while (i < (len - 1) && isalpha(str[i])) ++i;
			/*check condition where after the alphabet there are digits which is acceptable*/
			while (i < (len - 1) && isdigit(str[i])) ++i;
			break;


		case ALPHANUM_COMBINED:
			/*check condition where after the alphabet there are digits which is acceptable*/
			while (i < (len - 1) && isalnum(str[i])) ++i;
			break;

		default:
			break;
	}

	if (isalnum(str[i]) || str[i] == ':')
		return i == (len - 1) ? 1 : 0;
	else
		return 0;
}

void removeFrontalWhitespace(char *buffer, int *pos) {
	*pos = 0;
	while (isspace(buffer[*pos]))
		++(*pos);
}

char *strstrip(char *s) {
	int LEN;
	char *end;
	LEN = strlen(s);
	if (!LEN)
		return s;
	end = s + LEN - 1;
	while (end >= s && isspace(*end))
		end--;
	*(end + 1) = '\0';
	while (*s && isspace(*s))
		s++;

	return s;
}

int countCommas(char *str) {
	int count = 0;
	int i;
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == ',') {
			count++;
		}
	}
	return count;
}


int convertOrCheckStringToNum(char *str, convert_func_t type) {
	char *endtoken;
	unsigned long num = strtol(str, &endtoken, 10);


	switch (type) {
		case 0:
			if (*endtoken != '\0') {
				report_error(ERR_FAILED_TO_CONVERT_NUMBER, line_count, UTIL, CRIT, 0);
			} else
				return (int) num;
		case 1:
			if (*endtoken != '\0') {
				return 0;
			} else
				return 1;
	}
}


/*count the numbers in a string to check valditiy of .DATA directive */
int countNumbersInString(char *str) {
	int count = 0;
	int i = 0;
	int inNumber = 0;

	while (str[i] != '\0') {
		/* Check if the current character is a digit or a minus sign */
		if (isdigit(str[i]) || (str[i] == '-' && isdigit(str[i + 1]))) {
			if (!inNumber) {
				count++;
				inNumber = 1;
			}
		} else {
			inNumber = 0;
		}
		i++;
	}
	return count;
}

/*under the premise that in the string there is no whitespace before or after*/
int checkQuotes(char *str) { /*todo erase */
	/*check if string is not empty*/
	int len = strlen(str);
	if (len < 2) {
		return 0;
	} else len = len - 2;

	return 0;
}



