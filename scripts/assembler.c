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


FILE *fptr_before;
FILE *fptr_after;
int isError = 0;

int main(int argc, char *argv[]) {

	macro_table_t *mac_tbl = NULL;
	symbol_table_t *sym_tbl = NULL;
	word_table_t *wordTable = NULL;
	word_table_t *dataTable = NULL ;

	mac_tbl =  initMacroTable(mac_tbl);
	sym_tbl = init_symbol_table(sym_tbl);
	wordTable = initTable(wordTable,100);
	dataTable = initTable(dataTable,0);



	manage_files(argc, argv, mac_tbl, sym_tbl,wordTable, dataTable);


	fclose(fptr_before);
	fclose(fptr_after);

 return 0;
}

void manage_files(int _argc, char **_argv, macro_table_t *macro_tbl, symbol_table_t *sym_tbl , word_table_t *wordTable,
                  word_table_t *dataTable) {
	int idx;
	int num_files = _argc;
	char buffer[LINE_LENGTH];



	if (_argc == 1) {
		report_error(ERR_NO_FILES, line_count, CRIT);
		return;
	}
	fptr_after = initDestinationPointer(fptr_after, "out.txt", "a+");

	for (idx = 1; idx < num_files; ++idx) {
		if (isError) {
			break;
		}
		fptr_before = initSourceFiles(_argc, _argv, fptr_before, idx);
		parse(sym_tbl, wordTable, dataTable);


		/*	read_preprocessor(macro_tbl, sym_tbl);*/
	/*	rewind(fptr_after);*/



		/*todo last command should be print the final version */
	}
	freeAllTables(macro_tbl, sym_tbl, wordTable, dataTable);
}

/*index signifies argv index*/
/*TODO: add option to goto next file once a file fails to open*/
FILE *initSourceFiles(int _argc, char **_argv, FILE *fptr, int index) {

	char line[LINE_LENGTH];
	char filename[64];
	int argv_len = 0;
	if (_argc > 1) {/*argc  must always be subtracted by one*/
		argv_len = nonNullTerminatedLength(_argv[index]);

		strcpy(filename, PATH_BASE);/*TODO no need in linux*/
		strncat(filename, _argv[index], argv_len); /*TODO arg_c iteration tbd*/
		printf("%s\n", filename);
		if (!(fptr = fopen(filename, "r"))) {
			report_error(ERR_FILE_BEFORE, line_count, CRIT);
			exit(0);
		} else return fptr;

	} else {
		report_error(ERR_NO_FILES, line_count, CRIT);
		return NULL;
	}
}

FILE *initDestinationPointer(FILE *fptr, char *filename, char mode[]) {
	// Construct the file path by prepending the parent directory
	char fname[64];
	strcpy(fname, PATH_BASE);//*TODO no need in linux*//
	strcat(fname, filename);
	printf("%s\n", fname);

	/* Attempt to open the file for writing*/
	if (!(fptr = fopen(fname, mode))) {
		report_error(ERR_FILE_AFTER, line_count, CRIT);
		exit(0);
	}
	/* Return the file pointer if the file was successfully created*/
	return fptr;
}

void move_one_directory_up(char *path) {
	/* Find the last occurrence of the directory separator*/
	char *last_separator = strrchr(path, '\\'); /* TODO: change when in linux */
	if (last_separator != NULL) {
		*last_separator = '\0'; // Terminate the string to remove the last directory
	}
}


void report_error(char *err, int line_count, err_type_t type) {


	printf("%s at line %lu\n", err, line_count);
	if (type == CRIT) {
		printf("Critical Error, Exiting\n");
		printf("terminating and Freeing Allocation\n");
		isError = 1;
	} else (printf("%s\n", "NON CRITICAL"));

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

	while (current!= NULL) {
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
	for (idx = 0; idx < table->size; idx++) {
		freeMacroLList(table->slot[idx]);
	}
}

void freeMacroLList(macro_node_t *head) {
	macro_node_t *current = head;
	macro_node_t *next;

	while (current != NULL) {
		next = current->macro_next;
		free(current);
		current = next;
	}

}

void freeAllTables(macro_table_t *macroTable, symbol_table_t *symbolTable, word_table_t *wordTable, word_table_t *dataTable) {
	freeMacroTable(macroTable);
	freeSymbolTable(symbolTable);
	freeWordTable(wordTable);
	freeWordTable(dataTable);
}

