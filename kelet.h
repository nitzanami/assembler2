#ifndef KELET_H
#define KELET_H

#include "instructions.h"
#include "constants.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


#define R_ARI_VARS 3
#define R_CPY_VARS 2

typedef struct keletVars{
	char line[MAX_INPUT_LINE + 1];
	char *cmd, *label, *file, *nextChar;
	int charsChecked, row, isLabel, isInstruction, isExtern, isEntry, lineLength;
	instruction cur_inst;
	enum inst_type in_typ;
	long numbers[MAX_INPUT_LINE];
	unsigned long dc, ic;
}keletVars;

/*this function makes sure the file name is valid and if it is, opens the file to the pointer it gets*/
enum errors getFile(keletVars *kv, FILE **fp);

/*this function gets the command line and checks for correct length. if there is EOF it reports about it*/
enum errors getCommandLine(FILE *fp, keletVars *kv);

/*this function gets the next word (instruction,guidance or label) from the line,alerts on errors if there are any*/
enum errors getWord(char** word, keletVars *kv);

/*this function gets the next argument from the line into the arg parameter, and prints an error message if an error occoured
 returns valid if there were no errors, else invalid*/
enum errors getArgument(char** arg,keletVars *kv);

/*this function gets the string inside the quotation marks for asciz lines, alerts on errors if occoured*/
enum errors getAscizArgument(char** arg,keletVars *kv);

/*this function gets the command from the command line and checks for correct name. alerts on errors if there are any*/
enum errors getCommandName(keletVars *kv);

/*this function checks if the word is a label*/
int isLabel(char *word, keletVars *kv);

/*this function checks if the label us legal*/
int isLabelLegal(char *label, keletVars *kv);

/*this function checks if the command is an instruction, if it is one then it saves the type and the pointer to it and returns 1,
 else retruns 0*/
int isInstruction(char *word,keletVars *kv);

/*this function checks if the command is a guidance, if it is extern or entry it activates the proper flag
 returns 1 if its a guidance, else returns 0*/
int isGuidance(char *word,keletVars *kv);

/*converts a string to a long number. returns valid if the string is a long number and invalid otherwise*/
enum errors my_atol(char w[], long *result);
#endif
