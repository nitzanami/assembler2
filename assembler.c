#include "assembler.h"

/* runs the assembler for each file given as a command line arguments */
int main(int argc, char **argv)
{/*
	char *file;
	file = argv[1];
	enum errors error;
	symboltable *symbolTable;
	dataimage *dataImage;
	uint32 icf,dcf;
	symbolTable = initSymbolTable();
	dataImage = initDataImage();
	error = first_transition(file, symbolTable, dataImage, &icf, &dcf);
	*/
	int i;
	
	for(i = 1 ; i < argc ; i++)
	{
		runAssembler(argv[i]);
	}
	return 0;
	
}
/* runs the assembler on the file with the given name*/
void runAssembler(char file[])
{
	enum errors error;
	symboltable *symbolTable;
	dataimage *dataImage;
	uint32 icf,dcf;
	symbolTable = initSymbolTable();
	dataImage = initDataImage();
	/* run the first transition*/
	error = first_transition(file, symbolTable, dataImage, &icf, &dcf);
	
	if(error == valid)/*if there was no error in the first transition, run the second transition*/
	{
		second_transition(file, symbolTable, dataImage,icf,dcf);
	}
	
	freeDataImage(dataImage);
	freeSymbolTable(symbolTable);
	
}
