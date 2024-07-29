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


FILE *fptr_before;
FILE *fptr_after;


int main(int argc, char *argv[]) {
	macro_table_t *mac_tbl = NULL;
	symbol_table_t *sym_tbl = NULL;
	mac_tbl = initMacroTable(mac_tbl);
	sym_tbl = init_symbol_table(sym_tbl);
	manage_files(argc ,argv, mac_tbl,sym_tbl);


	fclose(fptr_before);
	fclose(fptr_after);
	/*free(tbl);*/


}

void manage_files(int _argc ,char ** _argv, macro_table_t* macro_tbl, symbol_table_t *sym_tbl){
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
		 collect_symbol_names(sym_tbl);
	}
	for ( idx = 1; idx < num_files ; ++idx)
	{
		fptr_before = initSourceFiles(_argc, _argv, fptr_before, idx);
		read_preprocessor(macro_tbl,sym_tbl);
	}
}

