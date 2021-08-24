#include "second_transition.h"
#include "instExecution.h"

/*executes a line in the second transition, returns valid if there were no errors, else invalid*/
static enum errors executeLine(symboltable *symbolTable,keletVars* kv, FILE *obj, FILE *ext)
{
	char *label;
	if(getCommandName(kv) == valid)
	{
		if(kv->isEntry)/* on entry line*/
		{
			if(getArgument(&label,kv) == valid)/* read argument for entry*/
			{
				if(doesSymbolExist(symbolTable,label))
				{
					if(!(getAttributes(symbolTable,label) & EXTERN))/*entry must not be extern too*/
						addAttributes(symbolTable,ENTRY,label);
					else
					{
						printError("illegal entry and extern on the same label");
						return invalid;
					}
				}
				else
				{
					printError("label does not exist");
					return invalid;
				}
			}
		}
		/* if there is an instruction, we encode it and if it isnt valid we return invalid */
		else if(kv->isInstruction && (encode_instruction(symbolTable,kv,obj,ext) != valid))
			return invalid;
	}
	else
		return invalid;
	return valid;
}
/*executes the second_transition and creates output files */
enum errors second_transition(char file[],symboltable *symbolTable,dataimage *dataImage,uint32 icf, uint32 dcf)
{
	/*declare variables*/
	FILE *assembly, *obj, *ent, *ext;
	enum errors lineErr = valid, errInFile = valid;
	keletVars kv;
	kv.row = 0;
	kv.ic = MEMORY_START_ADDRESS;
	
	/*copy file name (for error messages)*/
	kv.file = malloc(strlen(file)+1);
	strcpy(kv.file,file);
	/*open input file*/
	if (getFile(&kv, &assembly) != valid)
		return invalid;
	/*open output files*/
	file = strtok(file,".");
	obj = getfilepointer(file,OBJECT_EXTENSION,"w");
	ent = getfilepointer(file,ENTRY_EXTENSION,"w");
	ext = getfilepointer(file,EXTERN_EXTENSION,"w");
	
	/* make sure the obj file title is needed*/
	if((icf != MEMORY_START_ADDRESS) && (dcf != 0))
		fprintf(obj,"     %lu %lu\n",icf - MEMORY_START_ADDRESS, dcf);
	
	while ((lineErr != eof) && (lineErr != emptyAndeof)) /*haven't reached the end of the file*/
	{
		lineErr = getCommandLine(assembly, &kv);/* get the label if there is any, and then the instruction or guidance, setting flags accordingly*/
		/*valid line and command*/
		if (lineErr == valid || lineErr == eof) /*if the line is valid and not empty, analyze it*/
		{	
			if(executeLine(symbolTable,&kv,obj,ext) != valid)
				errInFile = invalid;
		}
		if(lineErr == invalid)
			errInFile = invalid;
	}
	free(kv.file);
	/*writes data image to file after code image */
	writeDataToFile(dataImage,kv.ic,obj);
	createEntFile(ent,symbolTable);
	/* delete files if there was error or if the files are empty*/
	finalizeFiles(file,obj,ent,ext,errInFile);
	return errInFile;
}
/* encodes an instruction and adds it to the object file, if the instruction used an extern label print it to extern file.
   if there was an error in the encoding return invalid */
enum errors encode_instruction(symboltable *symbolTable, keletVars *kv,FILE *objfp, FILE *extfp)
{
	enum errors err = valid;
	uint32 result;
	/* executre the correct fucntion according to the instruction type*/
	switch(kv->in_typ)
	{
	case Ra:
		err = execute_Ra(kv);
		break;
	case Rc:
		err = execute_Rc(kv);
		break;
	case Ia:
		err = execute_Ia_ls(kv);
		break;
	case Ib:
		err = execute_Ib(symbolTable,kv);
		break;
	case Ils:
		err = execute_Ia_ls(kv);
		break;
	case Jj:
		err = execute_Jj(symbolTable,kv,extfp);
		break;
	case Jlc:
		err = execute_Jlc(symbolTable,kv,extfp);
		break;
	case Js:
		err = execute_Js(kv);
		break;
	}
	if(err == invalid)
		return invalid;
	/* call the correct instruction encode with the parameters */
	result = kv->cur_inst.f(kv->numbers[0], kv->numbers[1], kv->numbers[2], kv->cur_inst.opcode, kv->cur_inst.funct);
	
	printInstructionToObj(objfp, result, kv->ic);
	kv->ic += INSTRUCTION_BYTE_LEN;
	return valid;
}
/* deletes the file if it is empty or if there was an error */
void finalizeFile(char file[],FILE *fp,char extension[], enum errors errInFile)
{
	char *filename = addExtensionToFile(file,extension);
	if(errInFile == invalid)/*if there was an error delete the file*/
		remove(filename);
	else
	{
		/*reopen the file for reading*/
		fclose(fp);
		fp = getfilepointer(file,extension,"r");
		/*if the file is empty delete it*/
		if(fgetc(fp) == EOF)
			remove(filename);
	}
	
	fclose(fp);
	free(filename);
}
/* deletes all empty files and delete all the files if there was an error*/			
void finalizeFiles(char file[],FILE *obj, FILE *ent, FILE *ext, enum errors errInFile)
{
	finalizeFile(file,obj,OBJECT_EXTENSION,errInFile);
	finalizeFile(file,ent,ENTRY_EXTENSION,errInFile);
	finalizeFile(file,ext,EXTERN_EXTENSION,errInFile);
}








