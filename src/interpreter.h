#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "instruction.h"
#include "interpreter_state.h"

void interpreter_run(InterpreterState *state, Instruction *instructions, bool verbose);

#endif /* INTERPRETER_H */
