#ifndef COMPILER_H
#define COMPILER_H

#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

#include "instruction.h"

Instruction *compile_file(FILE *input_file, bool normalized);

#endif /* COMPILER_H */
