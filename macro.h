
#ifndef MACRO_H
#define MACRO_H

#define MAX_MACRO_NAME  10
#define MAX_FILE_LENGTH 400
#define LINE_LENGTH 80
#define MACRO_START_WORD "macr"
#define MACRO_START_LEN 4
#define MACRO_END_WORD "endmacr"
#define MACRO_END_LEN 7



#define EXT_BEFORE ".txt"/*TODO change to appropriate extension once in linux*/
#define EXT_AFTER "out.txt"
#define ERR_FILE_BEFORE ".as File Failed To Open"
#define ERR_FILE_AFTER ".am File Failed To Create"
#define ERR_NO_MORE_FILES "No Files Inserted"
#define ERR_MACRO_DEFINE "Error In Macro Definition"
#define ERR_MACRO_END "Error In Macro End Definition"

#define PATH_BASE "..\\" /*TODO change when moving to linux*/
#define MAX_MACRO_NAME 10
#define MAX_MACROS 20
#define SPACES  " \t\v\f"


typedef struct macro_t{
	char name[MAX_MACRO_NAME];/*ref to macro name*/
	char mbuffer[LINE_LENGTH];/*holds all contents of macro - dynamically allocated*/
	struct macro_t *macro_next;
}macro_t;

typedef struct {
	macro_t *tbl[MAX_MACROS];
}macro_table_t;



typedef struct  {
	char *strings[80];
	int strings_count;
}string_seperator_t;



typedef enum{MACRO_START, MACRO_END , MACRO_EXPAND , LINE_INSIDE , LINE_OUTSIDE , MACRO_ERROR }MACRO_STATE_T;


/*int  getErrorLoci(FILE*);*/
/*function that will locate line number using ftell or fpos see page 248 in c books*/

 /*returns two file ptrs based and argv and argc */
FILE* initPointerFile(int ,char**,FILE*, int );/*returns ptr to current .as  or .am file*/
void readline (int, char**);
int nonNullTerminatedLength(char* arr);/*argv is not null terminated, this counts its length*/
int typeofline(char* line);/*decides where is the line in pre processor terms*/
/*create a node specify name and store one macro line in it */
/*macro_t *createNode(macro_t *mptr , char name[], char line[]);*/
/*macro_t *createNewMacro(macro_t *mptr , char name[], char line[]);*/
int dummy();
int checkMacroStart(char* , char *,int pos);
int	checkMacroEnd(char *,char *, int pos);
void macro_error(char* r);
int checkLegalName(char* str);
string_seperator_t string_sep(char * str);
#endif /*MACRO_H*/

