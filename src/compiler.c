#include "compiler.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define JUMP_STACK_SIZE 1024

Instruction *compile_file(FILE *input_file, bool normalized) {
    char chunk[128];
    size_t instructions_size = 256;
    Instruction *instructions = malloc(sizeof(Instruction) * instructions_size);
    size_t instructions_index = 0;

    int rotation = 0;

    // Read only the instructions
    while(fgets(chunk, sizeof(chunk), input_file) != NULL) {
        for (int i = 0; i < 128; ++i) {
            char current_char = chunk[i];
            if (current_char == '\0') break; // End of string

            if (isdigit(current_char)) {
                // Reallocate the buffer if necessary
                if (instructions_index >= instructions_size) {
                    instructions_size *= 2;
                    instructions = realloc(instructions, sizeof(Instruction) * instructions_size);
                }

                // Rotate current operator value
                int instruction_value = current_char - '0';
                int rotated_op = instruction_value >= rotation
                    ? (instruction_value - rotation)
                    : (10 - (rotation - instruction_value));
                rotated_op %= 10;

                instructions[instructions_index].operator = rotated_op;
                instructions_index++;

                // Increment rotation (if not in normalized mode)
                if (!normalized) {
                    rotation++;
                    rotation %= 10;
                }
            }
        }
    }

    // Reallocate the buffer one last time if necessary
    if (instructions_index >= instructions_size) {
        instructions_size += 1;
        instructions = realloc(instructions, sizeof(Instruction) * instructions_size);
    }

    // Insert invalid instruction at the end to signify end
    instructions[instructions_index].operator = OPERATOR_END_OF_INSTRUCTIONS;

    // Handle jump forward and jump backward instructions
    size_t jump_stack[JUMP_STACK_SIZE], jump_stack_ptr = 0;
    size_t instruction_ptr = 0;

    while (true) {
        Instruction current_instruction = instructions[instruction_ptr];
        if (current_instruction.operator == OPERATOR_END_OF_INSTRUCTIONS) {
            break;
        }

        if (current_instruction.operator == OPERATOR_JUMP_FWD) {
            if (jump_stack_ptr >= JUMP_STACK_SIZE) {
                fprintf(stderr, "Too much loop nesting: reached jump stack size of %d\n", JUMP_STACK_SIZE);
                free(instructions);
                return NULL;
            }

            jump_stack[jump_stack_ptr] = instruction_ptr;
            jump_stack_ptr++;
        }
        if (current_instruction.operator == OPERATOR_JUMP_BCK) {
            if (jump_stack_ptr == 0) {
                fprintf(stderr, "Encountered jump back instruction without matching jump forward instruction at index %lu\n", instruction_ptr);
                free(instructions);
                return NULL;
            }

            jump_stack_ptr--;
            size_t matched_instruction_ptr = jump_stack[jump_stack_ptr];

            // Link jump instructions
            instructions[instruction_ptr].operand = matched_instruction_ptr;
            instructions[matched_instruction_ptr].operand = instruction_ptr;
        }

        instruction_ptr++;
    }

    return instructions;
}
