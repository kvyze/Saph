#pragma once

#include "lexer.h"
#include "ast.h"

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

ASTNode* parse_program(Parser* parser);
ASTNode* parse_statement(Parser* parser);
ASTNode* parse_expression(Parser* parser);
ASTNode* parse_term(Parser* parser);
ASTNode* parse_factor(Parser* parser);
