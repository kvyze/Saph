#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/vm.h"
#include "../include/merror.h"

#define READ_OPCODE vm->program[vm->ip++]
#define READ_STACK vm->stack[--vm->sp]
#define PUSH_STACK(v) vm->stack[vm->sp++] = v


VM* vm_create(uint8_t* program, double* constants)
{
	VM* vm = malloc(sizeof(VM));
	merror(vm, "Virtual machine")

	vm->sp = 0;
	vm->ip = 0;
	vm->program = program;
	vm->constants = constants;

	return vm;
}

void vm_destroy(VM* vm)
{
	if (vm)
		free(vm);
}

double vm_run(VM* vm)
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
			{
				if (vm->sp > 0)
					return READ_STACK;

				return 0;
			}

			default:
				error("Error: unknown opcode (VM).")
		}
	}
}
