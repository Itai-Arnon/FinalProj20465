

#ifndef M14_SECOND_PASS_H
#define M14_SECOND_PASS_H


#include "shared.h"
#include "symbols.h"
#include "first_pass.h"
#include "macro_list.h"
/*main function| checks for symbol problems and other address*/
void second_pass(macro_table_t *, symbol_table_t *, symbol_table_t * ,word_table_t *, word_table_t *);

		void moveEntryValues(word_table_t *table , symbol_table_t *symbolTable);

/*check if there are duplicate symbols in macro table*/
void checkSymbolsUnique(macro_table_t*,  symbol_table_t *sym_table);
int convertToOctal(int num);
/*printing table in octal */
void printTableToFile(word_table_t *wTable ,word_table_t *dTable, char* file_name);
/*returns 1 for the first symbol w/o correct value*/
symbol_t * firstSymbolMissingValue(symbol_table_t *table);
/*2nd pass check of opcode*/
void checkOPCODE_INSTRUCTION(symbol_table_t *sym_tbl, word_table_t *table);
void checkOPCODE_WORDS(symbol_table_t *sym_tbl, word_table_t *table, int idx, int type);
void checkDATA_WORDS(symbol_table_t *sym_tbl, word_table_t *table);
void checkSTRING_WORDS(symbol_table_t *sym_tbl, word_table_t *table);
void checkEXTERN(symbol_table_t *sym_tbl, word_table_t *table);
/*entry has a specific table , all entry are move to it*/
int moveSymbolsToEntry(symbol_table_t *sym_tbl ,symbol_table_t *entrySTable);

/*Check if symbols of type ARE_T E have the same name as symbols of type ARE_T A or R */
int checkExternSymbols(symbol_table_t *table);
/*adds an existing symbol to a table*/
int addSymbolToTable(symbol_table_t *table, symbol_t *_symbol);
void print_symbol_table(symbol_table_t *sym_tbl);

/*print to file ps.ext or stdout  entry information - use a specific symbol table */
void printExternTable(word_table_t *table, FILE *file, int outputType);

/*print to file ps.ent or stdout  entry information - use a specific symbol table */
/*void printEntrySymbolTable(symbol_table_t *table, FILE *file, int outputType);*/

void update_Reloc_Lines(word_table_t *table);
void printBinary(unsigned short num);

void printTable(word_table_t *table);



#endif /*M14_SECOND_PASS_H*/
