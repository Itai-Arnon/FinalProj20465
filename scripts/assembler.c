#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>

#include "headers/macro.h"
#include "headers/macro_list.h"
#include "headers/shared.h"
#include "headers/global_vars.h"
#include "headers/utils.h"
#include "headers/assembler.h"
#include "headers/symbols.h"
#include "headers/parser.h"
#include "headers/error.h"
#include "headers/first_pass.h"


FILE *fptr_before;
FILE *fptr_after;


int main(int argc, char *argv[]) {
	macro_table_t *mac_tbl = NULL;
	symbol_table_t *sym_tbl = NULL;
	 word_table_t *wordTable;
	 word_table_t *dataTable;
	mac_tbl = initMacroTable(mac_tbl);
	sym_tbl = init_symbol_table(sym_tbl);
	manage_files(argc, argv, mac_tbl, sym_tbl,wordTable, dataTable);


	fclose(fptr_before);
	fclose(fptr_after);


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
		fptr_before = initSourceFiles(_argc, _argv, fptr_before, idx);
		parse(sym_tbl);
		first_pass(sym_tbl, wordTable ,dataTable);

		/*	read_preprocessor(macro_tbl, sym_tbl);*/
		/*rewind(fptr_before);*/



		/*todo last command should be print the final version */
	}
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

