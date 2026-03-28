#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/merror.h"
#include "../include/saph.h"
#include "../include/codegen.h"


void get_root_path(char* buffer, DWORD size)
{
	if (GetModuleFileNameA(NULL, buffer, size) == 0)
		error("Error: cannot get executable path.")

	char* last_slash = strrchr(buffer, '\\');
	if (last_slash != NULL)
		*last_slash = '\0';
}

SaphCore* saph_init(char source[])
{
	SaphCore* core = malloc(sizeof(SaphCore));
	merror(core, "Saph")

	core->lexer = lexer_create(source);
	core->parser = parser_create(core->lexer);
	core->ast = parse_program(core->parser);
	core->bc = bytecode_create();
	bytecode_generate(core->bc, core->ast);
	bytecode_add(core->bc, OP_HALT);
	core->vm = vm_create(core->bc->code, core->bc->constants);

	return core;
}

void saph_free(SaphCore* core)
{
	if (core)
	{
		vm_destroy(core->vm);
		bytecode_destroy(core->bc);
		ast_free(core->ast);
		parser_destroy(core->parser);
		lexer_destroy(core->lexer);
		free(core);
	}
}

void saph_run(SaphCore* core, int debug)
{
	if (debug)
	{
		printf("====== AST ======\n");
		ast_print(core->ast, 0);
		printf("\n====== BYTECODE ======\n");
		bytecode_print(core->bc);
		printf("\n");
	}

	vm_run(core->vm);
}

void saph_build(SaphCore* core, char* filename, int debug)
{
	char fbuf[260], root_path[MAX_PATH];
	strcpy(fbuf, filename);
	strcat(fbuf, ".saph.c");
	FILE* fp = fopen(fbuf, "w");
	get_root_path(root_path, MAX_PATH);

	codegen_generate(fp, core, root_path);
	fclose(fp);

	char command[256];
	snprintf(command, sizeof(command), "gcc %s \"%s\\savm.obj\" -o %s.exe", fbuf, root_path, filename);
	system(command);
	
	if (debug == 1)
	{
		printf("%s", command);
		return;
	}

	remove(fbuf);
}
