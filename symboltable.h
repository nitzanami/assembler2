#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "constants.h"
#include <stdio.h>
typedef struct symboltable symboltable;
/*allocate memory for the symbol table and initinalize it*/
symboltable *initSymbolTable(void);
/* frees the memory used by the table and its contents*/
void freeSymbolTable(symboltable *table);
/*adds a symbol to the symbol table (does not check if the symbol is in the table already)*/
void addSymbol(symboltable *table, char *symbol,uint32 value,unsigned char attributes);
/*returns the attributes of the given symbol, or 0 if the symbol doesnt exist in the table*/
unsigned char getAttributes(symboltable *table, char *symbol);
/*returns the value of the given symbol, or -1 if the symbol doesnt exist in the table*/
uint32 getValue(symboltable *table, char *symbol);
/* returns true (1) if the symbol is in the table and false if it isnt (0) */
int doesSymbolExist(symboltable *table, char *symbol);
/*add the given attributes to the given symbol in the table*/
void addAttributes(symboltable *table, unsigned char attributes, char *symbol);
/* add the ic to all symbols with the attribute DATA */
void addIcToData(symboltable *table, uint32 ic);
/* creates the enrties file */
void createEntFile(FILE *fp, symboltable *table);

#endif
