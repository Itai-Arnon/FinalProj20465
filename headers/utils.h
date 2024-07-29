

#ifndef M14_UTILS_H
#define M14_UTILS_H

/*colors*/
#define BLACK_COLOR "\033[0m"
#define RED_COLOR "\x1b[38;2;247;84;100m"

#define SPACES  " \t\v\f"

typedef enum{ALPHA,ALPHANUM, ALPHANUM_COMBINED}check_legal_name;



typedef struct  {
	char *strings[80];
	int strings_count;
}string_separator_t;

typedef


sepBase create_sep_arr(char* str)
{
	/*type changes between read_set and all other function enum for read_set others*/
	sepBase sep_res = {0};
	char* s;
	int seps_count = 0;
	/*int comma_count = countCommas(str);
	printf("%d\n", comma_count);
*/

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

				strcpy(parser.err_buff, ERR_COMMA);
				printf("%s\n", parser.err_buff);
				parser.oper = ERROR_OPER;
				sep_res.seps[seps_count] = '\0';
				return sep_res;
			}
		}
		str = s;
	}
	while (s);
	sep_res.seps_count = seps_count;
	if (comma_count + 1 < seps_count)
	{
		sep_res.fault_line = 1;
		strcpy(parser.err_buff, ERR_COMMA);
		printf("%s\n", parser.err_buff);
		parser.oper = ERROR_OPER;
		return sep;
	}

	return sep_res;
}


/*checks that the tail of a cmd sentence is free of misplaced chars- */
/*finds first occurance of non white space - */
int isRestOfLineEmpty(char *);
/*find the first occurane of chars in sep[]*/
int findSeperator(char* str, char sep[], int num_of_sep);
int int_to_octal(int num);
void ret_next_word(char line_text[], int *offset, char line[], char separators[], int separators_amount);
void jump_spaces_and_tabs(int *offset, char line[]);

void ret_next_word_n_skip(char line_text[], int *offset, char line[], char separators[], int separators_amount);
int count_char_until_not_separator(char line[], char c, int *offset, char separators[], int separators_amount);
int is_char_separator(char c, char separators[], int separators_amount);
int extra_char_at_end(const char line[], int loc);
/**
 * creates an array of separated strings using strpbrk
 */
string_separator_t string_sep(char* );
/*purposed to count non null terminated functions*/
int nonNullTerminatedLength(char *arr);
/*uses the ENUM CHECK_LEGAL_NAME*/
/*checks chars containing str, alphanum , alpha*/
int checkLegalName(char *str, check_legal_name type);



#endif /* M14_UTILS_H*/
