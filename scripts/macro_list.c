#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "headers/macro_list.h"
#include "headers/shared.h"
#include "headers/global_vars.h"
#include "headers/assembler.h"


macro_table_t *initMacroTable(macro_table_t *tbl) {
	int i = 0;
	if (!(tbl = calloc(1, sizeof(macro_table_t)))) {
		report_error(ERR_MACRO_TABLE_GENERAL_ERROR, line_count , MACL , CRIT ,0);    /*critical error*/
		return NULL;
	}

	tbl->isEmpty = 1;
	tbl->isMacroOpen = 0;
	tbl->amount = 0;
	tbl->size = 0;



	if (!(tbl->slot = calloc(1, sizeof(macro_node_t)))) {
		report_error(ERR_MACRO_TABLE_GENERAL_ERROR, line_count ,MACL , CRIT ,0);    /*critical error*/
		return NULL;
	}

	tbl->slot->macro_lock = 0;


	return tbl;
}


macro_node_t *constructMacroNode(macro_table_t *tbl, char *macro_name, char *line, int len) {

	if (tbl == NULL)
		return NULL;



	if (!(tbl->slot = realloc(tbl->slot, (tbl->size) * sizeof(macro_node_t)))) {
		report_error(ERR_MACRO_NODE_CREATION_FAILED, line_count ,MACL , CRIT ,0);    /*critical error*/
		return NULL;
	}

	/*problem with conflicting address macr_name and line*/
	if (macro_name[0] != '\0' || line[0] != '\0') {
		strcpy(tbl->slot[tbl->size - 1].macro_name, macro_name);
		strcpy(tbl->slot[tbl->size - 1].macro_line, line);
		tbl->slot[tbl->size - 1].macro_lock = 0;
		tbl->slot[tbl->size - 1].len = len;
		return &tbl->slot[tbl->size - 1];
	}
	return NULL;
}


int loadMacroTable(macro_table_t *tbl, char *macro_name, char *line) {
	macro_node_t *temp = NULL;


	if (!(temp = constructMacroNode(tbl, macro_name, line, 1))) {
		report_error(ERR_MACRO_NODE_CREATION_FAILED, line_count ,MACL , CRIT ,0);    /*critical error*/
		return 0;
	}

	tbl->amount += 1;
	tbl->isEmpty = 0;
	return 1;
}

/*expands macro writing to  file*/
int expandMacro(macro_table_t *tbl, char *macro_name) {
	int i = 0;
	if (tbl->isEmpty == 1) return 0;

	for (i = 0; i < tbl->size; i++) {
		if (strcmp(tbl->slot[i].macro_name, macro_name) == 0) {
			printf("%s\n", tbl->slot[i].macro_line);
			fprintf(fptr_after, "%s", tbl->slot[i].macro_line);
			return 1;
		}
		i++;
	}
	return 0;
}


void MacroName(macro_node_t *head) {
	if (head != NULL) {
		printf("%s\n", head->macro_name);
	} else report_error(ERR_MACRO_NOT_FOUND, line_count,MACL, NON_CRIT , 0);
}

int getLengthMacro(macro_table_t *tbl, char *macro_name) {
	int i = 0;
	int length = 0;
	for (i = 0; i < tbl->size; i++) {
		if ((strcmp(macro_name, tbl->slot[i].macro_name)) == 0) {
			tbl->slot[i].len = length++;
		}

	}
	return length;
}

macro_node_t *retMacro(macro_table_t *tbl, char *macro_name) {
	int i = 0;
	if (tbl->isEmpty == 1) return NULL;

	for (i = 0; i < tbl->size; i++) {
		if ((strcmp(macro_name, tbl->slot[i].macro_name)) == 0) {

			return &(tbl->slot[i]);
		}
	}
/*not found*/
	return NULL;


}

int retSlot(macro_table_t *tbl, char *macro_name) {
	int i = 0;
	if (tbl->isEmpty == 1) return 0;

	for (i = 0; i < tbl->size; i++) {
		if ((strcmp(macro_name, tbl->slot[i].macro_name)) == 0) {
			/*if macro already exist returns err*/
			return i;
		}
	}
	/*not found*/
	return -1;


}


/* Checks if a given name exists in any slot of the macro table  true 1 , false 0*/
int dupNameExistsInTable(macro_table_t *tbl, char *macro_name, int locked ) {
	/*locked = 1  checks if it exists and  locked | 0 exist but not locked */
	int i = 0;
	if (tbl->isEmpty == 1) return 0;

	for (i = 0; i < tbl->size; ++i) {

			switch(locked) {
				case 1:
					/* Compare the macro_name of the first node in this slot with the given name*/
					if (strcmp(tbl->slot[i].macro_name, macro_name) == 0 && tbl->slot[i].macro_lock == 1)
						return 1;  /*Name found*/
				case 2:
					if (strcmp(tbl->slot[i].macro_name, macro_name) == 0 && tbl->slot[i].macro_lock == 0)
						return 1;  /*Name found*/
			}
	}
	return 0;/* Name not found*/
}

void macro_lock(macro_table_t *tbl, char *macro_name) {
	int slot_index;
	int i = 0, len = 0;
	if (tbl->isEmpty == 1) return;

	slot_index = retSlot(tbl, macro_name);
	len = getLengthMacro(tbl, macro_name);
	for (i = slot_index; i < len; i++)
		tbl->slot[i].macro_lock = 1;


}