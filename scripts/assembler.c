#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>

#include "headers/macro.h"
#include "headers/macro_list.h"
#include "headers/shared.h"
#include "headers/global_vars.h"
#include "headers/utils.h"
#include "headers/symbols.h"
#include "headers/parser.h"
#include "headers/first_pass.h"
#include "headers/assembler.h"
#include "headers/second_pass.h"


FILE *fptr_before;
FILE *fptr_after;
char *current_filename;
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

	char f[20] = "matil";
	char *str;

	str = addExtension(f,"symbolic");
	printf("str: %s\n",str);
	/*manage_files(argc, argv, mac_tbl, sym_tbl, wordTable, dataTable);


	fclose(fptr_before);*/
	fclose(fptr_after);

	return 0;
}

void manage_files(int _argc, char **_argv, macro_table_t *macro_tbl, symbol_table_t *sym_tbl, word_table_t *wordTable,
                  word_table_t *dataTable) {
	int idx;
	int num_files = _argc;
	char buffer[LINE_LENGTH];


	if (_argc == 1) {
		report_error(ERR_NO_FILES, line_count, AS, CRIT, 0);
		return;
	}
	fptr_after = initDestinationPointer(fptr_after, "out.txt", "a+" ,0);

	for (idx = 1; idx < num_files; ++idx) {
		if (isError) {
			break;
		}
		fptr_before = initSourceFiles(_argc, _argv, fptr_before, idx,0 );
		read_preprocessor(macro_tbl, sym_tbl);
		rewind(fptr_after);
		parse( sym_tbl, wordTable, dataTable , current_filename);/*after parse comes first_pass and second_pass*/





		/*todo last command should be print the final version */
	}
	checkSymbolsUnique(macro_tbl, sym_tbl);/*only done once the 1st pass is done*/
	freeAllTables(macro_tbl, sym_tbl, wordTable, dataTable);
}

/*index signifies argv index*/
/*TODO: add option to goto next file once a file fails to open*/
FILE *initSourceFiles(int _argc, char **_argv, FILE *fptr, int index , int os) {

	char line[LINE_LENGTH];
	char filename[64]={'\0'};
	int argv_len = 0;
	if (_argc > 1) {/*argc  must always be subtracted by one*/
		argv_len = nonNullTerminatedLength(_argv[index]);

	switch(os) {
		case 0:/*windows*/
			strcpy(filename, PATH_BASE);
			strncat(filename, _argv[index], argv_len);
			printf("%s\n", filename);
			break;
		case 1: /*linux*/
			strncat(filename, _argv[index], argv_len);
			printf("%s\n", filename);
			break;
		default:
			report_error(ERR_FILE_BEFORE, line_count, AS, CRIT, 0);
			return NULL;
		}
			if (!(fptr = fopen(filename, "r"))) {
			report_error(ERR_FILE_BEFORE, line_count, AS, CRIT, 0);
			exit(0);
		} else return fptr;

	} else {
		report_error(ERR_NO_FILES, line_count, AS, CRIT, 0);
		return NULL;
	}
}

FILE *initDestinationPointer(FILE *fptr, char *filename, char mode[] , int os) {
	// Construct the file path by prepending the parent directory
	char fname[64];

	switch(os) {
		case 0:/*windows*/
		strcpy(fname, PATH_BASE);//*TODO no need in linux*//
		strcat(fname, filename);
		printf("%s\n", fname);
		case 1:			/*linux*/
		  break;
		default:
			report_error(ERR_FILE_AFTER, line_count, AS, CRIT, 0);
			break;
	}
	/* Attempt to open the file for writing*/
	if (!(fptr = fopen(fname, mode))) {
		report_error(ERR_FILE_AFTER, line_count, AS, CRIT, 0);
		return NULL;
	}
	/* Return the file pointer if the file was successfully created*/
	return fptr;
}

char *addExtension(char file_name[], char *ext) {

	char *str = strrchr(file_name, '.');
	if(str == NULL)
		strcat(file_name, ext);
	else {
		*(str + 1) = '\0';
		strcat(file_name,ext);
	}
	return file_name;

}

void move_one_directory_up(char *path) {
	/* Find the last occurrence of the directory separator*/
	char *last_separator = strrchr(path, '\\'); /* TODO: change when in linux */
	if (last_separator != NULL) {
		*last_separator = '\0'; // Terminate the string to remove the last directory
	}
}

void setFilename(char *file) {
	strcpy(current_filename, file);
}

void report_error(char *err, int line_count, file_t fenum, err_type_t type, int IC_ADDRESS) {
	char *str = calloc(30, sizeof(char));
	char fname[8][16] = {"assembler.c", "macro.c", "macro_list.c", "symbols.c",
	                     "parser.c", "first_pass.c", "second_pass.c", "utils.c"};

	/* case there is a warning with symbols in wordTable*/
	if ((fenum == FIRST || fenum == SECOND) && IC_ADDRESS > 0 && type == NON_CRIT)
		printf("%s at WordTable Address %d  line %lu || At: %s\n", WAR_MEMORY_NOT_CONFIGURED, IC_ADDRESS, line_count,
		       fname[fenum]);
		/* case there is an error with symbols in wordTable*/
	else if ((fenum == FIRST || fenum == SECOND) && IC_ADDRESS > 0 && type == CRIT )
			printf("%s at WordTable Address %d  line %lu || At: %s\n", ERR_MEMORY_NOT_CONFIGURED, IC_ADDRESS, line_count,
			       fname[fenum]);
	else if (type == CRIT) {
		printf("%s at line %lu || At: %s\n", err, line_count, fname[fenum]);
		printf("Critical Error,  terminating and Freeing Allocation\n");
		isError = 1;
	} else
		printf(" NON CRITICAL %s at line %lu || At: %s\n", err, line_count, fname[fenum]);


}

void freeWordTable(word_table_t *wordTable);

void freeWordTable(word_table_t *table) {
	if (table == NULL) {
		return;
	}
	free(table->lines);
	free(table);
}

/* Frees the memory allocated for the symbol table */
void freeSymbolTable(symbol_table_t *symbolTable) {
	symbol_t *current;
	symbol_t *next;

	if (symbolTable == NULL) {
		return;
	}
	current = symbolTable->symbol_List;

	while (current != NULL) {
		next = current->next_sym;
		free(current);
		current = next;
	}
	free(symbolTable);
}

void freeMacroTable(macro_table_t *table) {
	int idx = 0;
	if (table == NULL) {
		return;
	}
	free(table->slot);
	free(table);
}


void freeAllTables(macro_table_t *macroTable, symbol_table_t *symbolTable, word_table_t *wordTable,
                   word_table_t *dataTable) {
	freeMacroTable(macroTable);
	freeSymbolTable(symbolTable);
	freeWordTable(wordTable);
	freeWordTable(dataTable);
}

