#include <stdio.h>
#include <stdlib.h>

#include "../include/bytecode.h"
#include "../include/merror.h"


Bytecode* bytecode_create(void)
{
	Bytecode* bc = malloc(sizeof(Bytecode));
	merror(bc, "Bytecode")

	bc->capacity = 16;
	bc->size = 0;
	bc->code = malloc(bc->capacity * sizeof(int));

	merror(bc->code, "Bytecode instructions")

	return bc;
}

void bytecode_destroy(Bytecode* bc)
{
	if (bc)
	{
		if (bc->code) free(bc->code);
		free(bc);
	}
}

void bytecode_add(Bytecode* bc, int op)
{
	if (bc->size >= bc->capacity)
	{
		bc->capacity *= 2;
		int* code = realloc(bc->code, bc->capacity * sizeof(int));
		merror(code, "Bytecode instructions (realloc)")
		bc->code = code;
	}

	bc->code[bc->size++] = op;
}

void bytecode_generate(Bytecode* bc, ASTNode* node)
{
	switch (node->type)
	{
		case AST_NUMBER:
			bytecode_add(bc, OP_PUSH);
			bytecode_add(bc, node->data.number_value);
			break;

		case AST_BINARY_OP:
			bytecode_generate(bc, node->data.binary_op.left);
			bytecode_generate(bc, node->data.binary_op.right);

			switch (node->data.binary_op.op)
			{
				case TOKEN_PLUS:	bytecode_add(bc, OP_ADD); break;
				case TOKEN_MINUS:	bytecode_add(bc, OP_SUB); break;
				case TOKEN_MUL:		bytecode_add(bc, OP_MUL); break;
				case TOKEN_DIV:		bytecode_add(bc, OP_DIV); break;
				default:			error("Error: unknown operator (bytecode generation).")
			}
			break;

		case AST_UNARY_OP:
			bytecode_generate(bc, node->data.unary_op.operand);
			bytecode_add(bc, OP_NEG);
			break;

		default:
			error("Error: unknown AST node type (bytecode generation).")
	}
}

void bytecode_print(Bytecode* bc)
{
	for (int i = 0; i < bc->size; i++)
	{
		int opcode = bc->code[i];

		switch (opcode)
		{
			case OP_PUSH:	printf("PUSH %d\n", bc->code[++i]); break;
			case OP_ADD:	printf("ADD\n"); break;
			case OP_SUB:	printf("SUB\n"); break;
			case OP_MUL:	printf("MUL\n"); break;
			case OP_DIV:	printf("DIV\n"); break;
			case OP_NEG:	printf("NEG\n"); break;
			case OP_HALT:	printf("HALT\n"); break;
			default:		printf("UNKNOWN(%d)\n", opcode); break;
		}
	}
}
