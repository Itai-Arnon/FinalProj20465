

#ifndef M14_SHARED_H
#define M14_SHARED_H

/**
 * reports of macro_error as well as which line
 */
void report_error(char* r);
/*check if name is only alpha or alpha ending numerals*/
/*uses the ENUM CHECK_LEGAL_NAME*/
int checkLegalName(char* str , int);


#endif
