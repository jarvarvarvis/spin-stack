"""
Denormalize spin-stack instructions in a file
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

    rotation = 0

    result = ""
    with open(file_path, "r") as input_file:
        for line in input_file.readlines():
            stripped_line = line.replace("\n", "").replace("\r", "")
            new_line = ""
            for char in stripped_line:
                if char.isdigit():
                    rotated_value = (int(char) + rotation) % 10
                    new_line += str(rotated_value)

                    rotation += 1
                    rotation = rotation % 10
                else:
                    new_line += char

            result += new_line + "\n"
            print(f"   {stripped_line}")
            print(f"=> {new_line}")

    print()
    print(f"Result:\n{result}")

if __name__ == "__main__":
    main()
