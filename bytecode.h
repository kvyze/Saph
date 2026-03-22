#pragma once

#include "ast.h"

typedef enum
{
	OP_PUSH,
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_NEG,
	OP_HALT
} Opcode;

typedef struct
{
	int* code;
	int size;
	int capacity;
} Bytecode;

Bytecode* bytecode_create(void);
void bytecode_destroy(Bytecode* bc);
void bytecode_add(Bytecode* bc, int op);
void bytecode_generate(Bytecode* bc, ASTNode* node);
void bytecode_print(Bytecode* bc);
