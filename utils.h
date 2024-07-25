

#ifndef M14_UTILS_H
#define M14_UTILS_H

/*colors*/
#define BLACK_COLOR "\033[0m"
#define RED_COLOR "\x1b[38;2;247;84;100m"

#define SPACES  " \t\v\f"




typedef struct  {
	char *strings[80];
	int strings_count;
}string_separator_t;



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
string_separator_t string_sep(char*);




#endif /* M14_UTILS_H*/
