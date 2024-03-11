"""
Extract only instructions from a spin-stack source file
"""

import sys

def print_usage(prog_name):
    print(f"Usage: {prog_name} INPUT")

def main():
    if len(sys.argv) < 2:
        print_usage(sys.argv[0])
        exit(-1)

    file_path = sys.argv[1]
    print(f"File: {file_path}")

    instructions = ""
    with open(file_path, "r") as input_file:
        for line in input_file.readlines():
            for char in line:
                if char.isdigit():
                    instructions += char

    print(f"Instructions:\n{instructions}")

if __name__ == "__main__":
    main()
