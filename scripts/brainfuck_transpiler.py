"""
Transpile brainfuck program to spin-stack
"""

INSTRUCTION_TABLE = {
    "+": "(00033 11403 111444 4) 5 5 03  (00033 11403 111444 4) 5 6",
    "-": "(00033 11403 111444 4) 5 5 023 (00033 11403 111444 4) 5 6",
    ">": "0003311403111444450300033114031114444232100033114031114444036000033114031114444035700033114031114444503000331140311144446012340123870123000331140311144446012381230039",
    "<": "0003311403111444450230310003311403111444403600003311403111444403570003311403111444450230003311403111444460123401238700033114031114444023000331140311144446012381230039",
    "[": "(00033 11403 111444 4) 5 5 7",
    "]": "(00033 11403 111444 4) 5 5 1 8",
    ".": "(00033 11403 111444 4) 5 5 003 9",
    ",": "0123 9 (00033 11403 111444 4) 5 6"
}

import sys

def print_usage(prog_name):
    print(f"Usage: {prog_name} INPUT OUTPUT")

def main():
    if len(sys.argv) < 3:
        print_usage(sys.argv[0])
        exit(-1)

    input_file_path = sys.argv[1]
    output_file_path = sys.argv[2]
    print(f"Input File: {input_file_path}")
    print(f"Output File: {output_file_path}")


    result = ""
    with open(input_file_path, "r") as input_file:
        for line in input_file.readlines():
            for c in line:
                if c in INSTRUCTION_TABLE:
                    result += INSTRUCTION_TABLE[c]
                    result += "\n"

    with open(output_file_path, "w") as output_file:
        output_file.write(result)

if __name__ == "__main__":
    main()

