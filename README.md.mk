# spin-stack

An esoteric programming language with a rotating instruction set, stack memory and a simple heap.

A value on spin-stack's stack and heap is a signed 16-bit integer.

spin-stack's stack and heap both have a size of 32768 (=2^15).

## cloc output

```
@CLOC_OUTPUT@
```

## Instructions (normalized)

Below is a table that shows the normalized instruction set (without rotation applied):

| Instruction | Behavior                                                                                                                                     |
|-------------|----------------------------------------------------------------------------------------------------------------------------------------------|
|      0      | Push a one on the stack                                                                                                                      |
|      1      | Duplicate the top of the stack                                                                                                               |
|      2      | Negate the top of the stack                                                                                                                  |
|      3      | Add the top two values of the stack and push the result                                                                                      |
|      4      | Multiply the top two values of the stack and push the result                                                                                 |
|      5      | Pop one value off the stack and use it as a heap address - push the value at that address on the stack                                       |
|      6      | Pop two values off the stack, the first is the address and the second the value - then write the value to the heap at the specified address  |
|      7      | Pop a value off the stack, if it is zero, jump to the corresponding 8                                                                        |
|      8      | Pop a value off the stack, if it is not zero, jump to the corresponding 7                                                                    |
|      9      | IO operation - pop one value off the stack and use it as a specifier                                                                         |

Note that negative heap addresses cause a crash.

## IO operations

| Specifier | Behavior                                                                           |
|-----------|------------------------------------------------------------------------------------|
|     0     | Read an ASCII character from the command line and push the value on the stack      |
|     1     | Read an integer from the command line and push the (truncated) value on the stack  |
|     2     | Pop one value off the stack and write it to the command line as a character        |
|     3     | Pop one value off the stack and write it to the command line as an integer         |
| otherwise | Do nothing                                                                         |

## Rotation

I have now introduced the entire instruction set, but the other important component to spin-stack is the rotation:

After each instruction, the instruction set is rotated right: 0 becomes 1, 1 becomes 2, ..., and 9 becomes 0

The IO operation specifiers won't rotate but this might change in the future.

## Example programs (normalized)

### Truth machine

(see examples/normalized/truth_machine_normalized.rtr for more info)

```
  0 1 7     1
1       0   1
        2   3
      9     0
    0       3

    9       8
```

### Hello world

This is very likely not the most optimal solution.

Unformatted:
```
003131300330031414130300033140314100313133100033133100033231003131330031313131300033140310340310003323110031313230300313131143
0039003900390039003900390039003900390039003900390039
```

## Example program (unnormalized)

### Truth machine

```
  0 2 9     4
5       5   7
        9   1
      8     0
    1       5

    2       2
```

### Hello world

This is very likely not the most optimal program.

Unformatted:
```
015476978231265971920423489829326445981034334590923223489910015476908932547698190156596099352655671224344598103353756092143488
6718015245968930237467180152459689302374671801524596
```

This is even more incomprehensible.
