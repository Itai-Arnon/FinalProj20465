
#ifndef M14_MACRO_LIST_H
#define M14_MACRO_LIST_H

#include "shared.h"


typedef struct  {
	char macro_name[MAX_MACRO_NAME];/*ref to macro name*/
	char macro_line[LINE_LENGTH];/*holds all contents of macro - dynamically allocated*/
	int macro_lock;
	int index;
}macro_node_t;

typedef struct {
	int isEmpty;
	int isMacroOpen;
	int size;
	macro_node_t *slot;
} macro_table_t;


/*  initiates tables; */
macro_table_t *initMacroTable(macro_table_t *tbl);

/*the table structure*/
/*allocation of new node with its params*/
macro_node_t *constructMacroNode(macro_table_t *tbl, char *macro_name, char *line);

/*return the end of the LL inside a table row*/
int retLengthMacro(macro_table_t *, char *);


/*load macro to table*/
int loadMacroTable(macro_table_t *tbl, char *macr_name, char *line);

/* @return index of table slot of macro_name or a new null slot ,-1 if not found*/
int retSlot(macro_table_t *tbl, char *macro_name);

int getMacroLength(macro_table_t *tbl, char *macro_name);

int dupNameExistsInTable(macro_table_t *tbl, char *macro_name );

void expandMacro(macro_table_t *tbl, char *macro_name);

macro_node_t *retMacro(macro_table_t *tbl, char *macro_name);

void printMacroName(macro_node_t *head);

/*lock the macro so it would not be accessed accidently*/
void macro_lock(macro_table_t *tbl, char *macro_name);

#endif /*M14_MACRO_LIST_H*/