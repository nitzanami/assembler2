#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "files.h"
/* returns a file pointer to the file with the given name and extension, opened for the given mode */
FILE *getfilepointer(char* nameWithoutExtension, char *extension,char *mode)
{
	char *filename;
	FILE *fp;
	filename = addExtensionToFile(nameWithoutExtension,extension);
	fp = fopen(filename,mode);
	free(filename);

	return fp;
}
/* prints an instruction to the object file */
void printInstructionToObj(FILE *fp, uint32 data, uint32 ic)
{
	int i;
	fprintf(fp,"\n%04lu",ic);/* at the start of the line, print the address*/
	for(i = 0; i < OBJ_LINE_LENGTH; i++)
		fprintf(fp," %02X", (unsigned char)(data >> (i*BIT_IN_BYTE)) & BYTE_MASK);/*print every byte in a 2 digit hexa number*/
}
/* prints a line of the extern file */
void printExternToFile(FILE *fp, uint32 address, char *symbol)
{
	fprintf(fp,"%s %04lu\n",symbol,address);
}
/* create a string containing the file name with the correct extension (using malloc)*/
char *addExtensionToFile(char* nameWithoutExtension, char *extension)
{
	/*allocate memory to the new string*/
	char *filename = malloc(strlen(nameWithoutExtension) + strlen(extension) + 1);
	strcpy(filename,nameWithoutExtension);/*copy the name without extension */
	strcat(filename,extension);/*add the extension*/
	return filename;
}
