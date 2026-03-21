#pragma once

#include "lexer.h"

typedef struct
{
	Lexer* lexer;
	Token current_token;
	int error;
} Parser;

Parser* parser_create(Lexer* lexer);
void parser_destroy(Parser* parser);
void parser_advance(Parser* parser);
int parser_match(Parser* parser, TokenType type);
void parser_consume(Parser* parser, TokenType expected, const char* error_message);

int parse_program(Parser* parser);
int parse_statement(Parser* parser);
int parse_expression(Parser* parser);
int parse_term(Parser* parser);
int parse_factor(Parser* parser);
