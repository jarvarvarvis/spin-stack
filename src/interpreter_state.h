#ifndef INTERPRETER_STATE_H
#define INTERPRETER_STATE_H

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include "error.h"

#define STACK_SIZE 32768
#define HEAP_SIZE  32768

typedef int16_t StorageDataType;

typedef struct {
    StorageDataType stack[STACK_SIZE];
    size_t stack_ptr;
    
    StorageDataType heap[HEAP_SIZE];
} InterpreterState;

InterpreterState *interpreter_state_new();
bool interpreter_state_push(InterpreterState *state, StorageDataType data);
bool interpreter_state_pop(InterpreterState *state, StorageDataType *out_data);
bool interpreter_state_handle_io_operator(InterpreterState *state, Error *current_error);
void interpreter_state_dump_stack(InterpreterState *state);
void interpreter_state_dump_heap(InterpreterState *state);

#endif /* INTERPRETER_STATE_H */
