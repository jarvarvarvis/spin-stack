#include "interpreter_state.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

InterpreterState *interpreter_state_new() {
    InterpreterState *state = malloc(sizeof(InterpreterState));
    
    state->stack_ptr = 0;
    memset(state->stack, 0, sizeof(StorageDataType) * STACK_SIZE);
    memset(state->heap, 0, sizeof(StorageDataType) * HEAP_SIZE);

    return state;
}

bool interpreter_state_push(InterpreterState *state, StorageDataType data) {
    if (state->stack_ptr >= STACK_SIZE) {
        return false;
    }

    state->stack[state->stack_ptr] = data;
    state->stack_ptr++;

    return true;
}

bool interpreter_state_pop(InterpreterState *state, StorageDataType *out_data) {
    if (state->stack_ptr == 0) {
        return false;
    }
    
    state->stack_ptr--;

    *out_data = state->stack[state->stack_ptr];
    state->stack[state->stack_ptr] = 0;
    
    return true;
}

bool interpreter_state_handle_io_operator(InterpreterState *state, Error *current_error) {
    StorageDataType io_specifier;
    if (!interpreter_state_pop(state, &io_specifier)) *current_error = ERROR_STACK_UNDERFLOW;

    // Read ASCII character from command line and push on stack
    if (io_specifier == 0) {
        char character = getchar();
        if (character == EOF) return false;

        if (!interpreter_state_push(state, character)) *current_error = ERROR_STACK_OVERFLOW;
    } 
    // Read int value from command line and push on stack
    else if (io_specifier == 1) {
        StorageDataType int_value;
        int result = scanf("%hd", &int_value); 
        
        if (!interpreter_state_push(state, int_value)) *current_error = ERROR_STACK_OVERFLOW;
    }
    // Pop value from stack and write as character
    else if (io_specifier == 2) {
        StorageDataType value;
        if (interpreter_state_pop(state, &value)) {
            putchar(value);
        } else {
            *current_error = ERROR_STACK_UNDERFLOW;
        }
    }
    // Pop value from stack and write as integer
    else if (io_specifier == 3) {
        StorageDataType value;
        if (interpreter_state_pop(state, &value)) {
            printf("%hd", value);
        } else {
            *current_error = ERROR_STACK_UNDERFLOW;
        }
    }

    return true;
}

void interpreter_state_dump_stack(InterpreterState *state) {
    printf("Stack Pointer: %lu\n", state->stack_ptr);
    printf("Stack Dump: ");
    for (size_t i = 0; i < state->stack_ptr; ++i) {
        printf("%d ", state->stack[i]);
    }
    printf("\n");
}

void interpreter_state_dump_heap(InterpreterState *state) {
    printf("Heap Dump:\n");

    for (size_t i = 0; i < HEAP_SIZE; ++i) {
        StorageDataType curr_value = state->heap[i];
        size_t range_start = i;
        
        // Check next values on the heap for repetitions
        while (i < HEAP_SIZE && state->heap[i + 1] == curr_value) {
            i++;
        }

        // Output
        printf("%05lu - %05lu | %hd\n", range_start, i, curr_value);
    }
}
