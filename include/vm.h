#pragma once

#include "opcode.h"

typedef struct
{
	int stack[256];
	int sp;
	int ip;
	int* program;
} VM;

VM* vm_create(int* program);
void vm_destroy(VM* vm);
int vm_run(VM* vm);
