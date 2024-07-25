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


int main(int argc, char *argv[]) {
	macro_table_t *tbl = NULL;
	tbl = initTable(tbl);
	manage_files(argc ,argv, tbl);


	fclose(fptr_before);
	fclose(fptr_after);
	/*free(tbl);*/


}

void manage_files(int _argc ,char ** _argv, macro_table_t* tbl){
	int idx;
	int num_files =_argc;
	if(_argc == 1) {
		report_error(ERR_NO_FILES,line_count);
		return;
	}
	fptr_after = initDestinationPointer(fptr_after, "out.txt");
	for ( idx = 1; idx < num_files ; ++idx)
	{
		fptr_before = initSourceFiles(_argc, _argv, fptr_before, idx);
		read_preprocessor(tbl);
	}
}