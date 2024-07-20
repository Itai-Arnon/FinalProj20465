#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"
#include "shared.h"


macro_table_t *initTable(macro_table_t *tbl) {
	int i = 0;
	tbl = (macro_table_t *) malloc(sizeof(macro_table_t));
	for (i = 0; i < MAX_MACROS; ++i)
		tbl->slot[i] = NULL;
	tbl->isFull = 0;
	tbl->isMacroOpen = 0;
	tbl->amount = 0;

	return tbl;
}


macro_node_t *constructMacroNode(char *macr_name, char *line, macro_node_t *next) {
	macro_node_t *node = malloc(sizeof(macro_node_t));
	/*problem with conflicting address macr_name and line*/
	if (*macr_name != '\0' || *line != '\0') {
		strcpy(node->macro_name, macr_name);
		strcpy(node->macro_line, line);
		node->macro_next = next;
		return node;
	}
	return NULL;
}


macro_node_t *retEndList(macro_node_t *head) {
	macro_node_t *temp = head;
	while (temp->macro_next != NULL)
		temp = temp->macro_next;
	return temp;
}

void loadTable(macro_table_t *tbl, char *macr_name, char *line) {
	macro_node_t *temp = NULL;
	macro_node_t *last = NULL;
	int slot_idx = 0;

	if (tbl->amount == MAX_MACROS) {
		report_error(ERR_MACRO_PERMISSION);
		return;
	}
	/*new Macro Node  */
	if (!(temp = constructMacroNode(macr_name, line, NULL))) {
		report_error(ERR_MACRO_NODE_CREATION_FAILED);
		exit(0);
	}
	slot_idx = retEmptySlot(tbl, macr_name);

	if (tbl->slot[slot_idx] != NULL) {
		last = retEndList(tbl->slot[slot_idx]);
		last->macro_next = temp;
	} else {
		tbl->slot[slot_idx] = temp;
		tbl->amount += 1;
	}

}

int retEmptySlot(macro_table_t *tbl, char *macr_name) {
	int i = 0;
	for (i = 0; i < MAX_MACROS; ++i) {
		if (tbl->slot[i] != NULL) {
			if ((strcmp(macr_name, tbl->slot[i]->macro_name)) == 0) {
				if (i == MAX_MACROS - 1)
					tbl->isFull = 1;
				return i;
			}
		} else /*slot[i] is null */
			return i;
	}
	return -1;
}

