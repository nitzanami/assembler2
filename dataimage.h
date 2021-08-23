#ifndef DATAIMAGE_H
#define DATAIMAGE_H

typedef struct dataimage dataimage;
/* allocate memory for the dataImage and return a pointer */
dataimage *initDataImage(void);
/* free the memory used by the dataImage */
void freeDataImage(dataimage *dp);
/* adds the data to the dataImage */
void addData(dataimage *dp, char *data,unsigned char length);
/* prints the data into the object file, with the requierd format:
the memory adress as a 4 digit decimal and then 4 bytes in hexa	on every line */
void writeDataToFile(dataimage *dp, uint32 startAddress, FILE *fp);

#endif
