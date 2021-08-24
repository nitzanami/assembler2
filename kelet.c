#include "kelet.h"
#include <errno.h>

#define removeWhitespaces() while(isspace(c = *(kv->nextChar))) kv->nextChar++
extern instruction instructionsArr[];

extern int errno;

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
			printError("label instead of instruction or guidance");/*after the first label there is another label*/
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
	/* store the end of the argument*/
	argEnd = kv->nextChar;
	removeWhitespaces();
	 /*if there is a comma after an argument, make sure there is another argument after it*/
	if((c = *kv->nextChar) == ',')
	{
		kv->nextChar++;
		removeWhitespaces();
		if((c = *kv->nextChar) == '\0' || c == ',')
		{
			printError("extra comma");
			return invalid;
		}
	}/*if there is no comma after an argument, and we have not reached the end of the line, there is a missing comma*/
	else if(c != '\0')
	{
		printError("missing comma");
		return invalid;
	}
	/*null-terminate the argument*/
	*argEnd = '\0';
	return valid;
}
/*this function gets the string inside the quotation marks for asciz lines, alerts on errors if occoured*/
enum errors getAscizArgument(char** arg,keletVars *kv)
{	
	char c;
	if(*kv->nextChar++ != '\"')/*find starting " of string*/
	{
		printError("missing starting \" in asciz string");
		return invalid;
	}
	else
	{
		*arg = kv->nextChar;
		while(*kv->nextChar != '\"' && *kv->nextChar != '\0')/* looks for ending " or end of line*/
			kv->nextChar++;
		if(*kv->nextChar == '\"')/* if the string is terminated correctly, make sure there are no extra characters after it*/
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
		else/* string is not teminated correctly - missing ending "*/
		{
			printError("missing ending \" in asciz string");
			return invalid;
		}
	}
}
/*this function gets the optinonal label and the command from the line and checks for correct name. alerts on errors if there are any*/
enum errors getCommandName(keletVars *kv)
{
	enum errors e;
	char* word;
	e = getWord(&word,kv);
	if(e == valid && kv->isLabel)/*if the first word is a label*/
	{
		kv->label = word;/* store the label and read another word*/
		e = getWord(&word,kv);
	}
	if(e == valid)
	{	
		/* store the command, if it is empty report error*/
		kv->cmd = word;
		if(word[0] == '\0')
		{
			printError("missing instruction or guidance");
			return invalid;
		}
		/* if command starts with "." check for guidance*/
		if(word[0] == '.')
			if(isGuidance(word,kv))
				kv->isInstruction = 0;
			else
			{
				printError("illegal guidance");
				return invalid;
			}
		else if(isInstruction(word,kv))/*if the command is a valid instuction raise a flag*/
			kv->isInstruction = 1;
		else /*the command is not a valid instruction or guidance*/
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
	if(word[strlen(word)-1] == ':') /*labels end with :*/
	{
		word[strlen(word)-1] = '\0';
		return 1;
	}
	return 0;
}
/*this function checks if the label is legal*/
int isLabelLegal(char *label, keletVars *kv)
{	
	/* if the label is empty, there is a missing label */
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
	/* check if label starts with letter, is alphanum and not a keyword*/
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
 returns 1 if its a guidance, else returns 0*/
int isGuidance(char* word,keletVars *kv)
{
	/*if cmd matches one of them return 1*/
	if(strcmp(word,".dw") == 0) 
		return 1;
	if(strcmp(word,".dh") == 0) 
		return 1;
	if(strcmp(word,".db") == 0) 
		return 1;
	if(strcmp(word,".asciz") ==0)	
		return 1;
	/* if cmd is entry or extern raise flag*/
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
	/*make sure w is not an empty string*/
	if(w[0] == '\0')
		return invalid;
	/* convert w to long*/
	*result = strtol(w, &str, 10);
	if(strlen(str) > 0)/* there are invalid characters in the number */
	{
		printError("parameter is not an integer");
		return invalid;
	}
	if (errno > 0) /*the number cant fit in a long*/
	{
		printError("number is too big to fit in long");
		return invalid;
	}
	return valid;
}


