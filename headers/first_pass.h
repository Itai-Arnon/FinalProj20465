

#ifndef M14_FIRST_PASS_H
#define M14_FIRST_PASS_H
#include "shared.h"
#include "symbols.h"

#define opcode_shift 11
#define operand_shift 3
#define operand_size 4
#define register_shift 3
#define register_size 3
#define immediate_shift 3
#define label_shift 3


#define initTableSize 1

/*processes parser struct one line at a time and translates to machine language*/
void first_pass(symbol_table_t *sym_tbl, word_table_t * , word_table_t *) ;

/*process .string directive*/
void set_STRING_WORDS(symbol_table_t *sym_tbl , word_table_t *dataTable);

/*process .data directive*/
void set_DATA_WORDS(symbol_table_t * , word_table_t *);

/*processes 1st instruction word*/
void setOPCODE_INSTRUCTION(symbol_table_t *, word_table_t * );
/*processes 2nd and 3rd  words if necessary*/
void setOPCODE_WORDS(symbol_table_t *, word_table_t*, int ,int);

int registerSelection();

int convertToTwoComp(int num);


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
void set_immediate_into_empty_word(word_t *word, int value);

/*insert ARE into instruction word */
void set_ARE_into_word(word_t *word, ARE_T are);

/*insert final computed value */
void set_value_to_word(word_t *word, int value);

/*initiate word or data table*/

word_table_t* initTable(word_table_t *wordTable);

/*reallocate more places in table*/
line_t *add_line(word_table_t *, int, symbol_t *);




#endif /*M14_FIRST_PASS_H*/
