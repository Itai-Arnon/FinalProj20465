
#ifndef M14_LINKEDLIST_H
#define M14_LINKEDLIST_H

#define MAX_MACRO_NAME  10
#define LINE_LENGTH 80
#define MAX_MACROS 20

#define MACRO_TABLE_FULL "Error No more space in Macro Table"
#define MACRO_TABLE_CLOSED "Error Macro Table Closed"

typedef struct macro_node_t{

	char name[MAX_MACRO_NAME];/*ref to macro name*/
	char mbuffer[LINE_LENGTH];/*holds all contents of macro - dynamically allocated*/
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

macro_table_t* loadTable(macro_table_t* tbl,char*macr_name, char*line, macro_node_t *next);

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
macro_table_t *initTable();

#endif /*M14_LINKEDLIST_H*/
