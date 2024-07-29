

#ifndef M14_SHARED_H
#define M14_SHARED_H



#define LINE_LENGTH 80
#define MAX_MACRO_NAME 31
#define MAX_MACROS 20
#define MAX_SYMBOL_NAME 20

#define MACRO_START_WORD "macr"
#define MACRO_END_WORD "endmacr"
#define MACRO_END_LEN 7
#define MACRO_START_LEN 4


/*text add in */
#define PATH_BASE "..\\" /*TODO change when moving to linux*/
#define EXT_BEFORE ".txt" /*TODO change to appropriate extension once in linux*/
#define EXT_AFTER "out.txt"


#define MACRO_TABLE_FULL "Macro Table full allocate more place"

/*writing Macro Error and File Errors*/

#define ERR_FILE_BEFORE ".as File Failed To Open"
#define ERR_FILE_AFTER ".am File Failed To Create"
#define ERR_MACRO_NAME_LONG "Macro Name Too Long"
#define ERR_NO_FILES "No Files Inserted"
#define ERR_MACRO_DEFINE "Error In Macro Start Definition"
#define ERR_MACRO_END "Error In Macro End Definition"
#define ERR_MACRO_EXPAND "Error In Macro Expand Definition"
#define ERR_MACRO_NAME_DUP "Error Duplicate Macro Name"

/*symbol op code  directive related errors*/
#define ERR_MACRO_NAME_OP_DIRECT_SYMBOL "Macro name is a opcode, a directive or a symbol"
#define ERR_FAIL_CREATE_SYMBOL_TBL "Symbol Table Creation Failed"
#define ERR_FAIL_CREATE_SYMBOL "Symbol  Creating Symbol Node Failed"
#define ERR_OP_CODE_RECOGNITION "OP CODE NOT RECOGNIZED"
#define ERR_OP_CODE_FAILED_STRUCTURE "OP Code sturcture Not Legal"
#define ERR_DIRECTIVE_RECOGNITION "Directive Not Recognized"



#define ERR_WRITING_MACRO  "Failed to write Macro"
#define EER_MACRO_TABLE_FULL "Error No more space in Macro Table"
#define EER_MACRO_TABLE_RETREIVE "Error Retreiving From Macro Table"
#define ERR_MACRO_TABLE_GENERAL_ERROR "General Macro Table Error"
#define ERR_MACRO_PERMISSION "Error  Macro Permission Mistake"
#define ERR_MACRO_NODE_CREATION_FAILED "Macro Node Allocation Failed"
#define ERR_LINE_LENGTH "Line is too long"
#define ERR_CHK_UNDEFINED_MACRO "Checking Macro that is Undefined"









/**
 * reports of macro_error as well as which line
 */
void report_error(char* err , int linecount);
/*check if name is only alpha or alpha ending numerals*/
/*uses the ENUM CHECK_LEGAL_NAME*/



#endif
