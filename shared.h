

#ifndef M14_SHARED_H
#define M14_SHARED_H



#define LINE_LENGTH 80
#define MAX_MACRO_NAME 31
#define MAX_MACROS 20

#define MACRO_START_WORD "macr"
#define MACRO_END_WORD "endmacr"
#define MACRO_END_LEN 7
#define MACRO_START_LEN 4


/*writing Macro Error and File Errors*/

#define ERR_MACRO_NAME_LONG "Macro Name Too Long"
#define ERR_FILE_BEFORE ".as File Failed To Open"
#define ERR_FILE_AFTER ".am File Failed To Create"
#define ERR_NO_MORE_FILES "No Files Inserted"
#define ERR_MACRO_DEFINE "Error In Macro Start Definition"

#define ERR_MACRO_END "Error In Macro End Definition"

#define ERR_WRITING_MACRO  "Failed to write Macro"
#define EER_MACRO_TABLE_FULL "Error No more space in Macro Table"
#define ERR_MACRO_PERMISSION "Error Writing Macro Permission Mistake"
#define ERR_MACRO_NODE_CREATION_FAILED "Macro Node Allocation Failed"




/*text add in */
#define PATH_BASE "..\\" /*TODO change when moving to linux*/
#define EXT_BEFORE ".txt" /*TODO change to appropriate extension once in linux*/
#define EXT_AFTER "out.txt"
#define SPACES  " \t\v\f"





/**
 * reports of macro_error as well as which line
 */
void report_error(char* r);
/*check if name is only alpha or alpha ending numerals*/
/*uses the ENUM CHECK_LEGAL_NAME*/
int checkLegalName(char* str , int);


#endif
