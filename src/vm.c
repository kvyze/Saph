#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/vm.h"
#include "../include/merror.h"

#define READ_OPCODE vm->program[vm->ip++]
#define READ_STACK vm->stack[--vm->sp]
#define PUSH_STACK(v) vm->stack[vm->sp++] = v
#define READ_VAR_NAME(n) { \
	uint8_t name_len = READ_OPCODE; \
	for (int i = 0; i < name_len; i++) \
		n[i] = READ_OPCODE; \
	n[name_len] = '\0'; \
}


void print_double(double num)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%.10f", num);
	char* end = buffer + strlen(buffer) - 1;
	while (*end == '0') end--;
	if (*end == '.') end--;
	*(end + 1) = '\0';
	printf("%s\n", buffer);
}

int vm_get_var(VM* vm, const char* name)
{
	for (int i = 0; i < vm->var_count; i++)
	{
		if (strcmp(vm->variables[i].name, name) == 0)
			return i;
	}

	return -1;
}

void vm_store_var(VM* vm, const char* name, double value)
{
	int index = vm_get_var(vm, name);

	if (index == -1)
	{
		if (vm->var_count >= MAX_STACK)
			error("Error: too many variables.")

		strcpy(vm->variables[vm->var_count].name, name);
		vm->variables[vm->var_count++].value = value;
	}
	else
		vm->variables[index].value = value;
}

double vm_load_var(VM* vm, const char* name)
{
	int index = vm_get_var(vm, name);

	if (index == -1)
	{
		fprintf(stderr, "NameError: variable '%s' doesn't exist.", name);
		exit(1);
	}

	return vm->variables[index].value;
}

VM* vm_create(uint8_t* program, double* constants)
{
	VM* vm = malloc(sizeof(VM));
	merror(vm, "Virtual machine")

	vm->sp = 0;
	vm->ip = 0;
	vm->program = program;
	vm->constants = constants;
	vm->var_count = 0;

	return vm;
}

void vm_destroy(VM* vm)
{
	if (vm)
		free(vm);
}

void vm_run(VM* vm)
{
	while (1)
	{
		uint8_t opcode = READ_OPCODE;

		switch (opcode)
		{
			case OP_PUSH:
			{
				uint8_t index = READ_OPCODE;
				PUSH_STACK(vm->constants[index]);
				break;
			}

			case OP_ADD:
			{
				double right = READ_STACK;
				double left = READ_STACK;
				PUSH_STACK(left + right);
				break;
			}

			case OP_SUB:
			{
				double right = READ_STACK;
				double left = READ_STACK;
				PUSH_STACK(left - right);
				break;
			}

			case OP_MUL:
			{
				double right = READ_STACK;
				double left = READ_STACK;
				PUSH_STACK(left * right);
				break;
			}

			case OP_DIV:
			{
				double right = READ_STACK;

				if (right == 0)
					error("ZeroDivisionError: division by 0.")

				double left = READ_STACK;
				PUSH_STACK(left / right);
				break;
			}

			case OP_NEG:
			{
				double value = READ_STACK;
				PUSH_STACK(-value);
				break;
			}

			case OP_HALT:
				return;

			case OP_PRINT:
			{
				double value = READ_STACK;
				print_double(value);
				break;
			}

			case OP_LOAD:
			{
				char name[64];
				READ_VAR_NAME(name)
				PUSH_STACK(vm_load_var(vm, name));
				break;
			}

			case OP_STORE:
			{
				char name[64];
				READ_VAR_NAME(name)
				vm_store_var(vm, name, READ_STACK);
				break;
			}

			default:
				error("Error: unknown opcode (VM).")
		}
	}
}
