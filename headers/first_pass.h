

#ifndef M14_FIRST_PASS_H
#define M14_FIRST_PASS_H
#include "shared.h"
#include "symbols.h"

typedef unsigned short word_t;

typedef struct line_t{
	word_t *word;
	int line_num;
	symbol_t *symbol; /*check if feasible*/
	struct line_t *next;
}line_t;

typedef struct word_table_t{
	line_t *lines;
	int length;
}word_table_t;



#endif /*M14_FIRST_PASS_H*/
