#pragma once

#include <stdint.h>

#include "ast.h"
#include "opcode.h"

typedef struct
{
	uint8_t* code;
	int size;
	int capacity;

	double* constants;
	int const_size;
	int const_capacity;
} Bytecode;

Bytecode* bytecode_create(void);
void bytecode_destroy(Bytecode* bc);
void bytecode_add(Bytecode* bc, uint8_t op);
int bytecode_add_constant(Bytecode* bc, double value);
void bytecode_generate(Bytecode* bc, ASTNode* node);
void bytecode_print(Bytecode* bc);
