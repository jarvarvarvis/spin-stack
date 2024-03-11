#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stddef.h>

typedef enum {
    OPERATOR_END_OF_INSTRUCTIONS    = -1,
    OPERATOR_PUSH_ONE               = 0,
    OPERATOR_DUPLICATE              = 1,
    OPERATOR_NEGATE                 = 2,
    OPERATOR_ADD                    = 3,
    OPERATOR_MULTIPLY               = 4,
    OPERATOR_READ_HEAP              = 5,
    OPERATOR_WRITE_HEAP             = 6,
    OPERATOR_JUMP_FWD               = 7,
    OPERATOR_JUMP_BCK               = 8,
    OPERATOR_IO                     = 9,
} Operator;

char operator_to_character(Operator operator);

typedef struct {
    size_t operator;
    size_t operand;
} Instruction;

#endif /* INSTRUCTION_H */
