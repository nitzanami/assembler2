#ifndef FILES_H
#define FILES_H

/* create a string containing the file name with the correct extension (using malloc)*/
char *addExtensionToFile(char* nameWithoutExtension, char *extension);
/* returns a file pointer to the file with the given name and extension, opened for the given mode */
FILE *getfilepointer(char* nameWithoutExtension, char *extension, char *mode);
/* prints an instruction to the object file */
void printInstructionToObj(FILE *fp, uint32 data, uint32 ic);
/* prints a line of the extern file */
void printExternToFile(FILE *fp, uint32 address, char *symbol);

#endif
