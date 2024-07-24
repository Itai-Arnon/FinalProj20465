
#ifndef MACRO_H
#define MACRO_H

#include <linkedlist.h>




typedef struct  {
	char *strings[80];
	int strings_count;
}string_separator_t;



typedef enum{MACRO_START, MACRO_END , MACRO_EXPAND , LINE_INSIDE , LINE_OUTSIDE, EMPTY_LINE , MACRO_ERROR }MACRO_STATE_T;
typedef enum{ALPHA,ALPHANUM, ALPHANUM_COMBINED}CHECK_LEGAL_NAME;

/*int  getErrorLoci(FILE*);*/
/*function that will locate line number using ftell or fpos see page 248 in c books*/

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
void readline (int, char** , macro_table_t*);
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
/*uses the ENUM CHECK_LEGAL_NAME*/
int checkLegalName(char* str , int);
/**
 * creates an array of separated strings using strpbrk
 */
string_separator_t string_sep(char*);

/**
 * check if rest of line is whitspace
 * @return int
 */
int isLineEmpty(char *);
/**
 * checks the line if its a macro and exapnds it
 * @param line
 * @param start
 * @param pos
 * @return
 */
int checkMacroExpand(macro_table_t *tbl, char *line, char *start, char *macro_name, int pos);

#include <stdio.h>

/**
 * Checks if the given buffer contains a specific EOF marker.
 *
 * @param buffer The buffer to check.
 * @return int Returns 1 if EOF marker is found, 0 otherwise.
 */
int checkEOFInBuffer(char* buffer);

#endif /*MACRO_H*/

