#include "first_transition.h"
#include <stdlib.h>

/*this function gets a file name, opens it and extracts the symbols and data image*/
enum errors first_transition(char file[], symboltable *symbolTable, dataimage *dataImage,uint32 *icf, uint32 *dcf)
{
	char *label;
	FILE *fp;
	enum errors lineErr = valid, err = valid, isErrInFile = valid;
	keletVars kv;
	
	kv.file = file;
	kv.row = kv.dc = 0;
	kv.ic = MEMORY_START_ADDRESS;
	
	/*open input file*/
	if (getFile(&kv, &fp) != valid)
		return invalid;
	
	while ((lineErr != eof) && (lineErr != emptyAndeof)) /*haven't reached the enf of the file*/
	{
		lineErr = getCommandLine(fp, &kv);
		/*valid line and command*/
		if (lineErr == valid || lineErr == eof) 
		{	
			err = getCommandName(&kv);
			if(err == valid)
			{
				if (kv.isLabel || kv.isExtern) /*first transition ignores entries*/
				{
					if(kv.isExtern)
					{
						err = getArgument(&label,&kv);/* if the command is extern, we add the label after the command*/
						if(err == valid)
							err = addLabel(symbolTable,label,&kv);
					}
					else if(!kv.isEntry) /*first transition doesn't analyze entries*/
					{
						err = addLabel(symbolTable,kv.label, &kv);
					}
				}
				if(!kv.isInstruction)
					addGuidanceToData(dataImage,&kv);
				else
					kv.ic += INSTRUCTION_BYTE_LEN;	
			}
		}
		/*an error occured*/
		if ((err == invalid) || (lineErr == invalid))
			isErrInFile = invalid;
	}
	addIcToData(symbolTable, kv.ic);
	*icf = kv.ic;
	*dcf = kv.dc;
	fclose(fp);
	return isErrInFile;
}

/*this function adds the label to the symbol table if it isnt already defined. if the label is defined it alerts about it properly*/
enum errors addLabel(symboltable *symbolTable,char *label, keletVars *kv)
{
	unsigned char atribute;
	unsigned long value;
	/*label isn't yet defined*/
	if (!doesSymbolExist(symbolTable, label))
	{
		(kv->isExtern) ? (value = 0, atribute = EXTERN) : (kv->isInstruction) ? (value = kv->ic, atribute = CODE) : (value = kv->dc, atribute = DATA);
		addSymbol(symbolTable,label, value, atribute);
		return valid;
	}
	atribute = getAttributes(symbolTable, label);
	if((atribute & EXTERN) && kv->isExtern)
		return valid;
	else if ((atribute & EXTERN) && !kv->isExtern)
		printError("label already defined as extern, can't redefine the label");
	else if (atribute & DATA)
		printError("label already defined as data, can't redefine the label");
	else if (atribute & CODE)
		printError("label already defined as code, can't redefine the label");
	
	return invalid; 
}

/*this function adds a guidance line to the data image*/
enum errors addGuidanceToData(dataimage *dataImage, keletVars *kv)
{
	int bytesToDivide, numberOfNums, lenOfCharArray;
	char *s;
	enum errors e = valid;
	if (kv->charsChecked >= kv->lineLength)
	{
		printError("missing data after guidance");
		return invalid;
	}
	/*a dw\dh\db guidance, gets the proper amount of bytes that each number in the set should take.
	adds the set to the data image if its valid*/
	if ((bytesToDivide = (strcmp(kv->cmd, ".dw") == 0) ? DW_BYTES : (strcmp(kv->cmd, ".dh") == 0) ? DH_BYTES : (strcmp(kv->cmd, ".db") == 0) ? DB_BYTES : 0))
	{
		e = getSet(&numberOfNums, kv);
		if (e == valid)
		{
			e = check_d_set(&s, bytesToDivide, numberOfNums, kv);
			if(e == valid)
			{
				lenOfCharArray = bytesToDivide * numberOfNums;
				addData(dataImage, s, lenOfCharArray);
				kv->dc += lenOfCharArray;
				free(s);
			}
		}

	}
	else if (strcmp(kv->cmd, ".asciz") == 0)
	{
		e = getAscizArgument(&s,kv);
		if(e == valid)
		{
			lenOfCharArray = strlen(s) + 1;
			addData(dataImage, s, lenOfCharArray);
			kv->dc += lenOfCharArray;
		}
		
	}
	return e;
}



/*this function gets the set numbers from the command line and checks that they are all long numbers. alerts on errors if there are any*/
enum errors getSet(int *numberOfNums, keletVars *kv)
{
	long num, i = 0;
	char *n;
	while(getArgument(&n,kv) == valid)
	{
		if(my_atol(n,&num) == valid)
		{
			kv->numbers[i++] = num;
		}
		else
			break;
	}
	*numberOfNums = i;	
	return valid;
}

/*this function checks for correct numbers in the set, splits the numbers into char sized blocks and stores them in a given char array*/
enum errors check_d_set(char **s, int bytesToDivide, int numberOfNums, keletVars *kv)
{
	int i, j;
	long range;
	
	range = (signed long)1 << (bytesToDivide * BIT_IN_BYTE - 1);
	
	if ((*s = (char *) malloc(numberOfNums * bytesToDivide)) == NULL)
	{
		printError("an error occured while allocating memory");
		return invalid;
	}
	
	for (i = 0; i < numberOfNums; i++)
	{
		if ((range - 1) < kv->numbers[i] ||  -range > kv->numbers[i]) /*number outside of range*/
		{
			printError("number too big to fit in the number of bytes given");
			return invalid;
		}
		for (j = 0; j < bytesToDivide; j++) /*dividing the number into char sized blocks and saving them*/
		{
			(*s)[i * bytesToDivide + j] = (char)((kv->numbers[i] >> (j * BIT_IN_BYTE)) & BYTE_MASK);
		}
	}
	return valid;
}


