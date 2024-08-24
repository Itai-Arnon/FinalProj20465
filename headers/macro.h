
#ifndef MACRO_H
#define MACRO_H

#include "macro_list.h"
#include "symbols.h"
#include "shared.h"





/**

 *
 */


/**
 * prime function of the pre-compiler
 * manages all the parsing
 * @param receives argc and argv
 */
void read_preprocessor ( macro_table_t*, symbol_table_t *sym_tbl);
/**
  * @param arr
 *  length of non null terminated strings
 */
int nonNullTerminatedLength(char* arr);
/**
 * decides where is the line in pre processor terms
 */
int typeofline( macro_table_t *tbl, char*  , char*  , symbol_table_t* );

/* checks if macro start, returns 1 or 0
*/
 int checkMacroStart(char* , char *, char *, int , symbol_table_t *,  macro_table_t* );
/* checks if macro ends, returns 1 or 0
*/
int	checkMacroEnd(char *,char *, int pos);


 /* checks the line if its a macro and exapnds it*/
int checkMacroExpand(macro_table_t *tbl, char *line, char *start, char *macro_name, int pos);

/*filters non valid sentences*/
int isHeadOfSentenceValid(macro_table_t*tbl, char *buffer);
/* Checks if the given buffer contains a specific EOF marker.*/
int checkEOFInBuffer(char* buffer);
/*check if macro name has opcode directive or  symbol*/
int macro_name_duplicate(char*);

#endif /*MACRO_H*/

