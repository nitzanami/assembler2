#ifndef INST_EXECUTION_H
#define INST_EXECUTION_H


#include "instructions.h"
#include "kelet.h"
#include "constants.h"
#include "symboltable.h"
#include "files.h"

#define checkExtraArgs() if (*(kv->nextChar) != '\0')\
				{\
					printError("too many arguments");\
					return invalid;\
				}


/* gets the parameters for R_arithmatic: <register>,<register>,<register>*/
enum errors execute_Ra(keletVars *kv);
/* gets the parameters for R_copy: <register>,<register>*/
enum errors execute_Rc(keletVars *kv);
/* gets the parameters for I_arithmatic or I_load_store : <register>,<immed>,<register> */
enum errors execute_Ia_ls(keletVars *kv);
/* gets the parameters for I_b : <register>,<register>,<label>
	the label must not be extern */
enum errors execute_Ib(symboltable *symbolTable, keletVars *kv);
/* gets the parameters for J_jump : <register> or <label> */
enum errors execute_Jj(symboltable *symbolTable, keletVars *kv, FILE *extfp);
/* gets the parameters for J_la or J_call : <label> */
enum errors execute_Jlc(symboltable *symbolTable, keletVars *kv, FILE *extfp);
/* make sure there are no arguments for stop*/
enum errors execute_Js(keletVars *kv);
/* gets the next register parameter, returns invalid if there is an error */
enum errors getReg(long *reg, keletVars *kv);
/* gets the next immed parameter, returns invalid if there is an error */
enum errors getImmed(long *immed,keletVars *kv);
#endif
