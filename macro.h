
#ifndef MACRO_H
#define MACRO_H

#define MAX_MACRO_NAME  10
#define MAX_FILE_LENGTH 400
#define LINE_LENGTH 80

#define EXT_BEFORE ".txt"/*TODO change to appropriate extension once in linux*/
#define EXT_AFTER "out.txt"
#define ERR_FILE_BEFORE ".as File Failed To Open"
#define ERR_FILE_AFTER ".am File Failed To Create"
#define ERR_NO_MORE_FILES "No Files Inserted"
#define PATH_BASE "..\\" /*TODO change when moving to linux*/
#define MACRO_START_WORD "macr"
#define MACRO_END_WORD "endmacr"
#define MAX_MACRO_NAME 10

typedef struct macro_t{
	char name[MAX_MACRO_NAME];/*ref to macro name*/
	char mbuffer[LINE_LENGTH];/*holds all contents of macro - dynamically allocated*/
	struct macro_t *macro_next;
}macro_t;

typedef enum{macro_start, macro_end , macro_expand , simple_inside , simple_outside }MACRO_STATE_T;


/*int  getErrorLoci(FILE*);*/
/*function that will locate line number using ftell or fpos see page 248 in c books*/

 /*returns two file ptrs based and argv and argc */
FILE* initPointerFile(int ,char**,FILE*, int );/*returns ptr to current .as  or .am file*/
void readline (int, char**);
int argv_length(char* arr);/*argv is not null terminated, this counts its length*/
int typeofline(char* line);/*decides where is the line in pre processor terms*/
/*create a node specify name and store one macro line in it */
/*macro_t *createNode(macro_t *mptr , char name[], char line[]);*/
int dummy();
#endif /*MACRO_H*/

