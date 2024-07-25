#include <ctype.h>
#include <string.h>
#include "utils.h"

/*check is the rest of the line is filled with unwanted char*/
/*the first unwanted char disqualifies the sentence*/

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



int is_char_separator(char c, char separators[], int separators_amount);

int int_to_octal(int num) {
	int answer = 0, y = 1;

	while (num != 0) {
		answer += (num % 8) * y;
		num = num / 8;
		y *= 10;
	}

	return answer;
}

void jump_spaces_and_tabs(int *offset, char line[]) {
	while (line[*offset] == ' ' || line[*offset] == '\t')
		(*offset)++;
}
void ret_next_word(char line_text[], int *offset, char line[], char separators[], int separators_amount){
	int i = 0;

	while (is_char_separator(line[i+*offset], separators, separators_amount)==0) {
		line_text[i] = line[i+*offset];
		i++;
	}

	line_text[i] = '\0';
	*offset += i;
}


void ret_next_word_n_skip(char line_text[], int *offset, char line[], char separators[], int separators_amount){
	jump_spaces_and_tabs(offset, line);
	ret_next_word(line_text, offset, line, separators, separators_amount);
	jump_spaces_and_tabs(offset, line);
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

/* seperastor char array checks all given amount by __seperators__amont__ */
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
