#include "instruction.h"

char operator_to_character(Operator operator) {
    switch (operator) {
    case OPERATOR_END_OF_INSTRUCTIONS:
        return '?';
    case OPERATOR_PUSH_ONE:
    case OPERATOR_DUPLICATE:
    case OPERATOR_NEGATE:
    case OPERATOR_ADD:
    case OPERATOR_MULTIPLY:
    case OPERATOR_READ_HEAP:
    case OPERATOR_WRITE_HEAP:
    case OPERATOR_JUMP_FWD:
    case OPERATOR_JUMP_BCK:
    case OPERATOR_IO:
        return '0' + operator;
    }
}
