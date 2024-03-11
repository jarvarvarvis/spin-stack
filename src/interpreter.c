#include "interpreter.h"

#include <stdio.h>

void interpreter_run(InterpreterState *state, Instruction *instructions, bool verbose) {
    size_t instruction_ptr = 0;
    while (true) {
        Instruction current_instruction = instructions[instruction_ptr];
        if (current_instruction.operator == OPERATOR_END_OF_INSTRUCTIONS) {
            break;
        }

        bool performed_jump = false;
        Error current_error = ERROR_NONE;

        if (verbose) {
            char op = operator_to_character(current_instruction.operator);
            printf("Current instruction (index = %lu): %c\n", instruction_ptr, op);
        }

        // Push a one
        if (current_instruction.operator == OPERATOR_PUSH_ONE) {
            if (!interpreter_state_push(state, 1)) current_error = ERROR_STACK_OVERFLOW;
        }

        // Duplicate the top of the stack
        if (current_instruction.operator == OPERATOR_DUPLICATE) {
            StorageDataType top = 0;
            if (interpreter_state_pop(state, &top)) {
                // Push top data twice
                if (!interpreter_state_push(state, top)) current_error = ERROR_STACK_OVERFLOW;
                if (!interpreter_state_push(state, top)) current_error = ERROR_STACK_OVERFLOW;
            } else {
                current_error = ERROR_STACK_UNDERFLOW;
            }
        }

        // Negate the top of the stack
        if (current_instruction.operator == OPERATOR_NEGATE) {
            if (state->stack_ptr == 0) {
                current_error = ERROR_STACK_UNDERFLOW;
            } else {
                // Negate the top of the stack
                state->stack[state->stack_ptr - 1] *= -1;
            }
        }

        // Add the top two values of the stack and push the result
        if (current_instruction.operator == OPERATOR_ADD) {
            StorageDataType a = 0, b = 0;
            if (interpreter_state_pop(state, &a) && interpreter_state_pop(state, &b)) {
                interpreter_state_push(state, a + b);
            } else {
                current_error = ERROR_STACK_UNDERFLOW;
            }
        }

        // Multiply the top two values of the stack and push the result
        if (current_instruction.operator == OPERATOR_MULTIPLY) {
            StorageDataType a = 0, b = 0;
            if (interpreter_state_pop(state, &a) && interpreter_state_pop(state, &b)) {
                interpreter_state_push(state, a * b);
            } else {
                current_error = ERROR_STACK_UNDERFLOW;
            }
        }

        // Heap read
        if (current_instruction.operator == OPERATOR_READ_HEAP) {
            StorageDataType heap_addr;
            if (interpreter_state_pop(state, &heap_addr)) {
                if (heap_addr < 0) current_error = ERROR_INVALID_HEAP_ADDRESS;
                else {
                    // Read heap at the address and push value on the stack
                    StorageDataType heap_value = state->heap[heap_addr];
                    if (!interpreter_state_push(state, heap_value)) current_error = ERROR_STACK_OVERFLOW;
                }
            } else {
                current_error = ERROR_STACK_UNDERFLOW;
            }
        }

        // Heap write
        if (current_instruction.operator == OPERATOR_WRITE_HEAP) {
            StorageDataType heap_addr = 0, written_value = 0;
            if (interpreter_state_pop(state, &heap_addr) && interpreter_state_pop(state, &written_value)) {
                if (heap_addr < 0) current_error = ERROR_INVALID_HEAP_ADDRESS;
                else {
                    // Write value to the heap at the given address
                    state->heap[heap_addr] = written_value;
                }
            } else {
                current_error = ERROR_STACK_UNDERFLOW;
            }
        }

        // Loop
        if (current_instruction.operator == OPERATOR_JUMP_FWD) {
            StorageDataType condition = 0;
            if (interpreter_state_pop(state, &condition)) {
                if (!condition) {
                    // Jump behind the matching operator
                    instruction_ptr = current_instruction.operand + 1;

                    performed_jump = true;
                }
            } else {
                current_error = ERROR_STACK_UNDERFLOW;
            }
        }
        if (current_instruction.operator == OPERATOR_JUMP_BCK) {
            StorageDataType condition = 0;
            if (interpreter_state_pop(state, &condition)) {
                if (condition) {
                    instruction_ptr = current_instruction.operand;
                    performed_jump = true;
                }
            } else {
                current_error = ERROR_STACK_UNDERFLOW;
            }
        }

        // IO operation
        if (current_instruction.operator == OPERATOR_IO) {
            if (!interpreter_state_handle_io_operator(state, &current_error)) {
                break;
            }
        }

        // Dump stack and heap if in verbose mode
        if (verbose) {
            interpreter_state_dump_stack(state);
            interpreter_state_dump_heap(state);
            printf("\n");
        }

        // Handle error
        if (current_error != 0) {
            output_error_info(current_error, instructions, instruction_ptr);
            break;
        }

        // Exit loop if the end of instructions has already been reached
        if (instructions[instruction_ptr].operator == OPERATOR_END_OF_INSTRUCTIONS) {
            break;
        }

        if (!performed_jump)
            instruction_ptr++;
    }
}
