#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "dataimage.h"

/* the dataimage is implemented as a linked list of char arrays */
typedef struct dataNode
{
	char *data;
	unsigned char length;
	struct dataNode *next;
} dataNode;

struct dataimage
{
	dataNode *head, *tail;
};
/* allocates memory for the dataImage and returns a pointer */
dataimage *initDataImage(void)
{
	dataimage *result = malloc(sizeof(dataimage));
	checkMalloc(result);
	
	result->head = result->tail = NULL;
	return result;
}
/* free the memory used by the dataimage*/
void freeDataImage(dataimage *dp)
{
	dataNode *current, *next;
	if(dp == NULL)
		return;
	/* iterate over the linked list and frees each node */
	current = dp->head;
	while(current != NULL)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}
	free(dp);
}
/* copy the data into a new node and add it to the dataimage*/
void addData(dataimage *dp, char *data,unsigned char length)
{
	/* allocate memory to the node */
	dataNode *newNode = malloc(sizeof(dataNode));
	checkMalloc(newNode);
	
	newNode->data = malloc(length);
	checkMalloc(newNode->data);
	
	newNode->length = length;
	newNode->next = NULL;
	
	/* copy the data to the node */
	while(length-- > 0)
		newNode->data[length] = data[length];
	/* add the new node to the list */
	if(dp->head == NULL)
		dp->head = newNode;
	else
		dp->tail->next = newNode;
	
	dp->tail = newNode;
	
}
/* prints the data into the object file, with the requierd format : the memory adress as a 4 digit decimal and then 4 bytes in hexa	on every line */
void writeDataToFile(dataimage *dp, uint32 startAddress, FILE *fp)
{ 
	int i;
	uint32 count = 0;
	dataNode *node = dp->head;
	while(node != NULL)
	{
		for(i = 0 ; i < node->length ; i++, count++)
		{
			/* at the start of a line print the address as a 4 digit decimal number with padding of 0's*/
			if((count % OBJ_LINE_LENGTH) == 0) 
				fprintf(fp,"%04lu", count + startAddress);
			/* print the current byte in hexadecimal */
			fprintf(fp," %02X", node->data[i] & BYTE_MASK);
			/* if we reached end of line, print \n*/
			if((count % OBJ_LINE_LENGTH) == OBJ_LINE_LENGTH -1)
				putc('\n',fp);
		}
		node = node->next;
	}
	/* make sure the file ends with a \n character*/
	if((count % OBJ_LINE_LENGTH) != OBJ_LINE_LENGTH -1)
		putc('\n',fp);
}
