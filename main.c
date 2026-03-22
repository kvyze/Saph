#include <stdio.h>

#include "parser.h"


int main(void)
{
	const char* input = "(3 + 5) * 8;";

	printf("Input: %s\n", input);

	Lexer* lexer = lexer_create(input);
	Parser* parser = parser_create(lexer);

	ASTNode* ast = parse_program(parser);
	printf("Result: %d\n\n=== AST ===\n", ast_eval(ast));
	ast_print(ast, 0);

	ast_free(ast);
	parser_destroy(parser);
	lexer_destroy(lexer);

	return 0;
}
