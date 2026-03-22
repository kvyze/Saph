#include <stdio.h>
#include <stdlib.h>

#include "ast.h"


ASTNode* ast_alloc()
{
	ASTNode* node = malloc(sizeof(ASTNode));

	if (!node)
	{
		fprintf(stderr, "AST memory allocation error.");
		exit(EXIT_FAILURE);
	}

	return node;
}

ASTNode* ast_number(int value, int line)
{
	ASTNode* node = ast_alloc();

	node->type = AST_NUMBER;
	node->line = line;
	node->data.number_value = value;

	return node;
}

ASTNode* ast_binary_op(ASTNode* left, ASTNode* right, TokenType op, int line)
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

int ast_eval(ASTNode* node)
{
	switch (node->type)
	{
		case AST_NUMBER:
			return node->data.number_value;

		case AST_BINARY_OP:
		{
			int left = ast_eval(node->data.binary_op.left);
			int right = ast_eval(node->data.binary_op.right);

			switch (node->data.binary_op.op)
			{
				case TOKEN_PLUS:
					return left + right;
				case TOKEN_MINUS:
					return left - right;
				case TOKEN_MUL:
					return left * right;
				case TOKEN_DIV:
					if (right == 0)
					{
						fprintf(stderr, "ZeroDivisionError(%d): division by 0.", node->line);
						exit(1);
					}
					return left / right;
				default:
					fprintf(stderr, "Error(%d): unknown operator.", node->line);
					exit(1);
			}
		}

		case AST_UNARY_OP:
			return -ast_eval(node->data.unary_op.operand);

		default:
			fprintf(stderr, "Error(%d): unknown AST node type.", node->line);
			exit(1);
	}
}

void ast_free(ASTNode* node)
{
	if (!node) return;

	switch (node->type)
	{
		case AST_NUMBER: break;
		case AST_BINARY_OP:
			ast_free(node->data.binary_op.left);
			ast_free(node->data.binary_op.right);
			break;
		case AST_UNARY_OP:
			ast_free(node->data.unary_op.operand);
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
			printf("NUMBER(%d) [line:%d]\n", node->data.number_value, node->line);
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
	}
}
