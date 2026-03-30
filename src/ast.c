#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/ast.h"
#include "../include/merror.h"


ASTNode* ast_alloc()
{
	ASTNode* node = malloc(sizeof(ASTNode));
	merror(node, "AST")

	return node;
}

ASTNode* ast_number(double value, int line)
{
	ASTNode* node = ast_alloc();

	node->type = AST_NUMBER;
	node->line = line;
	node->data.number_value = value;

	return node;
}

ASTNode* ast_binary_op(ASTNode* left, ASTNode* right, Saph_TokenType op, int line)
{
	ASTNode* node = ast_alloc();

	node->type = AST_BINARY_OP;
	node->line = line;
	node->data.binary_op.left = left;
	node->data.binary_op.right = right;
	node->data.binary_op.op = op;

	return node;
}

ASTNode* ast_unary_op(ASTNode* operand, int line)
{
	ASTNode* node = ast_alloc();

	node->type = AST_UNARY_OP;
	node->line = line;
	node->data.unary_op.operand = operand;

	return node;
}

ASTNode* ast_print_stmt(ASTNode* expr, int line)
{
	ASTNode* node = ast_alloc();

	node->type = AST_PRINT;
	node->line = line;
	node->data.print_stmt.expr = expr;

	return node;
}

ASTNode* ast_block_create(void)
{
	ASTNode* node = ast_alloc();

	node->type = AST_BLOCK;
	node->line = 0;
	node->data.block.count = 0;
	node->data.block.capacity = 4;
	node->data.block.statements = malloc(node->data.block.capacity * sizeof(ASTNode*));
	merror(node->data.block.statements, "Statements")

	return node;
}

ASTNode* ast_variable(const char* name, int line)
{
	ASTNode* node = ast_alloc();

	node->type = AST_VARIABLE;
	node->line = line;
	strcpy(node->data.variable, name);

	return node;
}

ASTNode* ast_let_stmt(const char* name, ASTNode* value, int line)
{
	ASTNode* node = ast_alloc();

	node->type = AST_LET;
	node->line = line;
	strcpy(node->data.let_stmt.name, name);
	node->data.let_stmt.value = value;

	return node;
}

void ast_block_add(ASTNode* block, ASTNode* stmt)
{
	if (block->data.block.count >= block->data.block.capacity)
	{
		block->data.block.capacity *= 2;
		ASTNode** statements = realloc(block->data.block.statements, block->data.block.capacity * sizeof(ASTNode*));
		merror(statements, "Statements (realloc)")
		block->data.block.statements = statements;
	}

	block->data.block.statements[block->data.block.count++] = stmt;
}

void ast_free(ASTNode* node)
{
	if (!node) return;

	switch (node->type)
	{
		case AST_NUMBER: break;
		case AST_VARIABLE: break;
		case AST_BINARY_OP:
			ast_free(node->data.binary_op.left);
			ast_free(node->data.binary_op.right);
			break;
		case AST_UNARY_OP:
			ast_free(node->data.unary_op.operand);
			break;
		case AST_PRINT:
			ast_free(node->data.print_stmt.expr);
			break;
		case AST_BLOCK:
			for (int i = 0; i < node->data.block.count; i++)
				ast_free(node->data.block.statements[i]);
			free(node->data.block.statements);
			break;
		case AST_LET:
			ast_free(node->data.let_stmt.value);
			break;
	}

	free(node);
}

void ast_print(ASTNode* node, int indent)
{
	for (int i = 0; i < indent; i++)
		printf("\t");

	switch (node->type)
	{
		case AST_NUMBER:
			printf("NUMBER(%lf) [line:%d]\n", node->data.number_value, node->line);
			break;
			
		case AST_BINARY_OP:
		{
			const char* op = "";

			switch (node->data.binary_op.op)
			{
				case TOKEN_PLUS:	op = "+"; break;
				case TOKEN_MINUS:	op = "-"; break;
				case TOKEN_MUL:		op = "*"; break;
				case TOKEN_DIV:		op = "/"; break;
			}

			printf("BINARY_OP(%s) [line:%d]\n", op, node->line);
			ast_print(node->data.binary_op.left, indent + 1);
			ast_print(node->data.binary_op.right, indent + 1);
			break;
		}

		case AST_UNARY_OP:
			printf("UNARY_OP(-) [line:%d]\n", node->line);
			ast_print(node->data.unary_op.operand, indent + 1);
			break;

		case AST_PRINT:
			printf("PRINT [line:%d]\n", node->line);
			ast_print(node->data.print_stmt.expr, indent + 1);
			break;

		case AST_BLOCK:
			printf("BLOCK\n");
			for (int i = 0; i < node->data.block.count; i++)
				ast_print(node->data.block.statements[i], indent + 1);
			break;

		case AST_VARIABLE:
			printf("VARIABLE(%s) [line:%d]\n", node->data.variable, node->line);
			break;

		case AST_LET:
			printf("LET(%s) [line:%d]\n", node->data.let_stmt.name, node->line);
			ast_print(node->data.let_stmt.value, indent + 1);
			break;
	}
}
