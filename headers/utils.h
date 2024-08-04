

#ifndef M14_UTILS_H
#define M14_UTILS_H

/*colors*/
#define BLACK_COLOR "\033[0m"
#define RED_COLOR "\x1b[38;2;247;84;100m"

#define SPACES  " \t\v\f"

#include "shared.h"



typedef struct  {/*struct aids to create strings of whitespaced items*/
	char *strings[80];
	int strings_count;
}sep_whitespace_t;

typedef struct /*struct aid create a string free of commas*/
{
	char* seps[50];
	int seps_count;
	int fault_line;
} sep_commas_t;





/*check is the rest of the line is filled with unwanted char*/
/*the first unwanted char disqualifies the sentence*/
int isRestOfLineEmpty(char *);
/*find the first occurane of chars in sep[]*/
int findSeperator(char* str, char sep[], int num_of_sep);



int int_to_octal(int num);



void ret_next_word_n_skip(char line_text[], int *offset, char line[], char separators[], int separators_amount);
int count_char_until_not_separator(char line[], char c, int *offset, char separators[], int separators_amount);
/* seperastor char array checks all given amount by __seperators__amont__ */
int is_char_separator(char c, char separators[], int separators_amount);
int extra_char_at_end(const char line[], int loc);
/**
 * creates an array of separated strings using strpbrk
 */
sep_whitespace_t string_sep_white_space(char* );
/*purposed to count non null terminated functions*/
int nonNullTerminatedLength(char *arr);
/*uses the ENUM CHECK_LEGAL_NAME*/
/*checks chars containing str, alphanum , alpha*/
int checkLegalName(char *str, check_legal_name type);

void removeColon(char* symbol_name , char* symbol_name_c);

void removeFrontalWhitespace(char*buffer , int*pos);

#endif /* M14_UTILS_H*/
