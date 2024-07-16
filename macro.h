
#ifndef MACRO_H
#define MACRO_H


#define MAX_FILE_LENGTH 400
#define LINE_LENGTH 80
#define MAX_MACRO_NAME 10
#define MAX_MACROS 20

#define MACRO_START_WORD "macr"
#define MACRO_START_LEN 4
#define MACRO_END_WORD "endmacr"
#define MACRO_END_LEN 7
#define MAX_MACRO_NAME 10
#define MAX_MACROS 20


#define EXT_BEFORE ".txt" /*TODO change to appropriate extension once in linux*/
#define EXT_AFTER "out.txt"
#define ERR_FILE_BEFORE ".as File Failed To Open"
#define ERR_FILE_AFTER ".am File Failed To Create"
#define ERR_NO_MORE_FILES "No Files Inserted"
#define ERR_MACRO_DEFINE "Error In Macro Definition"
#define ERR_MACRO_END "Error In Macro End Definition"
#define ERR_WRITING_MACRO  "Failed to write Macro"

#define PATH_BASE "..\\" /*TODO change when moving to linux*/

#define SPACES  " \t\v\f"




typedef struct  {
	char *strings[80];
	int strings_count;
}string_separator_t;



typedef enum{MACRO_START, MACRO_END , MACRO_EXPAND , LINE_INSIDE , LINE_OUTSIDE , MACRO_ERROR }MACRO_STATE_T;
typedef enum{ALPHA,ALPHANUM}CHECK_LEGAL_NAME;

/*int  getErrorLoci(FILE*);*/
/*function that will locate line number using ftell or fpos see page 248 in c books*/

 /*returns two file ptrs based and argv and argc */
FILE* initPointerFile(int ,char**,FILE*, int );/*returns ptr to current .as  or .am file*/
/**
 * prime function of the pre-compiler
 * manages all the parsing
 * @param receives argc and argv
 */
void readline (int, char**);
/**
  * @param arr
 *  length of non null terminated strings
 */
int nonNullTerminatedLength(char* arr);
/**
 * decides where is the line in pre processor terms
 */
int typeofline(char* line);
/*create a node specify name and store one macro line in it */
/*macro_t *createNode(macro_t *mptr , char name[], char line[]);*/
/*macro_t *createNewMacro(macro_t *mptr , char name[], char line[]);*/
int dummy();
/**
 * checks if macro start, retunrs 1 or 0
 * @param pos 
 */
int checkMacroStart(char* , char *,int pos);
/**
 * checks if macro start, returns 1 or 0
 * @param pos 
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


#endif /*MACRO_H*/

