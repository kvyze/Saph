#pragma once

#include "token.h"

typedef enum
{
	AST_NUMBER,
	AST_BINARY_OP,
	AST_UNARY_OP
} ASTNodeType;

typedef struct ASTNode
{
	ASTNodeType type;
	int line;

	union {
		int number_value;

		struct {
			struct ASTNode* left;
			struct ASTNode* right;
			TokenType op;
		} binary_op;

		struct {
			struct ASTNode* operand;
		} unary_op;
	} data;
} ASTNode;

ASTNode* ast_alloc();
ASTNode* ast_number(int value, int line);
ASTNode* ast_binary_op(ASTNode* left, ASTNode* right, TokenType op, int line);
ASTNode* ast_unary_op(ASTNode* operand, int line);

int ast_eval(ASTNode* node);
void ast_free(ASTNode* node);
void ast_print(ASTNode* node, int indent);
