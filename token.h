#pragma once

typedef enum
{
	TOKEN_NUMBER,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_MUL,
	TOKEN_DIV,
	TOKEN_ORB,
	TOKEN_CRB,
	TOKEN_SEMICOLON,
	TOKEN_EOF,
	TOKEN_ERROR
} TokenType;

typedef struct
{
	TokenType type;
	int value;
	int line;
	int column;
} Token;

const char* token_type_to_string(TokenType type);
