#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include "constants.h"

/* create a string containing the file name with the correct extension (using malloc)*/
char *addExtensionToFile(char* nameWithoutExtension, char *extension);
/* returns a file pointer to the file with the given name and extension, opened for the given mode */
FILE *getfilepointer(char* nameWithoutExtension, char *extension, char *mode);
/*this function makes sure the file name is valid and if it is, opens the file to the pointer it gets*/
enum errors getFile(keletVars *kv, FILE **fp);
/* prints an instruction to the object file */
void printInstructionToObj(FILE *fp, uint32 data, uint32 ic);
/* prints a line of the extern file */
void printExternToFile(FILE *fp, uint32 address, char *symbol);

#endif
