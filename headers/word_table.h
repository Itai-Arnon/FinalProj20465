

#ifndef M14_WORD_TABLE_H
#define M14_WORD_TABLE_H


#include "symbols.h"

typedef unsigned short word_t;

typedef struct line_t{
	word_t *word;
	int line_num;
	symbol_t *symbol; /*check if feasible*/

}line_t;

/*the base of the instruction /data table */
typedef struct word_table_t{
	line_t *lines;
	int size;
	line_t *end;
	struct word_table_t *next;
}word_table_t;


#endif
