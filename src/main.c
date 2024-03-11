#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "command_line.h"
#include "compiler.h"
#include "instruction.h"
#include "interpreter.h"
#include "interpreter_state.h"

int main(int argc, char **argv) {
    bool normalized = false;
    bool verbose = false;
    char *input_file_path = NULL;

    // No arguments were supplied
    if (argc == 1) {
        print_usage(argv[0]);
        return -1;
    }

    // Parse command line arguments
    for (int i = 1; i < argc - 1; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return -1;
        }

        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
            continue;
        }
        
        if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--normalized") == 0) {
            normalized = true;
            continue;
        }

        printf("Unknown option: \"%s\"\n", argv[i]);
        print_usage(argv[0]);
        return -1;
    }

    // Check if last argument is help flag
    if (strcmp(argv[argc - 1], "-h") == 0 || strcmp(argv[argc - 1], "--help") == 0) {
        print_usage(argv[0]);
        return -1;
    }

    input_file_path = argv[argc - 1];

    printf("Input file path: %s\n", input_file_path);
    printf("Normalized? %s\n", normalized ? "Yes" : "No");
    printf("Verbose? %s\n", verbose ? "Yes" : "No");
    printf("\n");

    // Read input file
    FILE *input_file = fopen(input_file_path, "r");
    if (input_file == NULL) {
        perror("Failed to open input file");
        return -1;
    }

    InterpreterState *state = interpreter_state_new();
    Instruction *instructions = compile_file(input_file, normalized);
    if (instructions) {
        interpreter_run(state, instructions, verbose);
    }

    // Clean up
    fclose(input_file);
    free(instructions);
    free(state);

    return 0;
}
