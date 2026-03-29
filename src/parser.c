#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parser.h"
#include "../include/merror.h"


Parser* parser_create(Lexer* lexer)
{
	Parser* parser = malloc(sizeof(Parser));
	merror(parser, "Parser")

	parser->lexer = lexer;
	parser->current_token = lexer_next_token(lexer);
	parser->error = 0;

	return parser;
}

void parser_destroy(Parser* parser)
{
	if (parser)
		free(parser);
}

void parser_advance(Parser* parser)
{
	parser->current_token = lexer_next_token(parser->lexer);

	if (parser->current_token.type == TOKEN_ERROR)
		parser->error = 1;
}

int parser_match(Parser* parser, Saph_TokenType type)
{
	return parser->current_token.type == type;
}

int parser_match_word(Parser* parser, const char* value)
{
	return parser->current_token.type == TOKEN_WORD && strcmp(parser->current_token.word, value) == 0;
}

void parser_consume(Parser* parser, Saph_TokenType expected, const char* error_message)
{
	if (parser->current_token.type == expected)
		parser_advance(parser);
	else
	{
		fprintf(stderr, "SyntaxError(%d, %d): %s", parser->current_token.line, parser->current_token.column, error_message);
		parser->error = 1;
		exit(EXIT_FAILURE);
	}
}

ASTNode* parse_program(Parser* parser)
{
	ASTNode* block = ast_block_create();

	while (!parser_match(parser, TOKEN_EOF) && !parser->error)
		ast_block_add(block, parse_statement(parser));

	return block;
}

ASTNode* parse_statement(Parser* parser)
{
	ASTNode* node = NULL;
	int line = parser->current_token.line;

	if (parser_match_word(parser, "print"))
	{
		parser_advance(parser);
		node = ast_print_stmt(parse_expression(parser), line);
	}
	else
		node = parse_expression(parser);

	return node;
}

ASTNode* parse_expression(Parser* parser)
{
	ASTNode* left = parse_term(parser);

	while (parser_match(parser, TOKEN_PLUS) || parser_match(parser, TOKEN_MINUS))
	{
		Saph_TokenType op = parser->current_token.type;
		int line = parser->current_token.line;

		parser_advance(parser);

		ASTNode* right = parse_term(parser);
		left = ast_binary_op(left, right, op, line);
	}

	return left;
}

ASTNode* parse_term(Parser* parser)
{
	ASTNode* left = parse_factor(parser);

	while (parser_match(parser, TOKEN_MUL) || parser_match(parser, TOKEN_DIV))
	{
		Saph_TokenType op = parser->current_token.type;
		int line = parser->current_token.line;

		parser_advance(parser);

		ASTNode* right = parse_factor(parser);
		left = ast_binary_op(left, right, op, line);
	}

	return left;
}

ASTNode* parse_factor(Parser* parser)
{
	ASTNode* node = NULL;

	if (parser_match(parser, TOKEN_NUMBER))
	{
		node = ast_number(parser->current_token.value, parser->current_token.line);
		parser_advance(parser);
	}
	else if (parser_match(parser, TOKEN_ORB))
	{
		parser_advance(parser);
		node = parse_expression(parser);
		parser_consume(parser, TOKEN_CRB, "')' expected.");
	}
	else if (parser_match(parser, TOKEN_MINUS))
	{
		int line = parser->current_token.line;
		parser_advance(parser);
		node = ast_unary_op(parse_factor(parser), line);
	}
	else
	{
		fprintf(stderr, "SyntaxError(%d, %d): found unexpected %s",
			parser->current_token.line, parser->current_token.column, token_type_to_string(parser->current_token.type));
		parser->error = 1;
		exit(EXIT_FAILURE);
	}

	return node;
}
