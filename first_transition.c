#include "first_transition.h"
#include <stdlib.h>

static enum errors executeLine(symboltable *symbolTable,dataimage *dataImage,keletVars* kv)
{
	char *label;
	enum errors errorFound = valid;;
	errorFound = getCommandName(kv);
	if(errorFound == valid)/* get the label if there is any, and then the instruction or guidance, setting flags accordingly*/
	{
		if(kv->isExtern)/*on extern line, we add the argument of the command to the symbol table if it is a legal label*/
		{
			errorFound = getArgument(&label,kv);
			if(errorFound == valid)/* get the label after the command*/
			{
				if(isLabelLegal(label,kv))/*print message if the label is illegal , if label is legal add it*/
					errorFound = addLabel(symbolTable,label,kv);
			}
		}
		else if(kv->isLabel && !kv->isEntry) /* if we found a label on a non entry line, add it*/
		{
			if(isLabelLegal(kv->label,kv))/*print message if the label is illegal , if label is legal add it*/
				errorFound = addLabel(symbolTable,kv->label, kv);
		}
		if(!kv->isInstruction)/* if the line declates data, add it to data image*/
			addGuidanceToData(dataImage,kv);
		else /*if the line is an instruction, increment ic*/
			kv->ic += INSTRUCTION_BYTE_LEN;	
	}
	return errorFound;
}
/*this function gets a file name, opens it and extracts the symbols and data image*/
enum errors first_transition(char file[], symboltable *symbolTable, dataimage *dataImage,uint32 *icf, uint32 *dcf)
{
	/*declare variables*/
	FILE *fp;
	enum errors lineErr = valid, err = valid, isErrInFile = valid;
	keletVars kv;
	kv.file = file;
	kv.row = kv.dc = 0;
	kv.ic = MEMORY_START_ADDRESS;
	
	/*open input file*/
	if (getFile(&kv, &fp) != valid)
		return invalid;
	
	while ((lineErr != eof) && (lineErr != emptyAndeof)) /*read lines until we reach EOF*/
	{	
		lineErr = getCommandLine(fp, &kv);/*read next line*/
		if (lineErr == valid || lineErr == eof)/*valid line*/
		{	
			err = executeLine(symbolTable, dataImage,&kv);
		}
		/*an error occured in the current line*/
		if ((err == invalid) || (lineErr == invalid))
			isErrInFile = invalid;
	}
	addIcToData(symbolTable, kv.ic);/* add ic to all labels with attribute data*/
	*icf = kv.ic;
	*dcf = kv.dc;
	fclose(fp);
	return isErrInFile;
}

/*this function adds the label to the symbol table if it isnt already defined. if the label is defined it alerts about it properly*/
enum errors addLabel(symboltable *symbolTable,char *label, keletVars *kv)
{
	unsigned char attribute;
	unsigned long value;
	if (!doesSymbolExist(symbolTable, label))/*label isn't yet defined*/
	{
		if(kv->isExtern)
		{
			value = 0;
			attribute = EXTERN;
		}
		else if(kv->isInstruction)
		{
			value = kv->ic;
			attribute = CODE;
		} 
		else
		{
			value = kv->dc;
			attribute = DATA;
		}
		addSymbol(symbolTable,label, value, attribute);
		return valid;
	}
	else
	{
		attribute = getAttributes(symbolTable, label);
		
		/* check for errors in redifnition of a label*/
		if((attribute & EXTERN) && kv->isExtern)
			return valid;
		else if ((attribute & EXTERN) && !kv->isExtern)
			printError("label already defined as extern, can't redefine the label");
		else if (attribute & DATA)
			printError("label already defined as data, can't redefine the label");
		else if (attribute & CODE)
			printError("label already defined as code, can't redefine the label");
		return invalid;
	}

}
/*a adds a dw\dh\db guidance to the dataImage if it is valid*/
enum errors addNonAscizData(int bytesToDivide,dataimage *dataImage, keletVars *kv)
{
	int numberOfNums,lenOfCharArray;
	char *s;
	enum errors e = valid;
	e = getSet(&numberOfNums, kv); /* read the parameter list into kv->numbers*/
	if (e == valid)
	{
		e = check_d_set(&s, bytesToDivide, numberOfNums, kv);/*check that all number parameters are of legal size, if they are store them in s*/
		if(e == valid)
		{
			lenOfCharArray = bytesToDivide * numberOfNums;
			addData(dataImage, s, lenOfCharArray);
			kv->dc += lenOfCharArray;
			free(s);
		}
	}
	return e;
}
/*this function adds a guidance line to the data image*/
enum errors addGuidanceToData(dataimage *dataImage, keletVars *kv)
{
	int bytesToDivide = 0, lenOfCharArray;
	char *s;
	enum errors e = valid;
	
	if(kv->cmd != NULL)
	{
		/* find the type of guidance word */
		if(strcmp(kv->cmd, ".dw") == 0)
			bytesToDivide = DW_BYTES;
		else if(strcmp(kv->cmd, ".dh") == 0)
			bytesToDivide = DH_BYTES;
		else if(strcmp(kv->cmd, ".db") == 0)
			bytesToDivide = DB_BYTES;
			
		if(bytesToDivide != 0)/* if guidance is dw/dh/db */
		{
			addNonAscizData(bytesToDivide,dataImage,kv);
		}
		else if (strcmp(kv->cmd, ".asciz") == 0)/*if the guidance is asciz */
		{
			e = getAscizArgument(&s,kv); /*get the string for the asciz guidance*/
			if(e == valid)
			{
				lenOfCharArray = strlen(s) + 1;
				addData(dataImage, s, lenOfCharArray);
				kv->dc += lenOfCharArray;
			}
		}
	}
	
	return e;
}

/*this function gets the set numbers from the command line and checks that they are all long numbers. alerts on errors if there are any*/
enum errors getSet(int *numberOfNums, keletVars *kv)
{
	long num, i = 0;
	char *n;
	while(getArgument(&n,kv) == valid)/* read arguments until we find an error*/
	{
		if(n[0] == '\0')/* if we find an empty argument, we reached the end of the line*/
		{				
			if(i == 0)/*if there are 0 arguments print error*/
			{
				printError("missing arguments for guidance");
				return invalid;
			}
			break;/* stop reading arguments because we reached an empty argument*/
		}		
		if(my_atol(n,&num,kv) == valid)/*if the argument is a valid number, store it*/
		{
			kv->numbers[i++] = num;
		}
		else
			return invalid;/*there was an invalid argument*/
	}
	
	*numberOfNums = i;
	return valid;
}

/*this function checks for correct numbers in the set, splits the numbers into char sized blocks and stores them in a given char array*/
enum errors check_d_set(char **s, int bytesToDivide, int numberOfNums, keletVars *kv)
{
	int i, j;
	long range;
	
	/*find the range of numbers that fit in the given amount of bytesToDivide*/
	range = (signed long)1 << (bytesToDivide * BIT_IN_BYTE - 1);
	
	/* allocate char array to store the data in*/
	*s = malloc(numberOfNums * bytesToDivide);
	checkMalloc(*s);
	
	/* for every number in the num array*/
	for (i = 0; i < numberOfNums; i++)
	{
		/* make sure the number is in the valid range*/
		if ((range - 1) < kv->numbers[i] ||  -range > kv->numbers[i])
		{
			printError("number too big to fit in the number of bytes given");
			return invalid;
		}
		/*dividing the number into char sized blocks and saving them in order*/
		for (j = 0; j < bytesToDivide; j++) 
		{
			(*s)[i * bytesToDivide + j] = (char)((kv->numbers[i] >> (j * BIT_IN_BYTE)) & BYTE_MASK);
		}
	}
	return valid;
}


