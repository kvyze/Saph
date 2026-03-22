#include <stdio.h>

#include "parser.h"
#include "bytecode.h"


int main(void)
{
	const char* input = "17 + (134 - 80) * 3;";

	printf("Input: %s\n", input);

	Lexer* lexer = lexer_create(input);
	Parser* parser = parser_create(lexer);
	Bytecode* bc = bytecode_create();

	ASTNode* ast = parse_program(parser);
	printf("Result: %d\n\n=== AST ===\n", ast_eval(ast));
	ast_print(ast, 0);
	
	bytecode_generate(bc, ast);
	printf("\n=== BYTECODE ===\n");
	bytecode_print(bc);

	bytecode_destroy(bc);
	ast_free(ast);
	parser_destroy(parser);
	lexer_destroy(lexer);

	return 0;
}
