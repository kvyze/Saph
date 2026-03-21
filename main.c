#include <stdio.h>

#include "parser.h"


int main(void)
{
	const char* input = "1 + 1;";

	printf("Input: %s\n", input);

	Lexer* lexer = lexer_create(input);
	Parser* parser = parser_create(lexer);

	int result = parse_program(parser);
	printf("Result: %d", result);

	parser_destroy(parser);
	lexer_destroy(lexer);

	return 0;
}
