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


	tbl->slot = NULL;
	tbl->size = 0;
	tbl->isEmpty = 1;
	tbl->isMacroOpen = 0;


	return tbl;
}


macro_node_t *constructMacroNode(macro_table_t *tbl, char *macro_name, char *line) {
	macro_node_t *node = NULL;   /*macro node*/

	int LEN = strlen(macro_name);


	if (tbl == NULL)
		return NULL;

	tbl->size++;

	if (!(node = (macro_node_t *) calloc(1, sizeof(macro_node_t)))) {
		report_error(ERR_MACRO_NODE_CREATION_FAILED, line_count, MACL, CRIT, 0);    /*critical error*/
		tbl->size--;
		return NULL;
	}


	snprintf(node->macro_line, LINE_LENGTH, "%s\0", line);
	snprintf(node->macro_name, LEN + 1, "%s\0", macro_name);


	return node;
}


int loadMacroTable(macro_table_t *tbl, char *macro_name, char *line) {

	macro_node_t *node = NULL, *head = NULL;

	tbl->isEmpty = 0;

	if (macro_name[0] == '\0') {
		report_error(ERR_MACRO_NAME, line_count, MACL, CRIT, 0);
		return 0;
	}

	if (!(node = constructMacroNode(tbl, macro_name, line))) {
		report_error(ERR_MACRO_NODE_CREATION_FAILED, line_count, MACL, CRIT, 0);    /*critical error*/
		return 0;
	}


	if (tbl->slot == NULL) tbl->slot = node; /*case of empty table*/
	else {
		head = tbl->slot;
		while (head->next != NULL) {
			head = head->next;
		}
		head->next = node; /*add to the end of the list*/
	}
	node->macro_lock = 0;
	node->index = tbl->size;
	node->next = NULL;


	return 1;
}


/*expands macro writing to  file*/
void expandMacro(macro_table_t *tbl, char *macro_name) {
	macro_node_t *head = NULL;
	int i = 0, LEN = 0, length = 0;


	if (tbl == NULL || tbl->slot == NULL || macro_name[0] == '\0'){
		report_error(ERR_MACRO_NAME_RETRIEVE, line_count, MACL, CRIT, 0);
		return;
	}


	if (tbl == NULL || tbl->size == 0) {
		printf("Macro Table Empty");
		return;
	}
	head = retNode(tbl, macro_name);
	LEN = getMacroLength(tbl, macro_name);

	while (head != NULL) {
		if ((strcmp(macro_name, head->macro_name)) == 0) {
			fprintf(fptr_after, "%s", head->macro_line);
			length++;
		}
		head = head->next;

	}

	if (length != LEN)
		report_error("Not All Macros were Printed\n", line_count, MACL, NON_CRIT, 0);


	return;
}


void MacroName(macro_node_t *head) {
	if (head != NULL) {
		printf("%s\n", head->macro_name);
	} else report_error(ERR_MACRO_NOT_FOUND, line_count, MACL, NON_CRIT, 0);
}

int getMacroLength(macro_table_t *tbl, char *macro_name) {
	macro_node_t *head = NULL;
	int length = 0;

	if (tbl == NULL || tbl->slot == NULL || macro_name[0] == '\0') return 0;

	if (dupNameExistsInTable(tbl, macro_name) == 0) {
		report_error(ERR_MACRO_NOT_FOUND, line_count, MACL, CRIT, 0);
		return 0;
	}
	head = tbl->slot;

	while (head != NULL) {
		if ((strcmp(macro_name, head->macro_name)) == 0) {
			length++;
		}
		head = head->next;
	}
	return length;
}


/*return the first instance of macro by macro name*/
macro_node_t *retNode(macro_table_t *tbl, char *macro_name) {
	macro_node_t *head = NULL;

	int i = 0, LEN = 0;
	if (tbl == NULL || tbl->size == 0 || tbl->slot == NULL) return 0;

	head = tbl->slot;


	while (head != NULL) {
		if (strcmp(head->macro_name, macro_name) == 0)
			return head;
		head = head->next;
	}

	/*not found*/
	return NULL;
}


/* Checks if a given name exists  |true 1 | false 0*/

int dupNameExistsInTable(macro_table_t *tbl, char *macro_name) {
	macro_node_t *head = NULL;
	if (tbl == NULL || tbl->size == 0 || tbl->slot == NULL)
		return 0;

	head = tbl->slot;

	while(head !=NULL )  {
		if((strcmp(head->macro_name, macro_name) == 0))
			return 1; /*Name found*/
			head=head->next;
	}
	return 0;/* Name not found*/
}

/*locks the all macro lines  from overriding*/
void macro_lock(macro_table_t *tbl, char *macro_name) {

	macro_node_t *head = NULL;
	int i = 0, len = 0;
	if (tbl == NULL || tbl->size == 0 || tbl->slot == NULL) return;

	if (!(head = retNode(tbl, macro_name))) {
		return;
	}
	head = tbl->slot;


	while (head != NULL) {
		if (strcmp(head->macro_name, macro_name) == 0) {
			head->macro_lock = 1;
		}
		head = head->next;

	}
}


void printAllMacros(macro_table_t *tbl) {
	macro_node_t *head = NULL;
	int i = 0;
	if (tbl == NULL || tbl->size == 0 || tbl->slot == NULL) {
		printf("Macro Table is empty or not initialized.\n");
		return;
	}
	head = tbl->slot;

	while (head != NULL) {
		printf("\n");
		printf("Macro Name: %s\t", head->macro_name);
		printf("Macro Line: %s", head->macro_line);
		head = head->next;
	}
}



