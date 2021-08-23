#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "constants.h"

/* converts the parameters of the R instruction into a 4 byte word */
uint32 inst_R(int rs, long rt, int rd, int opcode, int funct);
/* converts the parameters of the I instruction into a 4 byte word */
uint32 inst_I(int rs, long immed, int rt, int opcode, int funct);
/* converts the parameters of the J instruction into a 4 byte word */
uint32 inst_J(int reg, long address, int x,int opcode, int y);

#endif
