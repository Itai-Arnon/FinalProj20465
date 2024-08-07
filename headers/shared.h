

#ifndef M14_SHARED_H
#define M14_SHARED_H
#include <stdlib.h>


#define LINE_LENGTH 80
#define MAX_MACRO_NAME 31
#define MAX_MACROS 20
#define MAX_SYMBOL_NAME 20

#define MACRO_START_WORD "macr"
#define MACRO_END_WORD "endmacr"
#define MACRO_END_LEN 7
#define MACRO_START_LEN 4
#define MAX_DIRECTIVE_NAME 8


/*text add in */
/*#define PATH_BASE "c:\\OPU\\20465\\m\\m14\\"*/          /*TODO change when moving to linux*/
#define PATH_BASE "..\\"                                           /*ALternate PATH_BASE */
#define EXT_BEFORE ".txt" /*TODO change to appropriate extension once in linux*/
#define EXT_AFTER "a.txt"


#define MACRO_TABLE_FULL "Macro Table full allocate more place"

/*writing Macro Error and File Errors*/



#define ERR_LINE_LENGTH "Line is too long"
#define ERR_FILE_BEFORE ".as File Failed To Open"
#define ERR_FILE_AFTER ".am File Failed To Create"


/*macro related*/
#define ERR_WRITING_MACRO  "Failed to write Macro"
#define EER_MACRO_TABLE_FULL "Error No more space in Macro Table"
#define EER_MACRO_TABLE_RETREIVE "Error Retreiving From Macro Table"
#define ERR_MACRO_TABLE_GENERAL_ERROR "General Macro Table Error"
#define ERR_MACRO_PERMISSION "Error  Macro Permission Mistake"
#define ERR_MACRO_NODE_CREATION_FAILED "Macro Node Allocation Failed"
#define ERR_MACRO_NAME_LONG "Macro Name Too Long"
#define ERR_MACRO_NAME_WRONG "Macro Name Illegal"
#define ERR_NO_FILES "No Files Inserted"
#define ERR_START_MACRO_DEF "Error In Macro Start Definition"
#define ERR_MACRO_END "Error In Macro End Definition"
#define ERR_MACRO_EXPAND "Error In Macro Expand Definition"
#define ERR_MACRO_NAME_DUP "Error Duplicate Macro Name"
#define ERR_MACRO_NOT_FOUND "Cannot find Macro"

/* symbol table related , symbol.c */
#define ERR_DUPLICATE_SYMBOL_NAME  "DUPLICATE SYMBOL NAME"
#define ERR_FAIL_CREATE_SYMBOL_TBL "Symbol Table Creation Failed"
#define ERR_FAIL_CREATE_SYMBOL "Symbol  Creating Symbol Node Failed"
#define ERR_SYMBOL_TABLE_EMPTY "Symbol Table Empty" /*non critial*/



/* front: op code  directive related errors*/
#define ERR_MACRO_NAME_OP_DIRECT_SYMBOL "Macro name is a opcode, a directive or a symbol"
#define ERR_OP_CODE_RECOGNITION "OP CODE NOT RECOGNIZED"
#define ERR_OP_CODE_FAILED_STRUCTURE "OP Code structure Unrecognized or Not Legal"
#define ERR_DIRECTIVE_RECOGNITION "Directive Not Recognized or Not Legal"
#define ERR_LINE_UNRECOGNIZED "Line Not Recognized as OP_CODE or DIRECTIVE"
#define ERR_GENERAL_FIRST_PASS_ERROR "General First Pass Erro"




typedef enum {
	DATA,
	STRING,
	ENTRY,
	EXTERN,
}directive_enums;
/*pass1 and pass2*/
typedef enum {
	mov, cmp, add, sub, lea, clr, not, inc, dec, jmp,
	bne, red, prn, jsr, rts, stop
} op_code;

/*macros*/
typedef enum{MACRO_START, MACRO_END , MACRO_EXPAND , LINE_INSIDE , LINE_OUTSIDE, EMPTY_LINE , MACRO_ERROR }MACRO_STATE_T;
/*utils */
typedef enum{ALPHA,ALPHANUM, ALPHANUM_COMBINED}check_legal_name;

typedef  enum{NO, YES }isUpdate;
 /* reports of macro_error as well as which line
 */
typedef enum{CRIT,NON_CRIT}err_type;

/*meant to decipher between reg behvior*/

typedef enum {IMMEDIATE,DIRECT,INDIRECT,REGULAR}typeOfRegister;
#endif
