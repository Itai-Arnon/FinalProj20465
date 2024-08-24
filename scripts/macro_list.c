#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "headers/macro_list.h"
#include "headers/shared.h"
#include "headers/global_vars.h"
#include "headers/assembler.h"


macro_table_t *initMacroTable(macro_table_t *tbl) {
	int i = 0;
	if (!(tbl = (macro_table_t *) calloc(1, sizeof(macro_table_t)))) {
		report_error(ERR_MACRO_TABLE_GENERAL_ERROR, line_count, MACL, CRIT, 0);    /*critical error*/
		return NULL;
	}

	if (!(tbl->slot = (macro_node_t *) calloc(1, sizeof(macro_node_t)))) {
		report_error(ERR_MACRO_TABLE_GENERAL_ERROR, line_count, MACL, CRIT, 0);    /*critical error*/
		return NULL;
	}

	tbl->size = 0;
	tbl->isEmpty = 1;
	tbl->isMacroOpen = 0;


	tbl->slot->macro_lock = 0;
	tbl->slot->index = tbl->size;
	memset(tbl->slot->macro_line , '\0', SET_BUFFER_LENGTH * sizeof(char));
	memset(tbl->slot->macro_name , '\0',MAX_MACRO_NAME * sizeof(char));


	return tbl;
}


macro_node_t *constructMacroNode(macro_table_t *tbl, char *macro_name, char *line) {
macro_node_t *node = NULL;
int LEN = strlen(macro_name);
if(macro_name[LEN-1]  != '\0') LEN--;

if (tbl == NULL)
		return NULL;

	tbl->size+5;

	if (!(node = (macro_node_t *) realloc(tbl->slot, (tbl->size) * sizeof(macro_node_t)))) {
		report_error(ERR_MACRO_NODE_CREATION_FAILED, line_count, MACL, CRIT, 0);    /*critical error*/
		tbl->size--;
		return NULL;
	}

	  tbl->slot = node;


	tbl->slot[tbl->size - 1].index = tbl->size ;
	snprintf(tbl->slot[tbl->size - 1].macro_line, LINE_LENGTH, "%s",line  );
	snprintf(tbl->slot[tbl->size - 1].macro_name, LEN+1, "%s", macro_name );
	tbl->slot[tbl->size - 1].macro_name[LEN] ='\0';
	tbl->slot[tbl->size - 1].macro_lock = 0;

	return &(tbl->slot[tbl->size - 1]);
}


int loadMacroTable(macro_table_t *tbl, char *macro_name, char *line) {
	/*increase in size , update of index are done inside constrrct node*/

	tbl->isEmpty = 0;

	if (macro_name[0] == '\0') {
		report_error(ERR_MACRO_NAME, line_count, MACL, CRIT, 0);
		return 0;
	}
	if (!(tbl->slot = constructMacroNode(tbl, macro_name, line))) {
		report_error(ERR_MACRO_NODE_CREATION_FAILED, line_count, MACL, CRIT, 0);    /*critical error*/
		return 0;
	}
	/*at init equal 1*/


	return 1;
}

/*expands macro writing to  file*/
void expandMacro(macro_table_t *tbl, char *macro_name) {

	int i, slot = 0;
	int LEN = 0;

	if (macro_name[0] == '\0')
		report_error(ERR_MACRO_NAME_RETRIEVE, line_count, MACL, CRIT, 0);


	if (tbl == NULL || tbl->size == 0) {
		printf("Macro Table Empty");
		return;
	}

	slot = retSlot(tbl, macro_name);
	LEN = getMacroLength(tbl, macro_name);



	for (i = slot ; i < LEN; i++) {
		fprintf(fptr_after, "%s\n", tbl->slot[i].macro_line);
	}

	return;
}


void MacroName(macro_node_t *head) {
	if (head != NULL) {
		printf("%s\n", head->macro_name);
	} else report_error(ERR_MACRO_NOT_FOUND, line_count, MACL, NON_CRIT, 0);
}

int getMacroLength(macro_table_t *tbl, char *macro_name) {
	int i = 0, LEN = 0;
	int length = 0;
	for (i = 0; i < tbl->size; i++) {
		LEN = strlen(macro_name);
		if ((strncmp(macro_name, tbl->slot[i].macro_name, LEN)) == 0) {
			length++;
		}
	}

	return length;
}

macro_node_t *retMacro(macro_table_t *tbl, char *macro_name) {
	int i = 0, LEN = 0;

	if (tbl == NULL || tbl->size == 0) return NULL;


	for (i = 0; i < tbl->size; i++) {
		LEN = strlen(macro_name);
		if ((strncmp(macro_name, tbl->slot[i].macro_name, LEN)) == 0) {
			return &(tbl->slot[i]);
		}
	}
/*not found*/
	return NULL;

}

/*return the first index of macro by macro name*/
int retSlot(macro_table_t *tbl, char *macro_name) {
	int i = 0, LEN = 0;
	if (tbl == NULL || tbl->size == 0) return 0;

	LEN = strlen(macro_name);
	for (i = 0; i < tbl->size; i++) {
		if ((strncmp(macro_name, tbl->slot[i].macro_name, LEN)) == 0) {
			/*if macro already exist returns err*/
			return i;
		}
	}
	/*not found*/
	return 0;
}


/* Checks if a given name exists  |true 1 | false 0*/

int dupNameExistsInTable(macro_table_t *tbl, char *macro_name) {
	int i = 0, LEN = 0;
	if (tbl == NULL || tbl->size == 0) return 0;

	LEN = strlen(macro_name);
	for (i = 0; i < tbl->size; ++i) {
		if (strncmp(tbl->slot[i].macro_name, macro_name, LEN) == 0)
			return 1;  /*Name found*/
	}
	return 0;/* Name not found*/
}

/*locks the all macro lines  from overriding*/
void macro_lock(macro_table_t *tbl, char *macro_name) {
	int slot_index;
	int i = 0, len = 0;
	if (tbl == NULL || tbl->isEmpty == 1) return;

	slot_index = retSlot(tbl, macro_name);
	len = getMacroLength(tbl, macro_name);
	for (i = slot_index; i < len; i++) {
		tbl->slot[i].macro_lock = 1;
	}
}


void printAllMacros(macro_table_t *tbl) {
	int i = 0;
	if (tbl == NULL || tbl->isEmpty == 1) {
		printf("Macro Table is empty or not initialized.\n");
		return;
	}

	for (i = 0; i < tbl->size; i++) {
		printf("\n");
		printf("Macro Name: %s\t", tbl->slot[i].macro_name);
		printf("Macro Line: %s", tbl->slot[i].macro_line);
	}
}


