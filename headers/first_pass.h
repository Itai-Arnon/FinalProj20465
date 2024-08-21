

#ifndef M14_FIRST_PASS_H
#define M14_FIRST_PASS_H

#include "shared.h"
#include "symbols.h"
#include "macro_list.h"

#define opcode_shift 11
#define operand_shift 3
#define operand_size 4
#define register_shift 3
#define register_size 3
#define immediate_shift 3
#define label_shift 3



#define _offset  100


typedef unsigned short word_t;

typedef struct line_t {
	word_t word;
	int line_num;
	ARE_T _ARE;/* A,R,E */
	symbol_t *symbol; /*check if feasible*/
} line_t;

/*the base of the instruction /data table */
typedef struct word_table_t {
	line_t *lines;
	int size;
} word_table_t;


/*processes parser struct one line at a time and translates to machine language*/
void first_pass( symbol_table_t *, word_table_t *, word_table_t *);

/*process .string directive*/
void set_STRING_WORDS(symbol_table_t *sym_tbl, word_table_t *dataTable);

/*processes 1st instruction word*/
void setOPCODE_INSTRUCTION(symbol_table_t *, word_table_t *);

/*processes 2nd and 3rd  words if necessary*/
void setOPCODE_WORDS(symbol_table_t *, word_table_t *, int, int);

/*process .data directive*/
void set_DATA_WORDS(symbol_table_t *, word_table_t *);

void set_EXTnEntry(symbol_table_t *sym_tbl, word_table_t *dataTable);

void set_number_data_word(word_t *word, char value);

void set_char_string_word(word_t *word, char value);

/*sets opcode type in instruction word |op_code - 0 to 15 */
void set_opcode_into_word(word_t *word, op_code_t op_code);

/*sets operand type in instruction word |operand_t: destination or source */
void set_operand_type_into_word(word_t *word, operand_t otype, type_of_register_t type);

/*insert register into new empty word| isSource - shift 3 bit more*/
void set_register_into_empty_word(word_t *word, int value, int isSource);

/*insert label into new empty word|  shift 3 bit */
void set_label_into_empty_word(word_t *word, int value);

/*insert immediate into new empty word| shift 3 bit */
void set_immediate_into_empty_word(word_t *word, unsigned short value);

/*insert ARE into instruction word */
void set_ARE_into_word(word_t *word, ARE_T are);

/*insert final computed value */
void set_value_to_word(word_t *word, int value);

/*initiate word or data table*/

word_table_t *initTable(word_table_t *wordTable , int memoryInit) ;

/*reallocate more places in table*/
line_t *add_line(word_table_t *, int, symbol_t * , memory_t _ARE);


void addNumberToWordTable(word_table_t *table, int number);

unsigned short convertToTwoComp(unsigned short num);

int registerSelection();

#endif /*M14_FIRST_PASS_H*/
