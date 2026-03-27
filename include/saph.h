#pragma once

#include "parser.h"
#include "bytecode.h"
#include "vm.h"

#include <Windows.h>

#define BUILD "Saph 0.2.0 win64"


typedef struct
{
	Lexer* lexer;
	Parser* parser;
	Bytecode* bc;
	ASTNode* ast;
	VM* vm;
} SaphCore;

void get_root_path(char* buffer, DWORD size);
SaphCore* saph_init(char source[]);
void saph_free(SaphCore* core);
void saph_run(SaphCore* core, int debug);
void saph_build(SaphCore* core, char* filename, int debug);
