#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>
#include "macro.h"
#include "linkedlist.h"
#include "shared.h"
#include "global_vars.h"
#include "utils.h"
#include "assembler.h"


FILE *fptr_before;
FILE *fptr_after;
int line_count;

int main(int argc, char *argv[]) {
	macro_table_t *tbl = NULL;
	tbl = initTable(tbl);
	manage_files(argc ,argv, tbl);




	/*free(tbl);*/
}

void manage_files(int num_files ,char ** file_list, macro_table_t* tbl){
	int idx;
	for ( idx = 1; idx < num_files ; ++idx) {
		fptr_before = initSourceFiles(_argc, _argv, fptr_before, 1);

	}
	fptr_after = initDestinationPointer(fptr_after, "out.txt");


	read_preprocessor(argc, argv, tbl);
}