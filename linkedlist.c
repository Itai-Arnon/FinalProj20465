#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"
#include "shared.h"


macro_node_t *constructMacroNode(char *macr_name, char *line, macro_node_t *next) {
	macro_node_t *macro_node_t = (calloc(1, sizeof(macro_node_t)));
	strcpy(macro_node_t->name, macr_name);
	strcpy(macro_node_t->mbuffer, line);
	macro_node_t->macro_next = next;
	return macro_node_t;
}


macro_node_t *retEndList(macro_node_t *head) {
	macro_node_t *temp = head;
	while (temp->macro_next != NULL)
		temp = temp->macro_next;
	return temp;
}

macro_table_t *loadTable(macro_table_t *tbl, char *macr_name, char *line, macro_node_t *next) {
	macro_node_t *temp = NULL;
	macro_node_t *last = NULL;
	int slot_idx = 0;

	if (tbl->amount == MAX_MACROS) {
		report_error(MACRO_TABLE_FULL);
		return NULL;
	}
	/*new Macro Node  */
	temp = constructMacroNode(macr_name, line, next);
	slot_idx = retEmptySlot(tbl, macr_name);

	if (tbl->slot[slot_idx] != NULL) {
		last = retEndList(tbl->slot[slot_idx]);
		last->macro_next = temp;
	} else {
		tbl->slot[slot_idx] = temp;
		tbl->amount += 1;
	}

	return tbl;
}

int retEmptySlot(macro_table_t *tbl, char *macr_name) {
	int i = 0;
	for (i = 0; i < MAX_MACROS; ++i) {
		if (tbl->slot[i] != NULL) {
			if ((strcmp(macr_name, tbl->slot[i]->name)) == 0) {
				if (i == MAX_MACROS - 1)
					tbl->isFull = 1;
				return i;
			}
		} else /*slot[i] is null */
			return i;
	}
	return -1;
}


macro_table_t *initTable() {
	int i = 0;
	macro_table_t *tbl = calloc(1, sizeof(macro_table_t));
	for (i = 0; i < MAX_MACROS; ++i) {
		tbl->slot[i] = NULL;
		tbl->isFull = 0;
		tbl->isMacroOpen = 0;
		tbl->amount = 0;
	}
	return tbl;
}