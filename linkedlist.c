#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"
#include "shared.h"
#include "global_vars.h"


macro_table_t *initTable(macro_table_t *tbl) {
	int i = 0;
	tbl = (macro_table_t *) malloc(sizeof(macro_table_t));

	tbl->isEmpty = 1;
	tbl->isMacroOpen = 0;
	tbl->amount = 0;
	tbl->size = MAX_MACROS;

	for (i = 0; i < tbl->size; ++i)
		tbl->slot[i] = NULL;


	return tbl;
}


macro_node_t *constructMacroNode(char *macro_name, char *line, macro_node_t *next) {
	macro_node_t *node = malloc(sizeof(macro_node_t));
	/*problem with conflicting address macr_name and line*/
	if (*macro_name != '\0' || *line != '\0') {
		strcpy(node->macro_name, macro_name);
		strcpy(node->macro_line, line);
		node->macro_lock = 0;
		node->macro_next = next;
		return node;
	}
	return NULL;
}


void loadTable(macro_table_t *tbl, char *macro_name, char *line) {
	macro_node_t *temp = NULL;
	macro_node_t *last = NULL;
	int slot_idx = 0;

	if (tbl->amount == tbl->size) {
		report_error(MACRO_TABLE_FULL, line_count);
		return;
	}
	/*new Macro Node  */
	if (!(temp = constructMacroNode(macro_name, line, NULL))) {
		report_error(ERR_MACRO_NODE_CREATION_FAILED, line_count);
		return; /*critical error*/
	}

	if ((slot_idx = retSlot(tbl, macro_name)) != -1) {
	printf("%s\n",macro_name);
		if (tbl->slot[slot_idx] != NULL) {
			last = retEndList(tbl->slot[slot_idx]);
			last->macro_next = temp;
		} else {
			tbl->slot[slot_idx] = temp;
			tbl->amount += 1;
		}
		tbl->isEmpty = 0;
	} else {
		/*slot_idx value is -1*/
		report_error(ERR_MACRO_NAME_DUP, line_count);
		return;
	}
}

int expandMacro(macro_table_t *tbl, char *macro_name) {
	int i = 0;
	if (tbl->isEmpty == 1) return 0;
	while (tbl->slot[i] != NULL && i < tbl->size) {
		if (strcmp(tbl->slot[i]->macro_name, macro_name) == 0) {
			printMacro(tbl->slot[i]);
			return 1;
		}
		i++;
	}
	return 0;
}


void printMacro(macro_node_t *head) {
	macro_node_t *temp = head;
	while (temp != NULL) {
		printf("%s", temp->macro_line);
		fprintf(fptr_after,"%s", temp->macro_line);
		temp = temp->macro_next;
	}
}

void printMacroName(macro_node_t *head) {
	if (head != NULL) {
		printf("%s\n", head->macro_name);
	} else report_error(ERR_CHK_UNDEFINED_MACRO, line_count);
}


macro_node_t *retEndList(macro_node_t *head) {
	macro_node_t *temp = head;
	while (temp->macro_next != NULL)
		temp = temp->macro_next;
	return temp;
}

int retSlot(macro_table_t *tbl, char *macro_name) {
	int i = 0;

	if(tbl->isEmpty == 1)  return 0;

	for (i = 0; i < tbl->amount; ++i) {
		if (tbl->slot[i] != NULL) {
			if ((strcmp(macro_name, tbl->slot[i]->macro_name)) == 0)
				/*if macro already exist returns err*/
				return  (tbl->slot[i]->macro_lock == 1) ? i : -1;
		}
	}
	/*case of beginning or no name match*/
	for (i = 0; i < tbl->amount; ++i) {
		if (tbl->slot[i] == NULL)
			return i;
	}
/*other error*/
	return -1;
}



 /* Checks if a given name exists in any slot of the macro table  true 1 , false 0*/
int checkNameExistsInTable(macro_table_t *tbl, char *macro_name) {
	int i = 0;
	 if(tbl->isEmpty == 1)  return 0;

	for (i = 0; i < tbl->amount; ++i) {
		if (tbl->slot[i] != NULL) {
			/* Compare the macro_name of the first node in this slot with the given name*/
			if (strcmp(tbl->slot[i]->macro_name, macro_name) == 0)
				return 1;  /*Name found*/
		}
	}
	return 0;/* Name not found*/
}