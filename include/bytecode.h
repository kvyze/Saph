#pragma once

#include "ast.h"
#include "opcode.h"

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
