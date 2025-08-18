#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>
#include <errno.h>

#include "include/macro.h"
#include "include/macro_list.h"
#include "include/shared.h"
#include "include/global_vars.h"
#include "include/utils.h"
#include "include/symbols.h"
#include "include/parser.h"
#include "include/first_pass.h"
#include "include/assembler.h"
#include "include/second_pass.h"


FILE *fptr_before;
FILE *fptr_after;
/*report error uses this to identify in whihc file an error occured*/


int isError = 0;

int main(int argc, char *argv[]) {

	macro_table_t *mac_tbl = NULL;
	symbol_table_t *sym_tbl = NULL;
	word_table_t *wordTable = NULL;
	word_table_t *dataTable = NULL;

	mac_tbl = initMacroTable(mac_tbl);
	sym_tbl = init_symbol_table(sym_tbl);
	wordTable = initTable(wordTable, 0);
	dataTable = initTable(dataTable, 0);
/*OS win:0 , linux:1*/
	manage_files(argc, argv, mac_tbl, sym_tbl, wordTable, dataTable , 0 ,"txt");


	fclose(fptr_before);
	fclose(fptr_after);

	return 0;
}

void manage_files(int _argc, char **_argv, macro_table_t *macro_tbl, symbol_table_t *sym_tbl, word_table_t *wordTable,
                  word_table_t *dataTable , int os , char ext[]) {
	int idx = 0;

	if (_argc == 1) {
		report_error(ERR_NO_FILES, line_count, AS, CRIT, 0);
		return;
	}
	/*argc  must always be subtracted by one*/
	/*OS determines Operating System */
	for (idx = 1; idx < _argc; ++idx) {
		printf("%d\n", idx);
		fptr_before = initSourcePtr(_argv, fptr_before, idx,"r", os);
		fptr_after = initDestinationPtr(_argv, fptr_after, idx, "w+",os ,ext);
		read_preprocessor(macro_tbl, sym_tbl);
		rewind(fptr_after);
		parse(sym_tbl, wordTable, dataTable,_argv[idx]);
		if (isError) {
			report_error("ERR_FILE_GENERAL",line_count,AS,CRIT,0);
			isError = 0;
			continue;
		}

	}

}

/*TODO: add option to goto next file once a file fails to open*/
/*fptr- global file ptr , index - argv idx , mode[]- fopen mode , os - 0:win/1:linux*/
FILE *initSourcePtr(char **_argv, FILE *fptr, int index,char mode[] , int os) {

	
	char *filename = calloc(64, sizeof(char));
	int argv_len = nonNullTerminatedLength(_argv[index]);


	switch (os)
	{
	case 0:/*windows*/
		strcpy(filename, PATH_BASE);
		strncat(filename, _argv[index], argv_len);
		printf("Source File %s\n", filename);
		/*report_error(ERR_FILE_BEFORE, __LINE__, AS, CRIT, 0); */
		break;

		case 1: /*linux*/
			strncat(filename, _argv[index], argv_len);
			printf("%s\n", filename);
			printf("Source File %s\n", filename);
			break;
		default:
			report_error(ERR_FILE_BEFORE, line_count, AS, CRIT, 0);
			return NULL;
	}


	if ((fptr = fopen(filename, mode))==NULL)
	{
		printf("Error Code:%d\n" , errno);
		printf("Error Description:%s\n" , strerror(errno));
		return NULL;
	}
	 	return fptr;

}

FILE *initDestinationPtr(char **_argv, FILE *fptr, int index, char mode[], int os ,char ext[] ) {
	/* Construct the file path by prepending the parent directory*/
	char *fname = calloc(64, sizeof(char));
	/*argv members are not null terminated*/
	int argv_len = nonNullTerminatedLength(_argv[index]);
	switch (os) {
		case 0:/*windows*/
			strcpy(fname, PATH_BASE);
			/*file extension in windows cannot be .am  extension defines as txt */
			strncat(fname, _argv[index], argv_len);
			fname = replaceExtension(fname, ext);
			printf("Destination File %s\n", fname);
			break;
		case 1:/*linux*/
			strncat(fname, _argv[index], argv_len);/*copies and add the .am extension*/
			fname = replaceExtension(fname, ext);
			printf("Destination File %s\n", fname);
			break;
		default:
			report_error(ERR_FILE_AFTER, line_count, AS, CRIT, 0);
			break;
	}
	/* Attempt to open the file for writing*/
	if ((fptr != fopen(fname, mode))) {
		report_error(ERR_FILE_AFTER, line_count, AS, CRIT, 0);
		printf("Error Code:%d\t" , errno);
		printf("Output file error description:%s\n" , strerror(errno));
		return NULL;
	}
	/* Return the file pointer if the file was successfully created*/
	return fptr;
}

char *replaceExtension(char file_name[], char *ext) {
	/*check if period alread exists*/
	char *str = strrchr(file_name, '.');

	if (str == NULL) {
		strcat(file_name, ".");
		strcat(file_name, ext);
	}
	else if ((str  - file_name ) > 0) {
		strcat(file_name, ".");
		strcat(file_name, ext);
	}
	else{
		*str = '\0';
		strcat(file_name, ext);
	}
	return file_name;
}

void move_one_directory_up(char *path) {
	/* Find the last occurrence of the directory separator*/
	char *last_separator = strrchr(path, '\\'); /* TODO: change when in linux */
	if (last_separator != NULL) {
		*last_separator = '\0'; /*Terminate the string to remove the last directory*/
	}
}


void report_error(char *err, int line_count, file_t fenum, err_type_t type, int IC_ADDRESS ) {
	char fname[8][16] = {"assembler.c", "macro.c", "macro_list.c", "symbols.c",
	                     "parser.c", "first_pass.c", "second_pass.c", "utils.c"};

	/* case there is a warning with symbols in wordTable*/

	char *critic = (char*) calloc( ERR_MAX_LENGTH + 1, sizeof(char));
	char *non_critic = (char*) calloc( ERR_MAX_LENGTH + 1, sizeof(char));
	if (critic == NULL || non_critic == NULL)
	{
		printf("err");
		exit(0);
	}
	printf("sucess");
	strcpy(critic,BASE_CRIT_ERR);
	strcpy(non_critic,BASE_NON_CRIT_ERR);

	switch(fenum){
		case AS:
		case UTIL:
		case MACL:
		case SYM:
		case SECOND:
			sprintf(critic, "  Error Found in file %s\t line number %d", fname[fenum] , line_count);
			sprintf(non_critic, " Error Found in file %s\t line number %d", fname[fenum] , line_count );
			break;

		default:
			break;


	}


	if (type == CRIT) {
		printf("%s at line %d || At: %s\n", err, line_count, fname[fenum]);
		printf("%s\n",critic);
		isError = 1;
	} else
		printf(" NON CRITICAL  %s at line %d || At: %s\n", err, line_count, fname[fenum]);
		printf("%s\n",non_critic);

}


void freeWordTable(word_table_t *table) {
	if (table == NULL) {
		return;
	}
	free(table->lines);
	free(table);
	table = NULL;
}

/* Frees the memory allocated for the symbol table */
void freeSymbolTable(symbol_table_t *symbolTable) {
	symbol_t *current = NULL;
	symbol_t *prev = NULL;

	if (symbolTable == NULL || symbolTable->size == 0) {
		free(symbolTable);
		symbolTable = NULL;
		return;
	}

	current = symbolTable->symbol_List;

	while (current != NULL) {
		prev = current;
		current = current->next_sym;
		free(prev);
	}
	free(symbolTable);
	symbolTable = NULL;
}

void freeMacroTable(macro_table_t *table) {
	if (table == NULL) {
		return;
	}
	free(table->slot);
	free(table);
	table = NULL;
}

