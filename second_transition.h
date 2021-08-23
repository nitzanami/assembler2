#ifndef SECOND_TRANSITION_H
#define SECOND_TRANSITION_H

#include "constants.h"
#include "symboltable.h"
#include "dataimage.h"
#include "kelet.h"
#include "files.h"
/*executes the second_transition and creates output files */
enum errors second_transition(char file[],symboltable *symbolTable,dataimage *dataImage,uint32 icf, uint32 dcf);
/* executes an instruction line, adding it to the correct output files if needed*/
enum errors encode_instruction(symboltable *symbolTable, keletVars *kv,FILE *objfp, FILE *extfp);
/* deletes the file if it is empty or if there was an error */
void finalizeFile(char file[],FILE *fp,char extension[], enum errors errInFile);
/* deletes all empty files and delete all the files if there was an error*/
void finalizeFiles(char file[],FILE *obj, FILE *ent, FILE *ext, enum errors errInFile);

#endif
