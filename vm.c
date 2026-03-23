#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "merror.h"

#define READ_OPCODE vm->program->code[vm->ip++]
#define READ_STACK vm->stack[--vm->sp]
#define PUSH_STACK vm->stack[vm->sp++]


VM* vm_create(Bytecode* program)
{
	VM* vm = malloc(sizeof(VM));
	merror(vm, "Virtual machine")

	vm->sp = 0;
	vm->ip = 0;
	vm->program = program;

	return vm;
}

void vm_destroy(VM* vm)
{
	if (vm)
		free(vm);
}

int vm_run(VM* vm)
{
	while (1)
	{
		int opcode = READ_OPCODE;

		switch (opcode)
		{
			case OP_PUSH:
			{
				int value = READ_OPCODE;
				PUSH_STACK = value;
				break;
			}

			case OP_ADD:
			{
				int right = READ_STACK;
				int left = READ_STACK;
				PUSH_STACK = left + right;
				break;
			}

			case OP_SUB:
			{
				int right = READ_STACK;
				int left = READ_STACK;
				PUSH_STACK = left - right;
				break;
			}

			case OP_MUL:
			{
				int right = READ_STACK;
				int left = READ_STACK;
				PUSH_STACK = left * right;
				break;
			}

			case OP_DIV:
			{
				int right = READ_STACK;

				if (right == 0)
					error("ZeroDivisionError: division by 0.")

				int left = READ_STACK;
				PUSH_STACK = left - right;
				break;
			}

			case OP_NEG:
			{
				int value = READ_STACK;
				PUSH_STACK = -value;
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
