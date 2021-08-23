#include "instructions.h"

/* converts the parameters of the R instruction into a 4 byte word */
uint32 inst_R(int rs, long rt, int rd, int opcode, int funct)
{
	uint32 code = 0;
	
	/* add the parameters to the result at the correct location */
	code |= funct << FUNCT_START;
	code |= rd << RD_START;
	code |= rt << RT_START;
	code |= rs << RS_START;
	code |= opcode << OPCODE_START;
	
	return code;
}

/* converts the parameters of the I instruction into a 4 byte word */
uint32 inst_I(int rs, long immed, int rt, int opcode, int funct)
{
	uint32 code = 0;
	
	/* add the parameters to the result at the correct location */
	code |= immed & 0XFFFF;
	code |= rt << RT_START;
	code |= rs << RS_START;
	code |= opcode << OPCODE_START;

	return code;
}
/* converts the parameters of the J instruction into a 4 byte word */
uint32 inst_J(int reg, long address, int x,int opcode, int y)
{
	uint32 code = 0;
	
	code |= address & 0XFFFFFF;
	if (reg)/* if the instruction is a register, turn the reg bit on*/
		code |= 1 << REG_START;
	/* add opcode at the correct location */
	code |= opcode << OPCODE_START;
	
	return code;
}
