
#ifndef MACRO_H
#define MACRO_H

#include "linkedlist.h"



typedef enum{MACRO_START, MACRO_END , MACRO_EXPAND , LINE_INSIDE , LINE_OUTSIDE, EMPTY_LINE , MACRO_ERROR }MACRO_STATE_T;



 /*returns a file ptr based and argv and argc, based on index the last param
  * require more managament to increment the index*/
FILE* initSourceFiles(int ,char**,FILE*, int );/*returns ptr to current .as  */

/**
 * Creates a destination file & returns it file pointer.
 *
 * @param fptr The file pointer to be updated with the new file's pointer.
 * @return FILE* The updated file pointer if successful, NULL if an error occurred.
 */

FILE *initDestinationPointer(FILE *fptr, char *filename);

/**
 * prime function of the pre-compiler
 * manages all the parsing
 * @param receives argc and argv
 */
void read_preprocessor ( macro_table_t*);
/**
  * @param arr
 *  length of non null terminated strings
 */
int nonNullTerminatedLength(char* arr);
/**
 * decides where is the line in pre processor terms
 */
int typeofline( macro_table_t *tbl, char* line , char* macro_name);

/* checks if macro start, returns 1 or 0
*/
 int checkMacroStart(char* , char *, char *macro_name, int pos);
/* checks if macro ends, returns 1 or 0
*/
int	checkMacroEnd(char *,char *, int pos);

 /*
 * checks if macro line inside , returns 1 or 0
 */
int checkLineInside(char*,char*,int);
/*
 * checks if macro line outside , returns 1 or 0
 */
int checkLineOutside(char*,char*,int);
/*
 * checks if this is a macro and whether to expand it
 */
/*int checkMacroExpand(char*,char*,int)*/
/**
 * reports of macro_error as well as which line
 */
void macro_error(char* r);
/*check if name is only alpha or alpha ending numerals*/



 /* checks the line if its a macro and exapnds it*/
int checkMacroExpand(macro_table_t *tbl, char *line, char *start, char *macro_name, int pos);

#include <stdio.h>

/**
 * Checks if the given buffer contains a specific EOF marker.
 *
 * @param buffer The buffer to check.
 * @return int Returns 1 if EOF marker is found, 0 otherwise.
 */
int checkEOFInBuffer(char* buffer);
/*check if macro name has opcode directive or  symbol*/
int macro_name_has_opcode_direct_symbol(char*);

#endif /*MACRO_H*/

