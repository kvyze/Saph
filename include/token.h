#pragma once

#define MAX_WORD 64

typedef enum
{
	TOKEN_NUMBER,
	TOKEN_WORD,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_MUL,
	TOKEN_DIV,
	TOKEN_ORB,
	TOKEN_CRB,
	TOKEN_SEMICOLON,
	TOKEN_EOF,
	TOKEN_ERROR,
	TOKEN_ASSIGN
} Saph_TokenType;

typedef struct
{
	Saph_TokenType type;
	double value;
	char word[MAX_WORD];
	int line;
	int column;
} Token;

const char* token_type_to_string(Saph_TokenType type);
