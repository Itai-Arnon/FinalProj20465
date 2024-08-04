#include <ctype.h>
#include <string.h>
#include "headers/utils.h"
#include "headers/error.h"


int isRestOfLineEmpty(char *line) {
	int idx = 0;
	int LEN = strlen(line);
	for(idx = 0 ; idx < LEN ; idx++)
		if ( !isspace(line[idx]) &&  line[idx]!= '\0' && LEN > 1   )
			return 0;
	return 1;
}


int findSeperator(char* str, char sep[], int num_of_sep) {
	int LEN = strlen(str);
	int i, j;
	i = j = 0;
	for (i = 0; str[i] < LEN; i++) {
		for (j = 0; j < num_of_sep; j++) {
			if (str[i] == sep[j] && i < (LEN -1 )  )
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





int int_to_octal(int num) {
	int answer = 0, y = 1;

	while (num != 0) {
		answer += (num % 8) * y;
		num = num / 8;
		y *= 10;
	}

	return answer;
}

int count_char_until_not_separator(char line[], char c, int *offset, char separators[], int separators_amount){
	int count = 0;
	int i = 0;
/* is char operator checks _seperators_ string - it's flexible  */
	while (is_char_separator(line[*offset + i], separators, separators_amount)==1){
		if (line[*offset + i] == c)
			count++;
		(*offset)++;
	}
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

int extra_char_at_end(const char line[], int loc){
	while (line[loc] == ' ' || line[loc] == '\t')
		loc++;

	if (line[loc] == '\0')
		return 0;

	return 1;
}

sep_whitespace_t string_sep(char *line) {
	int strings_count = 0;
	char *s;
	sep_whitespace_t ssr_t = {0};
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
void removeColon(char* symbol_name , char* symbol_name_c){
	int length = strlen(symbol_name);

	if(symbol_name[length]  == ':'){
		strncpy(symbol_name_c,symbol_name, length - 1);
	}
	else
		symbol_name_c = symbol_name;
}


		sep_commas_t get_comma_seps(char *str)
{
	sep_commas_t sep_res = {0};
	char *s;
	int seps_count = 0;
	if (*str == '\0')
	{
		return sep_res;
	}
	do
	{
		sep_res.seps[seps_count] = str;
		seps_count++;
		s = strchr(str, ',');
		if (s)
		{
			*s = '\0';
			s++;
			if (*s == ',')
			{
				sep_res.fault_line = 1;
				return sep_res;
			}
		}
		str = s;
	} while (s);
	sep_res.seps_count = seps_count;
	return sep_res;
}

int checkLegalName(char *str, check_legal_name type) {
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
}

void removeFrontalWhitespace(char* buffer , int*pos){
	*pos = 0;
	while(isspace(buffer[*pos] ))
		++(*pos);
}


