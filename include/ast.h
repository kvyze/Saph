#pragma once

#include "token.h"

typedef enum
{
	AST_NUMBER,
	AST_BINARY_OP,
	AST_UNARY_OP,
	AST_PRINT,
	AST_BLOCK
} ASTNodeType;

typedef struct ASTNode
{
	ASTNodeType type;
	int line;

	union {
		double number_value;

		struct {
			struct ASTNode* left;
			struct ASTNode* right;
			Saph_TokenType op;
		} binary_op;

		struct {
			struct ASTNode* operand;
		} unary_op;

		struct {
			struct ASTNode* expr;
		} print_stmt;

		struct {
			struct ASTNode** statements;
			int count;
			int capacity;
		} block;
	} data;
} ASTNode;

ASTNode* ast_alloc();
ASTNode* ast_number(double value, int line);
ASTNode* ast_binary_op(ASTNode* left, ASTNode* right, Saph_TokenType op, int line);
ASTNode* ast_unary_op(ASTNode* operand, int line);
ASTNode* ast_print_stmt(ASTNode* expr, int line);
ASTNode* ast_block_create(void);
void ast_block_add(ASTNode* block, ASTNode* stmt);

void ast_free(ASTNode* node);
void ast_print(ASTNode* node, int indent);
