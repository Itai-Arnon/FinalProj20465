#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"



macro_node_t *constructMacroNode(char *macr_name,char* line ,macro_node_t *next )
{
	macro_node_t *macro_node_t = (calloc(1,sizeof(macro_node_t)));
	strcpy(macro_node_t->name,macr_name);
	strcpy(macro_node_t->mbuffer,line);
	macro_node_t->macro_next=NULL;
	return macro_node_t;
}


macro_node_t* retEndList(macro_node_t* head)
{
	macro_node_t *temp = head;
	while(temp->macro_next!=NULL)
		temp=temp->macro_next;

	return temp;
}

macro_table_t* loadTable(macro_table_t* tbl, char*macr_name, char*line)
{
	macro_node_t* temp =NULL;
	macro_node_t* last = NULL;


	{
		if (hash_arr[i] != 0)
		{
			temp = constructMacroNode( macr_name,line ,NULL);
			if (tbl->slot[i] != NULL)
			{
				last = retEndList(tbl->slot[i]);
				last->next = temp;
			}
			else
				tbl->slot[i] = temp;

			temp->next = NULL;
		}
	}
	return tbl;
}

macro_node_t retEmptySlot(macro_table_t* tbl , char* macr_name){
	int i = 0;
	
}

macro_table_t* initTable()
{
	int i = 0;
	macro_table_t* tbl = calloc(1,sizeof(macro_table_t));
	for ( i = 0; i < ARR_SIZE; ++i)
	{
		tbl->slot[i] = NULL;
	}
	return tbl;
}