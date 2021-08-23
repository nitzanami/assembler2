#include "symboltable.h"
#include <stdlib.h>
#include <string.h>

typedef struct symbolNode{
	char symbol[MAX_SYMBOL_LENGTH+1];
	uint32 value;
	unsigned char attributes;
	struct symbolNode *next;
} symbolNode;
/*the symbol table is implemented as a linked list*/
struct symboltable
{
	symbolNode* head;
	symbolNode* tail;
};
/*allocates memory for a symbolnode and the array inside it, returns NULL if the allocation failed*/

static symbolNode* initsymbolNode(char* symbol,uint32 value,unsigned char attributes)
{
	/*allocate memory for the node*/
	symbolNode *newnode = malloc(sizeof(symbolNode));
	if(newnode==NULL)
		return NULL;
	/*copy the parameter values into the symbolNode*/
	newnode->value = value;
	newnode->attributes = attributes;
	newnode->next = NULL;
	strcpy(newnode->symbol,symbol);
	
	return newnode;
}
/*allocate memory for the symbol table and initinallize it*/
symboltable *initSymbolTable(void)
{
	/* allocate memory*/
	symboltable* newtable = malloc(sizeof(symboltable));
	if(newtable==NULL)
		return NULL;
	/*initialize fields to NULL*/
	newtable->head = NULL;
	newtable->tail = NULL;
	
	return newtable;
}
/* frees the memory used by the table and its contents*/
void freeSymbolTable(symboltable *table)
{
	symbolNode *current, *next;
	if(table == NULL)
		return;
	/* the loop iterates over the linked list and free each node */
	current = table->head;
	while(current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	
	free(table);
}
/*adds a symbol to the symbol table (does not check if the symbol is in the table already), returns 0 if the addition 
 was succesful, 1 if it failed to allocate memory*/
void addSymbol(symboltable *table, char *symbol,uint32 value,unsigned char attributes)
{
	symbolNode *newnode;
	/* allocate memory for a node with the given values, and make sure the allocation didnt fail*/
	newnode = initsymbolNode(symbol,value,attributes);
	checkMalloc(newnode);
	/* if the table is empty, insert the new node at the start.
	   if it isnt empty, insert the new node after the tail node and move the tail pointer*/
	if(table->head == NULL)
		table->head = table->tail = newnode;
	else
	{
		table->tail->next = newnode;
		table->tail = newnode;
	}
}
/* returns true (1) if the symbol is in the table and false if it isnt (0) */
int doesSymbolExist(symboltable *table, char *symbol)
{
	symbolNode *current = table->head;
	/* iterate over the list, and if we find a matching symbol, return true (1)*/
	while(current != NULL)
	{
		if(strcmp(current->symbol,symbol)==0)
			return 1;
		current = current->next;
	}
	return 0;
}
/*returns the attributes of the given symbol, or 0 if the symbol doesnt exist in the table*/
unsigned char getAttributes(symboltable *table, char *symbol)
{
	symbolNode *current = table->head;
	/* iterate over the list, and if we find a matching symbol, return its attributes*/
	while(current != NULL)
	{
		if(strcmp(current->symbol,symbol)==0)
			return current->attributes;
		current = current->next;
	}
	return 0;
}
/*returns the value of the given symbol, or -1 if the symbol doesnt exist in the table*/
uint32 getValue(symboltable *table, char *symbol)
{
	symbolNode *current = table->head;
	/* iterate over the list, and if we find a matching symbol, return its value*/
	while(current != NULL)
	{
		if(strcmp(current->symbol,symbol)==0)
			return current->value;
		current = current->next;
	}
	return -1;
}
/*add the given attributes to the given symbol in the table*/
void addAttributes(symboltable *table, unsigned char attributes, char *symbol)
{
	symbolNode *current = table->head;
	/* iterate over the list, and if we find a matching symbol, add the given attributes to its attributes*/
	while(current != NULL)
	{
		if(strcmp(current->symbol,symbol)==0)
		{
			current->attributes |= attributes;
			return;
		}
		current = current->next;
	}
}
/* add the ic to all symbols with the attribute DATA */
void addIcToData(symboltable *table, uint32 ic)
{
	symbolNode *current = table->head;
	/* iterate over the list, and if we find data ,add ic to its value*/
	while((current!= NULL))
	{
		if(current->attributes & DATA)
			current->value += ic;
		current = current->next;
	}
}
/* creates the enrties file */
void createEntFile(FILE *fp, symboltable *table)
{
	symbolNode *current = table->head;
	/* iterate over the list, and if we find an entry , print it to the file pointer in the correct format*/
	while((current!= NULL))
	{
		if(current->attributes & ENTRY)
			fprintf(fp,"%s %04lu\n", current->symbol, current->value);
		current = current->next;
	}
}
