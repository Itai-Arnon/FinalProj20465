
#ifndef M14_LINKEDLIST_H
#define M14_LINKEDLIST_H

#include "shared.h"




typedef struct macro_node_t{

	char macro_name[MAX_MACRO_NAME];/*ref to macro name*/
	char macro_line[LINE_LENGTH];/*holds all contents of macro - dynamically allocated*/
	struct macro_node_t *macro_next;

}macro_node_t;

typedef struct {
	int isFull;
	int isMacroOpen;
	int amount;
	macro_node_t *slot[MAX_MACROS];
}macro_table_t;


/*the table structure*/

/*allocation of new node with its params*/
macro_node_t *constructMacroNode(char* ,char* , macro_node_t*);
/*return the end of the LL inside a table row*/
macro_node_t* retEndList(macro_node_t*);

void loadTable(macro_table_t* tbl,char*macr_name, char*line);

/***
 *
 * @param tbl
 * @param macro_name
 * @return index of table slot of macro_name or a new null slot ,-1 if not found
 */
int retEmptySlot(macro_table_t *tbl , char* macro_name);
/**
 * initiates tables;
 * */
macro_table_t *initTable( macro_table_t *tbl);

#endif /*M14_LINKEDLIST_H*/
