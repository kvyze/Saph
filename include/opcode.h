#pragma once

typedef enum
{
	OP_PUSH,
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_NEG,
	OP_HALT,
	OP_PRINT,
	OP_LOAD,
	OP_STORE
} Opcode;
