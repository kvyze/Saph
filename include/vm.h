#pragma once

#include <stdint.h>

#include "opcode.h"

typedef struct
{
	double stack[256];
	int sp;
	int ip;
	uint8_t* program;
	double* constants;
} VM;

VM* vm_create(uint8_t* program, double* constants);
void vm_destroy(VM* vm);
void vm_run(VM* vm);
