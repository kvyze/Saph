#include <stdio.h>
#include <stdlib.h>

#include "parser.h"


Parser* parser_create(Lexer* lexer)
{
	Parser* parser = malloc(sizeof(Parser));

	if (!parser)
	{
		fprintf(stderr, "Parser memory allocation error.");
		exit(EXIT_FAILURE);
	}

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

int parser_match(Parser* parser, TokenType type)
{
	return parser->current_token.type == type;
}

void parser_consume(Parser* parser, TokenType expected, const char* error_message)
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

int parse_program(Parser* parser)
{
	int result = 0;

	while (!parser_match(parser, TOKEN_EOF) && !parser->error)
		result = parse_statement(parser);

	return result;
}

int parse_statement(Parser* parser)
{
	int result = parse_expression(parser);
	parser_consume(parser, TOKEN_SEMICOLON, "';' expected.");

	return result;
}

int parse_expression(Parser* parser)
{
	int left = parse_term(parser);

	while (parser_match(parser, TOKEN_PLUS) || parser_match(parser, TOKEN_MINUS))
	{
		TokenType op = parser->current_token.type;
		parser_advance(parser);

		int right = parse_term(parser);

		if (op == TOKEN_PLUS)
			left += right;
		else
			left -= right;
	}

	return left;
}

int parse_term(Parser* parser)
{
	int left = parse_factor(parser);

	while (parser_match(parser, TOKEN_MUL) || parser_match(parser, TOKEN_DIV))
	{
		TokenType op = parser->current_token.type;
		parser_advance(parser);

		int right = parse_factor(parser);

		if (op == TOKEN_MUL)
			left *= right;
		else
		{
			if (right == 0)
			{
				fprintf(stderr, "ZeroDivisionError(%d, %d): division by 0.", parser->current_token.line, parser->current_token.column);
				parser->error = 1;
				exit(EXIT_FAILURE);
			}
			left /= right;
		}
	}

	return left;
}

int parse_factor(Parser* parser)
{
	int result = 0;

	if (parser_match(parser, TOKEN_NUMBER))
	{
		result = parser->current_token.value;
		parser_advance(parser);
	}
	else if (parser_match(parser, TOKEN_ORB))
	{
		parser_advance(parser);
		result = parse_expression(parser);
		parser_consume(parser, TOKEN_CRB, "')' expected.");
	}
	else
	{
		fprintf(stderr, "SyntaxError(%d, %d): found unexpected %s",
			parser->current_token.line, parser->current_token.column, token_type_to_string(parser->current_token.type));
		parser->error = 1;
		exit(EXIT_FAILURE);
	}

	return result;
}
