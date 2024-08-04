
#ifndef M14_MACRO_LIST_H
#define M14_MACRO_LIST_H

#include "shared.h"




typedef struct macro_node_t{
	char macro_name[MAX_MACRO_NAME];/*ref to macro name*/
	char macro_line[LINE_LENGTH];/*holds all contents of macro - dynamically allocated*/
	int macro_lock;
	struct macro_node_t *macro_next;
}macro_node_t;

typedef struct {/*change to dynmaic allocation*/
	int isEmpty;
	int isMacroOpen;
	int amount;
	int size;
	macro_node_t *slot[MAX_MACROS];
}macro_table_t;


/*  initiates tables; */
macro_table_t *initMacroTable( macro_table_t *tbl);

/*the table structure*/
/*allocation of new node with its params*/
macro_node_t *constructMacroNode(char* ,char* , macro_node_t*);
/*return the end of the LL inside a table row*/
macro_node_t* retEndList(macro_node_t*);


/*load macro to table*/
void loadMacroTable(macro_table_t* tbl,char*macr_name, char*line);
/* @return index of table slot of macro_name or a new null slot ,-1 if not found
 */
 static int retSlot(macro_table_t *tbl , char* macro_name);



int checkNameExistsInTable(macro_table_t *tbl,  char *macro_name );

int expandMacro(macro_table_t *tbl , char * macro_name);

void printMacro(macro_node_t *head );

void printMacroName(macro_node_t *head);
/*lock the macro so it would not be accessed accidently*/
void macro_lock(macro_table_t *tbl, char *macro_name);
#endif /*M14_MACRO_LIST_H*/