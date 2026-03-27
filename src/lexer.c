#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/lexer.h"
#include "../include/merror.h"


Lexer* lexer_create(const char* input)
{
	Lexer* lexer = malloc(sizeof(Lexer));
	merror(lexer, "Lexer")

	lexer->input = input;
	lexer->pos = 0;
	lexer->line = 1;
	lexer->column = 1;
	lexer->current_char = input[0];

	return lexer;
}

void lexer_destroy(Lexer* lexer)
{
	if (lexer)
		free(lexer);
}

void lexer_advance(Lexer* lexer)
{
	if (lexer->current_char != '\0')
	{
		if (lexer->current_char == '\n')
		{
			lexer->line++;
			lexer->column = 1;
		}
		else
			lexer->column++;

		lexer->current_char = lexer->input[++lexer->pos];
	}
}

void lexer_skip_whitespace(Lexer* lexer)
{
	while (lexer->current_char != '\0' &&
		  (lexer->current_char == ' ' || lexer->current_char == '\n' ||
		   lexer->current_char == '\t' || lexer->current_char == '\r'))
	{
		lexer_advance(lexer);
	}
}

Token lexer_read_number(Lexer* lexer)
{
	Token token;

	token.type = TOKEN_NUMBER;
	token.line = lexer->line;
	token.column = lexer->column;
	token.value = 0;

	while (lexer->current_char != '\0' && isdigit(lexer->current_char))
	{
		token.value = token.value * 10 + (lexer->current_char - '0');
		lexer_advance(lexer);
	}

	if (lexer->current_char == '.')
	{
		lexer_advance(lexer);
		double fraction = 1;

		while (lexer->current_char != '\0' && isdigit(lexer->current_char))
		{
			fraction /= 10;
			token.value = token.value + (lexer->current_char - '0') * fraction;
			lexer_advance(lexer);
		}
	}

	return token;
}

Token lexer_next_token(Lexer* lexer)
{
	Token token;

	lexer_skip_whitespace(lexer);

	int line = lexer->line;
	int column = lexer->column;

	switch (lexer->current_char)
	{
		case '\0':	token.type = TOKEN_EOF; break;
		case '+':	token.type = TOKEN_PLUS; break;
		case '-':	token.type = TOKEN_MINUS; break;
		case '*':	token.type = TOKEN_MUL; break;
		case '/':	token.type = TOKEN_DIV; break;
		case '(':	token.type = TOKEN_ORB; break;
		case ')':	token.type = TOKEN_CRB; break;
		case ';':	token.type = TOKEN_SEMICOLON; break;
		default:
			if (isdigit(lexer->current_char))
				return lexer_read_number(lexer);
			else
			{
				token.type = TOKEN_ERROR;
				
				fprintf(stderr, "Lexical error(%d, %d): unknown symbol '%c'.\n",
								line, column, lexer->current_char);
			}

			break;
	}

	token.line = line;
	token.column = column;
	token.value = 0;

	lexer_advance(lexer);

	return token;
}
