#pragma once

#include <stdint.h>
#include "opcode.h"

#define MAX_STACK 256

typedef struct
{
	double stack[MAX_STACK];
	int sp;
	int ip;
	uint8_t* program;
	double* constants;

	struct {
		char name[64];
		double value;
	} variables[MAX_STACK];
	int var_count;
} VM;

VM* vm_create(uint8_t* program, double* constants);
void vm_destroy(VM* vm);
void vm_run(VM* vm);
