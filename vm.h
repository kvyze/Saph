#pragma once

#include "bytecode.h"

typedef struct
{
	int stack[256];
	int sp;
	int ip;
	Bytecode* program;
} VM;

VM* vm_create(Bytecode* program);
void vm_destroy(VM* vm);
int vm_run(VM* vm);
