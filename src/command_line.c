#include "command_line.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void print_usage(char *file_name) {
    printf("Usage: %s [OPTIONS] INPUT\n", file_name);
    printf("\nOptions:\n");
    printf("    -h, --help        Displays this message\n");
    printf("    -n, --normalized  Run in normalized mode\n");
    printf("    -v, --verbose     Verbose output (dump stack state)\n");
}
