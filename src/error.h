#ifndef ERROR_H
#define ERROR_H

#include <stddef.h>

#include "instruction.h"

typedef enum {
    ERROR_NONE                  = 0,
    ERROR_STACK_OVERFLOW        = 1,
    ERROR_STACK_UNDERFLOW       = 2,
    ERROR_INVALID_HEAP_ADDRESS  = 3,
} Error;

void output_error_info(Error current_error, Instruction *instructions, size_t instruction_ptr);

#endif /* ERROR_H */
