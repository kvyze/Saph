#include <stdio.h>

#include "parser.h"
#include "bytecode.h"
#include "vm.h"


void compile(const char* input);

int main(int argc, char* argv[])
{
	compile("1 + 1 * 3");

	return 0;
}

void compile(const char* input)
{
	printf("Source: %s\n", input);

	Lexer* lexer = lexer_create(input);
	Parser* parser = parser_create(lexer);
	Bytecode* bc = bytecode_create();

	ASTNode* ast = parse_program(parser);
	printf("\n=== AST ===\n");
	ast_print(ast, 0);

	bytecode_generate(bc, ast);
	bytecode_add(bc, OP_HALT);
	printf("\n=== BYTECODE ===\n");
	bytecode_print(bc);

	VM* vm = vm_create(bc);
	int result = vm_run(vm);
	printf("\nResult: %d\n", result);

	vm_destroy(vm);
	bytecode_destroy(bc);
	ast_free(ast);
	parser_destroy(parser);
	lexer_destroy(lexer);
}
