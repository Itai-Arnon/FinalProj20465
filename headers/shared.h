

#ifndef M14_SHARED_H
#define M14_SHARED_H
#include <stdlib.h>

#define MAX_15Bit_NUM 16382
#define MIN_15Bit_NUM -16383
#define MAX_12Bit_NUM 2047
#define MIN_12Bit_NUM -2048

#define LINE_LENGTH 80
#define MAX_MACRO_NAME 31
#define MAX_MACROS 20
#define MAX_SYMBOL_NAME 31

#define MACRO_START_WORD "macr"
#define MACRO_END_WORD "endmacr"
#define MACRO_END_LEN 7
#define MACRO_START_LEN 4
#define MAX_DIRECTIVE_NAME 8
#define IC_SHIFT 100


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
#define ERR_MACRO_NAME_OP_DIRECT_SYMBOL "Macro name is a opcode, a Directive or a Symbol"

/* symbol table related , symbol.c */
#define ERR_DUPLICATE_SYMBOL_NAME  "DUPLICATE SYMBOL NAME"
#define ERR_FAIL_CREATE_SYMBOL_TBL "Symbol Table Creation Failed"
#define ERR_FAIL_CREATE_SYMBOL "Symbol  Creating Symbol Node Failed"
#define ERR_SYMBOL_NOT_FOUND "Symbol Not Found" /*non critial*/



/* parser: op code  directive related errors*/
#define ERR_OP_CODE_RECOGNITION "OP CODE NOT RECOGNIZED"
#define ERR_OP_CODE_FAILED_STRUCTURE "OP Code structure Unrecognized or Not Legal"
#define ERR_OP_CODE_REGISTRY_ILLEGAL "One or both OP Code register isn't legal"
#define ERR_REGISTRY_ILLEGAL "Failed to Recognize Registry"

#define ERR_LINE_UNRECOGNIZED "Line Not Recognized as OP_CODE or DIRECTIVE"
/* directive related errors*/
#define ERR_DIRECTIVE_RECOGNITION "Directive Not Recognized"
#define ERR_FAILED_TO_CONVERT_NUMBER "Failed to convert Number"
#define ERR_DATA_DIRECTIVE_NUMBER "Data Directive Number or Numbers Invalid"
#define ERR_STRING_QUOTATION_MISSING "String Quotations missing or characters invalid"
#define ERR_STRING_INVALID  "String Line not recognized"
#define ERR_EXTERN_ENTRY_ILLEGAL "Illegal Extern or Entry Line"
#define ERR_EXTERN_ENTRY_SYMBOL "Symbol at the start of Extern or Entry Line"
#define WAR_EXTERN_ENTRY_SYMBOL "Warning: Symbol is Extern or Entry"

/*word_table*/
/*pass one and two*/
#define ERR_WORD_TABLE_ALLOCATION "Word Table Creation or Allocation Failed"
#define ERR_GENERAL_FIRST_PASS_ERROR "General First Pass Error"
#define WAR_MEMORY_NOT_CONFIGURED "Warning Memory Not Configured"
#define ERR_MEMORY_NOT_CONFIGURED "Error Symbol Memory Not Configured"


typedef enum{ AS ,MAC , MACL , SYM , PARS ,FIRST ,SECOND ,UTIL }file_t;


typedef enum {
	DATA,
	STRING,
	ENTRY,
	EXTERN,
}directive_cmd_t;
/*pass1 and pass2*/
typedef enum {
	mov, cmp, add, sub, lea, clr, not, inc, dec, jmp,
	bne, red, prn, jsr, rts, stop
} op_code_t;

/*macros*/
typedef enum{MACRO_START, MACRO_END , MACRO_EXPAND , LINE_INSIDE , LINE_OUTSIDE, EMPTY_LINE , MACRO_ERROR }MACRO_STATE_T;
/*utils */
typedef enum{ALPHA,ALPHANUM, ALPHANUM_COMBINED}check_legal_name;


/*extern entry*/
typedef enum {
	_EX, _EN ,_NO
}EXT_T;


typedef enum {E, R,	A } ARE_T;

typedef  enum{NO, YES }update_address_t;
typedef enum {CONVERT  , CHECK} convert_func_t;
typedef enum{  MIDDLE, HEAD}symbol_loci_t;


typedef enum{DESTINATION , SOURCE}operand_t;

/*CRIT -terminates NON_CRIT doesn't*/
typedef enum{CRIT,NON_CRIT}err_type_t;

/*meant to decipher between reg behvior*/

typedef enum { _IMMEDIATE = 0 , _DIRECT  ,_INDIRECT  , _REGULAR  , _ERROR   , _TBD /*5*/ }type_of_register_t;
typedef enum{ _INSTRUCTION , _DATA}memory_t;

#endif
