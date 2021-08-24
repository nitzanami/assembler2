#include "kelet.h"
#include <errno.h>

#define removeWhitespaces() while(isspace(c = *(kv->nextChar))) kv->nextChar++
extern instruction instructionsArr[];

extern int errno;

/*this function makes sure the file name is valid and if it is, opens the file to the pointer it gets*/
enum errors getFile(keletVars *kv, FILE **fp)
{
	int i = 0;
	char *suffix;
	
	
	while (kv->file[i++] != '.');
	suffix = kv->file + i - 1;
	if (strcmp(suffix, ASSEMBLY_EXTENSION) == 0) /*a valid file for the assembler*/
	{
		if ((*fp = fopen(kv->file, "r")) == NULL)
		{
			printf("can't open file:%s\n", kv->file);
			return invalid;
		}
		return valid;
	}
		
	printf("file %s: illegal file name for assembler\n", kv->file);
	return invalid;
}


/*this function gets the command line and checks for correct length. reports about problems if there are any*/
enum errors getCommandLine(FILE *fp, keletVars *kv)
{
	int i = 0;
	char c;
	enum errors e = valid;
	
	kv->charsChecked = kv->isLabel = kv->isInstruction = kv->isExtern = kv->isEntry = kv->lineLength = 0;
	kv->cmd = kv->label = NULL;	
	kv->row++;
	while (isspace(c = getc(fp)) && (c != '\n')); /*ignore white spaces in the beginnig of the line*/
	if (c == ';') /*comment line*/
		while((c = getc(fp)) != '\n');
	if(c == '\n')
		return emptyLine;
	while (c != EOF && c != '\n') /*end of row or file*/
	{
		if (i < MAX_INPUT_LINE)
		{
			kv->line[i++] = c;
		}
		else
		{
			if (!isspace(c)) /*character after maximum allowed characters, line too long*/
				e = invalid;
		}
		c = getc(fp);
	}

	if (e == invalid)
	{
		printError("line too long");
		return invalid;
	}
	kv->line[kv->lineLength = i] = '\0';
	kv->nextChar = kv->line;
	if (c == EOF) /*end of current file*/
	{
		if(kv->lineLength == 0)
			return emptyAndeof;
		else
			return eof;
	}
	
	return e;
}
/*this function gets the next word (instruction,guidance or label) from the line,alerts on errors if there are any*/
enum errors getWord(char** word, keletVars *kv)
{
	char c, *wordEnd;
	*word = kv->nextChar;/* store the start of the word */
	/* find the end of the word*/
	while(!isspace(c = *(kv->nextChar)) && c!= ',' && c != '\n' && c != ':' && c != '\0')
		kv->nextChar++;
	/* mark the end of the word */
	wordEnd = kv->nextChar;
	if(c == ':')/*if word ends with : it is a label*/
	{
		if(kv->isLabel)/*if we found a label already*/
		{
			printError("label instead of instruction");/*after the first label there is another label*/
			return invalid;
		}	
		kv->isLabel = 1;
	}	
	if(*kv->nextChar != '\0')
		kv->nextChar++;
	/* remove extra white spaces after word if there are any*/
	removeWhitespaces();
	/* a comma after the word is illegal */
	if(c == ',')
	{
		printError("Extra comma before arguments");
		return invalid;
	}
	/* null-terminate the word */
	*(wordEnd) = '\0';
	
	removeWhitespaces();
	return valid;
}
/*this function gets the next argument from the line into the arg parameter, and prints an error message if an error occoured
 returns valid if there were no errors, else invalid*/
enum errors getArgument(char** arg,keletVars *kv)
{
	char c,*argEnd;
	*arg = kv->nextChar;/* store the start of the word */
	while(!isspace(c = *kv->nextChar) && c != ',' && c!= '\0')/* find the end of the word*/
		kv->nextChar++;
		
	argEnd = kv->nextChar;
	removeWhitespaces();
	
	if((c = *kv->nextChar) == ',')
	{
		kv->nextChar++;
		removeWhitespaces();
		if((c = *kv->nextChar) == '\0' || c == ',')
		{
			printError("extra comma");
			return invalid;
		}
	}
	else if(c != '\0')
	{
		printError("missing comma");
		return invalid;
	}
	*argEnd = '\0';
	return valid;
}
/*this function gets the string inside the quotation marks for asciz lines, alerts on errors if occoured*/
enum errors getAscizArgument(char** arg,keletVars *kv)
{	
	char c;
	if(*kv->nextChar++ != '\"')
	{
		printError("missing starting \" in asciz string");
		return invalid;
	}
	else
	{
		*arg = kv->nextChar;
		while(*kv->nextChar != '\"' && *kv->nextChar != '\0')
			kv->nextChar++;
		if(*kv->nextChar == '\"')
		{
			*kv->nextChar++ = '\0';
			removeWhitespaces();
			if(c != '\0')
			{
				printError("extra characters after closing \"");
				return invalid;
			}
			else
				return valid;
		}
		else
		{
			printError("missing ending \" in asciz string");
			return invalid;
		}
	}
}
/*this function gets the command from the command line and checks for correct name. alerts on errors if there are any*/
enum errors getCommandName(keletVars *kv)
{
	enum errors e;
	char* word;
	e = getWord(&word,kv);
	if(e == valid && kv->isLabel)
	{
		kv->label = word;
		e = getWord(&word,kv);
	}
	if(e == valid)
	{
		kv->cmd = word;
		if(word[0] == '\0')
		{
			printError("missing instruction or guidance");
			return invalid;
		}
		if(word[0] == '.')
			if(isGuidance(word,kv))
				kv->isInstruction = 0;
			else
			{
				printError("illegal guidance");
				return invalid;
			}
		else if(isInstruction(word,kv))
			kv->isInstruction = 1;
		else 
		{
			printError("illegal instruction");
			return invalid;
		}
	}	
	return valid;
}
/*this function checks that the label starts with a letter and consists of letters and numbers only*/
enum errors checkAlphaNum(char *label, keletVars *kv)
{		
	if (!isalpha(label[0])) /*label sdould start with a letter*/
	{
		printError("label doesn't start with a letter");
		return invalid;
	}
	while (isalnum(*label))/*label consists of letters and numbers only*/
		label++;
	
	if (*label != '\0')
	{
		printError("label consists of letters and numbers only");
		return invalid;
	}
	return valid;
}
/*this function checks that the label isnt a key word*/
enum errors checkKeyWord(char* label,keletVars *kv)
{
	int i;
	for(i = 0 ; i < NUMBER_OF_INSTRUCTIONS ; i++) /*check if the label is an instruction key word*/
		if (strcmp(label, instructionsArr[i].name) == 0)
		{
			printError("label can't be an instruction key word");
			return invalid;
		}
	/*check if the label is a guidance key word
	if label matches one of them the total value will be 0*/
	if ((strcmp(label,"dw") && strcmp(label,"dh") && strcmp(label,"db") && strcmp(label,"asciz") && 
		strcmp(label,"entry") && strcmp(label,"extern")) == 0)
	{
		printError("label can't be a guidance key word");
		return invalid;
	}
	return valid;
}

/*this function checks if the word is a label*/
int isLabel(char *word, keletVars *kv)
{
	if(word[strlen(word)-1] == ':')
	{
		word[strlen(word)-1] = '\0';
		return 1;
	}
	return 0;
}
/*this function checks if the label us legal*/
int isLabelLegal(char *label, keletVars *kv)
{	
	if(label[0] == '\0')
	{
		printError("missing label");
		return 0;
	}
	if(strlen(label) > MAX_SYMBOL_LENGTH) /*label too long*/
	{
		printError("label too long");
		return 0;
	}
	if(checkAlphaNum(label,kv) == invalid || checkKeyWord(label,kv) == invalid)
		return 0;
	return 1;
}
/*this function checks if the word is an instruction, if it is one then it saves the type and the pointer to it and returns valid,
 else retruns that its illegal*/
int isInstruction(char* word,keletVars *kv)
{
	int i;
	for(i = 0 ; i < NUMBER_OF_INSTRUCTIONS ; i++)
	{
		if (strcmp(word, instructionsArr[i].name) == 0) /*cmd matches this instruction*/
		{
			kv->in_typ = instructionsArr[i].type; /*saves the instruction type*/
			kv->cur_inst = instructionsArr[i]; /*saves the pointer to the instruction*/
			return 1;
		}
	}
	/*cmd mathces no instruction*/
	return 0;
}

/*this function checks if the word is a guidance, if it is extern or entry it activates the proper flag
 returns true if its a guidance, else returns 0*/
int isGuidance(char* word,keletVars *kv)
{
	/*if cmd matches one of them the total value will be 0*/
	if ((strcmp(word, ".dw") && strcmp(word, ".dh") && strcmp(word, ".db") && strcmp(word, ".asciz")) == 0)
		return 1;
	if ((strcmp(word, ".entry") == 0)) 
	{	
		kv->isEntry = 1;
		return 1;
	}
	if(strcmp(word, ".extern") == 0) 
	{
		kv->isExtern = 1;
		return 1;
	}
	return 0; 
}

/*converts a string to a long number. returns valid if the string is a long number and invalid otherwise*/
enum errors my_atol(char w[], long *result,keletVars *kv)
{
	char *str;
	errno = 0;
	
	if(w[0] == '\0')
		return invalid;
	*result = strtol(w, &str, 10);
	if(strlen(str) > 0)
	{
		printError("parameter is not an integer");
		return invalid;
	}
	if (errno > 0) /*the number is bigger than long*/
	{
		printError("number is too big to fit in long");
		return invalid;
	}
	return valid;
}


