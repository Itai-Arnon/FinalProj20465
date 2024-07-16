
#ifndef M14_LINKEDLIST_H
#define M14_LINKEDLIST_H

#define MAX_MACRO_NAME  10
#define LINE_LENGTH 80
#define MAX_MACROS 20

typedef struct macro_node_t{
	char name[MAX_MACRO_NAME];/*ref to macro name*/
	char mbuffer[LINE_LENGTH];/*holds all contents of macro - dynamically allocated*/
	struct macro_node_t *macro_next;
}macro_node_t;

typedef struct {
	macro_node_t *slot[MAX_MACROS];
}macro_table_t;


/*the table structure*/

/*allocation of new node with its params*/
macro_node_t *constructMacroNode(char* ,char* , macro_node_t*);
/*return the end of the LL inside a table row*/
macro_node_t* retEndList(macro_node_t*);

macro_table_t* loadTable(macro_table_t* tbl,char*macr_name, char*line);

macro_node_t retEmptySlot(macro_table_t*, char*)


#endif /*M14_LINKEDLIST_H*/
