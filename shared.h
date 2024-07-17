

#ifndef M14_SHARED_H
#define M14_SHARED_H



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





/**
 * reports of macro_error as well as which line
 */
void report_error(char* r);
/*check if name is only alpha or alpha ending numerals*/
/*uses the ENUM CHECK_LEGAL_NAME*/
int checkLegalName(char* str , int);


#endif
