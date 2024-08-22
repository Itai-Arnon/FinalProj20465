

#ifndef M14_SECOND_PASS_H
#define M14_SECOND_PASS_H


#include "shared.h"
#include "symbols.h"
#include "first_pass.h"
#include "macro_list.h"
/*main function| checks for symbol problems and other address*/
void second_pass(symbol_table_t *, word_table_t *, word_table_t * , char *);

		void moveEntryValues(word_table_t *table , symbol_table_t *symbolTable);

/*check if there are duplicate symbols in macro table*/
void checkSymbolsUnique(macro_table_t*,  symbol_table_t *sym_table);
int convertToOctal(int num);

/*returns 1 for the first symbol w/o correct value*/
symbol_t * firstSymbolMissingValue(symbol_table_t *table);
/*2nd pass check of opcode*/
/*entry has a specific table , all entry are move to it*/
int moveSymbolsToEntry(symbol_table_t *sym_tbl ,symbol_table_t *entrySTable);

/*Check if symbols of type ARE_T E have the same name as symbols of type ARE_T A or R */
int checkExternSymbols(symbol_table_t *table);

/*adds existing symbol to another table */
int addSymbolToTable(symbol_table_t *table, symbol_t *_symbol);

/*all words that are relocatable addresses are updated*/
void updateRelocatableLines(word_table_t *table);

/*print to file ps.ent or stdout  entry information - use a specific symbol table */
void checkSymbolsUnique(macro_table_t *macro_table, symbol_table_t *sym_table);

/*count the number of Extern element in a table */
int countExternInTables(word_table_t *table);



/*printing table in octal */
void printTableToFile(word_table_t *wTable ,word_table_t *dTable, char* filename);
/*prints the Externals and their IC and DC locations*/
void printExternTable(word_table_t *table1, word_table_t *table2 , char  *filename);

/*print to file ps.ext or stdout  entry information - use a specific symbol table */
void printEntryTable(symbol_table_t *table, char *filename);

void print_symbol_table(symbol_table_t *sym_tbl);

/*print the word in binary form*/
void printBinary(unsigned short num);

/*print to file the entry table*/
void printEntryTableFile(symbol_table_t *, FILE *, int );
/*basic print of word_table */
void printTable(word_table_t *table);

#endif /*M14_SECOND_PASS_H*/
