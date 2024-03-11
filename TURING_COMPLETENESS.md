# Turing completeness considerations

If we can translate any brainfuck program to spin-stack, then spin-stack should be Turing complete.
Since it doesn't use brainfucks tape cell sizes (spin-stack uses 16 bit signed integers instead of Brainfuck's 8 bit unsigned ones), this obviously
doesn't work for all programs (but for the Hello World programs I have tested, so that's fine I guess).

You could obviously clamp the tape cells to the range `[0,255]` for every `+` and `-` operator and then it should work.

We allocate the first 30000 addresses on spin-stack's heap to brainfucks tape.
After that, we store things like the memory ptr or temporary values for equality checks.

We can represent the value 30000 like the following:
```
000 33   # => three
11 4 03  # => ten
111 444  # => ten^four
4        # => thirty thousand
```

In short: `00033 11403 111444 4`


Translation table:

| Brainfuck Instruction | Rotor instructions (normalized)                                                                                                                                           |
|-----------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|          `+`          | `(00033 11403 111444 4) 5 5 03  (00033 11403 111444 4) 5 6`                                                                                                               |
|          `-`          | `(00033 11403 111444 4) 5 5 023 (00033 11403 111444 4) 5 6`                                                                                                               |
|          `>`          | `0003311403111444450300033114031114444232100033114031114444036000033114031114444035700033114031114444503000331140311144446012340123870123000331140311144446012381230039`  |
|          `<`          | `0003311403111444450230310003311403111444403600003311403111444403570003311403111444450230003311403111444460123401238700033114031114444023000331140311144446012381230039`  |
|          `[`          | `(00033 11403 111444 4) 5 5 7`                                                                                                                                            |
|          `]`          | `(00033 11403 111444 4) 5 5 1 8`                                                                                                                                          |
|          `.`          | `(00033 11403 111444 4) 5 5 003 9`                                                                                                                                        |
|          `,`          | `0123 9 (00033 11403 111444 4) 5 6`                                                                                                                                       |

## Tape pointer modification

The tape pointer modification instructions should work like this:
```
load ptr value from heap

increment/decrement pointer value

if ptr value out of range
    wrap around pointer value
end

store ptr value back in heap
```

The first step is easy:
```
# Load ptr value
(00033 11403 111444 4) 5

# Increment
03

# or decrement
023
```

For the `<` instruction, we have to wrap around if the value is -1 (which should be easy enough):
```
# Load ptr value
(00033 11403 111444 4) 5

# Decrement
023

# If the value is negative one, set it back to thirty thousand - one
# Subtract: ptr - (-one) = ptr + one
03

# Store difference for later
1
(00033 11403 111444 4) 03  # heap addr
6

# Prepare stack: <diff> one <diff>
0
(00033 11403 111444 4) 03  # same heap addr
5

# If the value is not zero...
7

# Update pointer to the new value
(00033 11403 111444 4) 5
023
(00033 11403 111444 4)
6

# At the end, the stack can contain anything, but there must be a zero on top
# (for multiple checks get it to the format: ... <input> <difference> zero)
# Replace one with a zero
0123 4

0123
8

# One or zero remains on the stack

# If the value is zero...
7

# Set ptr to thirty thousand - one
(00033 11403 111444 4) 023
(00033 11403 111444 4)
6

0123
8

# Clear stack
123 0039
```

For the `>` instruction, we have to wrap around if the value is 30000:
```
# Load ptr value
(00033 11403 111444 4) 5

# Increment
03

# If the value is thirty thousand, set it back to zero
# Subtract: ptr - thirty thousand = -(thirty thousand - ptr)
(00033 11403 111444 4)
232

# Store difference for later
1
(00033 11403 111444 4) 03  # heap addr
6

# Prepare stack: <diff> one <diff>
0
(00033 11403 111444 4) 03  # same heap addr
5

# If the value is not thirty thousand...
7

# Update pointer to the new value
(00033 11403 111444 4) 5
03
(00033 11403 111444 4)
6

# At the end, the stack can contain anything, but there must be a zero on top
# (for multiple checks get it to the format: ... <input> <difference> zero)
# Replace one with a zero
0123 4

0123
8

# One or zero remains on the stack

# If the value is thirty thousand...
7

# Set ptr to zero
0123
(00033 11403 111444 4)
6

0123
8

# Clear stack
123 0039
```

## Loops

The brackets work like this:

For `[`:
```
# Load current tape value
(00033 11403 111444 4) 5 5

7
```

For `]`:
```
# Load current tape value and duplicate (for the matching seven instruction)
(00033 11403 111444 4) 5 5
1

8
```
