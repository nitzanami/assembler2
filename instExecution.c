#include "instExecution.h"

/* all the execute_<instruction> functions are responsible for getting the arguments for the instruction
   at the correct order, and report errors*/
   
/* gets the parameters for R_arithmatic: <register>,<register>,<register>*/
enum errors execute_Ra(keletVars *kv)
{
	int i;
	for (i = 0; i < R_ARI_VARS ; i++)/* get 3 registers*/
	{
		if (getReg(kv->numbers + i, kv) != valid)
			return invalid;
	}
	return valid;
}
/* gets the parameters for R_copy: <register>,<register>*/
enum errors execute_Rc(keletVars *kv)
{
	int i = 0;
	
	if (getReg(kv->numbers + (i++), kv) != valid)
		return invalid;
		
	kv->numbers[i++] = 0;/* rt = 0*/
	
	if (getReg(kv->numbers + (i++), kv) != valid)
		return invalid;
		
	
	return valid;
}
/* gets the parameters for I_arithmatic or I_load_store : <register>,<immed>,<register> */
enum errors execute_Ia_ls(keletVars *kv)
{
	int i = 0;
	long immed;
	
	if (getReg(kv->numbers+(i++), kv) != valid)
		return invalid;
		
	if(getImmed(&immed, kv) != valid)
		return invalid;
	else
	{
		kv->numbers[i++] = immed;
	}
		
	if (getReg((kv->numbers)+(i++), kv) != valid)
		return invalid;
		
	
	return valid;
}
/* gets the parameters for I_b : <register>,<register>,<label>
	the label must not be extern */
enum errors execute_Ib(symboltable *symbolTable, keletVars *kv)
{
	char *label;
	long distance;
	/* get 2 registers*/
	if (getReg(kv->numbers, kv) != valid)
			return invalid;
	if (getReg(kv->numbers + 2, kv) != valid)
			return invalid;
			
	if (getArgument(&label,kv) == valid)
	{
		if(doesSymbolExist(symbolTable,label))
		{
			if (getAttributes(symbolTable, label) & EXTERN)/*label for this instruction must not be extern */
			{
				printError("label for this instruction must not be extern");
				return invalid;
			}
			else
			{
				/*calculate the distance between the label and ic*/
				distance = getValue(symbolTable,label) - kv->ic;
				if(distance > MAX_IMMED || distance < MIN_IMMED)/*make sure the distance is legal*/
				{
					printError("immed value out of range, must fit in a 16 bit signed int");
					return invalid;
				}
				else
				{
					kv->numbers[1] = distance;
					
					return valid;
				}
			}
		}
		else
		{
			printError("this label does not exist");
			return invalid;
		}
	}
	return invalid;
}
/* gets the parameters for J_jump : <register> or <label> */
enum errors execute_Jj(symboltable *symbolTable, keletVars *kv, FILE *extfp)
{
	char *label;
	/*if the parameter starts with $ its a register, else its a label*/
	if(*kv->nextChar == '$')
	{
		if (getReg(kv->numbers + 1, kv) != valid)
			return invalid;
		kv->numbers[0] = 1;/*reg = 1*/
	}
	else if (getArgument(&label,kv) == valid)
	{
		kv->numbers[0] = 0;/*reg = 0*/
		if(doesSymbolExist(symbolTable,label))
		{
			kv->numbers[1] = getValue(symbolTable,label);
			if(getAttributes(symbolTable,label) & EXTERN)/*if the label is external print it to the extern file*/
				 printExternToFile(extfp,kv->ic,label);
		}
		else
		{
			printError("this label does not exist");
			return invalid;
		}
	}
		
	
	return valid;
}
/* gets the parameters for J_la or J_call : <label> */
enum errors execute_Jlc(symboltable *symbolTable, keletVars *kv, FILE *extfp)
{
	char *label;
	if (getArgument(&label,kv) == valid)
	{
		if(doesSymbolExist(symbolTable,label))
		{
			kv->numbers[0] = 0; /*reg = 0*/
			kv->numbers[1] = getValue(symbolTable,label);
			if(getAttributes(symbolTable,label) & EXTERN)/*if the label is external print it to the extern file*/
				 printExternToFile(extfp,kv->ic,label);
		}
	}
	
	return valid;
}
/* make sure there are no arguments for stop*/
enum errors execute_Js(keletVars *kv)
{
	kv->numbers[0] = kv->numbers[1] = kv->numbers[2] = 0;
	
	return valid;
}
/* gets the next register parameter, returns invalid if there is an error */
enum errors getReg(long *reg, keletVars *kv)
{
	char *n;
	if(getArgument(&n,kv) != valid)
		return invalid;
	if(n[0] != '\0')
	{
		if(n[0] != '$')
		{
			printError("a register must start with $");
			return invalid;
		}
		/*convert the register string to an integer and make sure the register value is valid*/
		if (my_atol(n+1, reg,kv) == valid && MIN_REG <= *reg && *reg <= MAX_REG)
			return valid;
		else
		{
			printError("invalid register number");
			return invalid;
		}
	}
	else
	{
		printError("missing arguments");
		return invalid;
	}
}
/* gets the next immed parameter, returns invalid if there is an error */
enum errors getImmed(long *immed,keletVars *kv)
{
	char *n;
	if(getArgument(&n,kv) != valid)
		return invalid;
	if(n[0] == '\0')
	{
		printError("missing arguments");
		return invalid;
	}
	if (my_atol(n,immed,kv) == valid)/*converts the immed from char[] to long*/
	{
		if (*immed <= MAX_IMMED && *immed >= MIN_IMMED)/*make sure the immed value is valid*/
			return valid;
		else
		{
			printError("invalid immediate value");
			return invalid;
		}
	}
	
	else
		return invalid;
}
