#include "second_transition.h"
#include "instExecution.h"

/*executes the second_transition and creates output files */
enum errors second_transition(char file[],symboltable *symbolTable,dataimage *dataImage,uint32 icf, uint32 dcf)
{
	char *label;
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
		fprintf(obj,"     %lu %lu",icf - MEMORY_START_ADDRESS, dcf);
	
	while ((lineErr != eof) && (lineErr != emptyAndeof)) /*haven't reached the enf of the file*/
	{
		lineErr = getCommandLine(assembly, &kv);
		/*valid line and command*/
		if (lineErr == valid || lineErr == eof) 
		{	
			if(getCommandName(&kv) == valid)
			{
				if(kv.isEntry)
				{
					if(getArgument(&label,&kv) == valid)
					{
						if(doesSymbolExist(symbolTable,label))
						{
							if(!(getAttributes(symbolTable,label) & EXTERN))/*entry must not be extern too*/
								addAttributes(symbolTable,ENTRY,label);
							else
							{
								printf("file: %s, line: %d :illegal entry and extern on the same label",kv.file,kv.row);
								errInFile = invalid;
							}
						}
						else
						{
							printf("file: %s, line: %d :label does not exist\n",kv.file,kv.row);
							errInFile = invalid;
						}
					}
				}
				/* if there is an instruction, we encode it and if it isnt valid we mark it */
				else if(kv.isInstruction && (encode_instruction(symbolTable,&kv,obj,ext) != valid))
					errInFile = invalid;
			}
			else
				errInFile = invalid;
		}
		else if(lineErr == invalid)
			errInFile = invalid;
	}
	free(kv.file);
	writeDataToFile(dataImage,kv.ic,obj);
	createEntFile(ent,symbolTable);
	finalizeFiles(file,obj,ent,ext,errInFile);
	return errInFile;
}
enum errors encode_instruction(symboltable *symbolTable, keletVars *kv,FILE *objfp, FILE *extfp)
{
	enum errors err = valid;
	uint32 result;
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








