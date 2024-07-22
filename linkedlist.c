#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"
#include "shared.h"
#include "global_vars.h"



macro_table_t *initTable(macro_table_t *tbl) {
	int i = 0;
	tbl = (macro_table_t *) malloc(sizeof(macro_table_t));
	for (i = 0; i < MAX_MACROS; ++i)
		tbl->slot[i] = NULL;
	tbl->isFull = 0;
	tbl->isMacroOpen = 0;
	tbl->amount = 0;
	tbl->size = MAX_MACROS;

	return tbl;
}


macro_node_t *constructMacroNode(char *macro_name, char *line, macro_node_t *next) {
	macro_node_t *node = malloc(sizeof(macro_node_t));
	/*problem with conflicting address macr_name and line*/
	if (*macro_name != '\0' || *line != '\0') {
		strcpy(node->macro_name, macro_name);
		strcpy(node->macro_line, line);
		node->macro_lock= 0;
		node->macro_next = next;
		return node;
	}
	return NULL;
}




void loadTable(macro_table_t *tbl, char *macro_name, char *line) {
	macro_node_t *temp = NULL;
	macro_node_t *last = NULL;
	int slot_idx = 0;

	if (tbl->amount == MAX_MACROS) {
		report_error(ERR_MACRO_PERMISSION, line_count);
		return;
	}


	/*new Macro Node  */
	if (!(temp = constructMacroNode(macro_name, line, NULL))) {
		report_error(ERR_MACRO_NODE_CREATION_FAILED,line_count);
		exit(0);
	}
	slot_idx = retSlot(tbl, macro_name);

	if (tbl->slot[slot_idx] != NULL) {
		last = retEndList(tbl->slot[slot_idx]);
		last->macro_next = temp;
	} else {
		tbl->slot[slot_idx] = temp;
		tbl->amount += 1;
	}

}


int expandMacro(macro_table_t *tbl , char * macro_name){
	int i= 0;
	if (!tbl) return 0;
	while(tbl->slot[i]!=NULL && i < tbl->size){
		if(strcmp(tbl->slot[i]->macro_name, macro_name)==0) {
			printMacro(tbl->slot[i]);
			return 1;
		}
		i++;
	}
	return 0;
}


void printMacro(macro_node_t *head ){
	macro_node_t *temp = head;
	while(temp->macro_next!=NULL)
		fprintf(fptr_after,"%s\n", temp->macro_line);
}



macro_node_t *retEndList(macro_node_t *head) {
	macro_node_t *temp = head;
	while (temp->macro_next != NULL)
		temp = temp->macro_next;
	return temp;
}

int retSlot(macro_table_t *tbl, char *macro_name) {
	int i = 0;
	for (i = 0; i < MAX_MACROS; ++i) {
		if (tbl->slot[i] != NULL) {
			if ((strcmp(macro_name, tbl->slot[i]->macro_name)) == 0) {
				 return i;
			}
		} else /*slot[i] is null */
			return i;
	}
	return -1;
}

