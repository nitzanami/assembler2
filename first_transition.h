#include "symboltable.h"
#include "kelet.h"
#include "dataimage.h"



/*this function gets a file name, opens it and extracts the symbols and data image*/
enum errors first_transition(char file[], symboltable *symbolTable, dataimage *dataImage,uint32 *icf, uint32 *dcf);
/*this function adds a guidance line to the data image*/
enum errors addGuidanceToData(dataimage *dataImage, keletVars *kv);
/*this function gets the set numbers from the command line and checks that they are all long numbers. alerts on errors if there are any*/
enum errors getSet(int *numberOfNums, keletVars *kv);
/*this function checks for correct numbers in the set*/
enum errors check_d_set(char **s, int bytesToDivide, int numberOfNums, keletVars *kv);
/*this function adds the label to the symbol table if it isnt already defined. if the label is defined it alerts about it properly*/
enum errors addLabel(symboltable *symbolTable, char* label, keletVars *kv);
