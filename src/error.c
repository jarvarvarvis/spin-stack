#include "error.h"
#include "maths.h"

#include <stdio.h>

void output_error_info(Error current_error, Instruction *instructions, size_t instruction_ptr) {
    #define CHECK_ERROR(error_type) (current_error == error_type) ? #error_type

    char *err_type =
        CHECK_ERROR(ERROR_STACK_OVERFLOW) : 
        CHECK_ERROR(ERROR_STACK_UNDERFLOW) :
        CHECK_ERROR(ERROR_INVALID_HEAP_ADDRESS) :
        "Unknown";

    #undef CHECK_ERROR
    
    printf("Encountered error: %s (%d) at instruction index %lu\n", err_type, current_error, instruction_ptr);
    
    printf("Instruction region: ");
    size_t left_index = instruction_ptr - min(instruction_ptr, 5);
    size_t right_index = instruction_ptr + 5;
    for (size_t j = left_index; j < right_index; ++j) {
        char op_char = operator_to_character(instructions[j].operator);
        if (j == instruction_ptr) {
            printf("[%c]", op_char);
        } else {
            printf("%c", op_char);
        }
    }

    printf("\n");
}

