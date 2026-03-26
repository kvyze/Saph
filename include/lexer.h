#pragma once

#include "token.h"

typedef struct
{
	const char* input;
	int pos;
	int line;
	int column;
	int current_char;
} Lexer;

Lexer* lexer_create(const char* input);
void lexer_destroy(Lexer* lexer);
void lexer_advance(Lexer* lexer);
void lexer_skip_whitespace(Lexer* lexer);
Token lexer_read_number(Lexer* lexer);
Token lexer_next_token(Lexer* lexer);
