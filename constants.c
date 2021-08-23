#include "constants.h"
#include "instructions.h"
const instruction instructionsArr[]={
			{Ra,"add",1,0,inst_R},
			{Ra,"sub",2,0,inst_R},
			{Ra,"and",3,0,inst_R},
			{Ra,"or",4,0,inst_R},
			{Ra,"nor",5,0,inst_R},
			{Rc,"move",1,1,inst_R},
			{Rc,"mvhi",2,1,inst_R},
			{Rc,"mvlo",3,1,inst_R},
		
			{Ia,"addi",0,10,inst_I},
			{Ia,"subi",0,11,inst_I},
			{Ia,"andi",0,12,inst_I},
			{Ia,"ori",0,13,inst_I},
			{Ia,"nori",0,14,inst_I},
			{Ib,"bne",0,15,inst_I},
			{Ib,"beq",0,16,inst_I},
			{Ib,"blt",0,17,inst_I},
			{Ib,"bgt",0,18,inst_I},
			{Ils,"lb",0,19,inst_I},
			{Ils,"sb",0,20,inst_I},
			{Ils,"lw",0,21,inst_I},
			{Ils,"sw",0,22,inst_I},
			{Ils,"lh",0,23,inst_I},
			{Ils,"sh",0,24,inst_I},
			
			{Jj,"jmp",0,30,inst_J},
			{Jlc,"la",0,31,inst_J},
			{Jlc,"call",0,32,inst_J},
			{Js,"stop",0,63,inst_J},
			
		};
