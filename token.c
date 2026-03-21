#include "token.h"

const char* token_type_to_string(TokenType type)
{
	switch (type)
	{
		case TOKEN_NUMBER:		return "NUMBER";
		case TOKEN_PLUS:		return "PLUS";
		case TOKEN_MINUS:		return "MINUS";
		case TOKEN_MUL:			return "MULTIPLY";
		case TOKEN_DIV:			return "DIVIDE";
		case TOKEN_ORB:			return "OPEN_ROUND_BRACKET";
		case TOKEN_CRB:			return "CLOSE_ROUND_BRACKET";
		case TOKEN_SEMICOLON:	return "SEMICOLON";
		case TOKEN_EOF:			return "EOF";
		case TOKEN_ERROR:		return "ERROR";
		default:				return "UNDEFINED";
	}
}
