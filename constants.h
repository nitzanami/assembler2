#ifndef CONSTANTS_H
#define CONSTANTS_H

/*general constants*/
#define MALLOC_FAILURE 1
#define checkMalloc(pointer) if((pointer) == NULL) exit(MALLOC_FAILURE)
#define printError(message) printf("file %s: line %d: %s\n",kv->file, kv->row,message)

#define INSTRUCTION_BYTE_LEN 4
#define VALID 0
#define INVALID 1
#define MAX_REGISTER_ADDRESS 31
#define MACHINE_CODE_SIZE (1<<25)
#define MEMORY_START_ADDRESS 100
#define BYTE_MASK 0xff
#define MAX_INPUT_LINE 80
#define BIT_IN_BYTE 8
#define DW_BYTES 4
#define DH_BYTES 2
#define DB_BYTES 1
#define MAX_REG 31
#define MIN_REG 0
#define MAX_IMMED 32767
#define MIN_IMMED -32768
typedef unsigned long int uint32;
enum errors {valid, eof, emptyLine, invalid};

/* instruction constants and typedefs */
enum inst_type {Ra, Rc, Ia, Ib, Ils, Jj, Jlc,Js};

#define FUNCT_START 6;
#define RD_START 11;
#define RT_START 16;
#define RS_START 21;
#define OPCODE_START 26;
#define REG_START 25

typedef struct instruction{
	enum inst_type type;
	char *name;
	int funct;
	int opcode;
	uint32 (*f)(int, long, int, int, int);
} instruction;

#define NUMBER_OF_INSTRUCTIONS 27

/*symbol related constants*/
#define MAX_SYMBOL_LENGTH 31
#define EXTERN 1
#define ENTRY 1<<1
#define CODE 1<<3
#define DATA 1<<4

/* file extensions */
#define ASSEMBLY_EXTENSION ".as"
#define OBJECT_EXTENSION ".ob"
#define EXTERN_EXTENSION ".ext"
#define ENTRY_EXTENSION ".ent"

#define OBJ_LINE_LENGTH 4


#endif
